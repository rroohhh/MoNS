/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  io_stream.h -- Templated io_stream implementation, with csi sequence handling
*/

#ifndef __IO_STREAM_H_
#define __IO_STREAM_H_

#include <fcntl.h>
#include <memory>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "ascii.h"
#include "csi_sequence.h"

struct stdio {
    stdio(const int in = 0, const int out = 1) noexcept : in(in), out(out) {
        termios settings{};
        tcgetattr(in, &settings);
        old_settings = settings;
        settings.c_iflag |= IGNBRK;
        settings.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
        settings.c_lflag &=
            ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
        settings.c_cflag &= ~CSIZE;
        settings.c_cflag |= CS8;
        settings.c_cc[VMIN]  = 1;
        settings.c_cc[VTIME] = 0;
        tcsetattr(in, TCSANOW, &settings);
        new_settings = settings;
    }

    stdio(const stdio & other)
        : in(other.in), out(other.out), old_settings(other.old_settings) {
        termios settings{};
        tcgetattr(in, &settings);
        settings.c_iflag |= IGNBRK;
        settings.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
        settings.c_lflag &=
            ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
        settings.c_cflag &= ~CSIZE;
        settings.c_cflag |= CS8;
        settings.c_cc[VMIN]  = 1;
        settings.c_cc[VTIME] = 0;
        tcsetattr(in, TCSANOW, &settings);
        new_settings = settings;
    }

    ~stdio() noexcept { tcsetattr(in, TCSANOW, &old_settings); }

    int write(const char * buffer, const int size) const noexcept {
        return ::write(out, buffer, size);
    }

    int read(char * buffer, const int size) const noexcept {
        /* termios settings{}; */
        /* tcgetattr(in, &settings); */

        /* if(check_termios(settings)) { */
            tcsetattr(in, TCSANOW, &new_settings);
        /* } */

        return ::read(in, buffer, size);
    }

    int close() noexcept {
        return tcsetattr(in, TCSANOW, &old_settings);
        /* return ::close(in), ::close(out); */
    }

    int read_nonblocking(char * buffer, const int size) const noexcept {
        /*
		 * termios settings{};
         * tcgetattr(in, &settings);
		 *
         * if(check_termios(settings)) {
         *     tcsetattr(in, TCSANOW, &new_settings);
         * }
		 */

        tcsetattr(in, TCSANOW, &new_settings);

        int flags = fcntl(in, F_GETFL, 0);
        fcntl(in, F_SETFL, flags | O_NONBLOCK);

        int read = this->read(buffer, size);

        fcntl(in, F_SETFL, flags);

        return read;
    }

    int get_width() const noexcept {
        struct winsize w {};
        ioctl(in, TIOCGWINSZ, &w);
        return w.ws_col;
    }

    int get_height() const noexcept {
        struct winsize w {};
        ioctl(in, TIOCGWINSZ, &w);
        return w.ws_row;
    }

    int in, out;

private:
    bool check_termios(termios settings) const noexcept {
        if(settings.c_iflag & IGNBRK)
            if(settings.c_iflag & ~(INLCR | ICRNL | IXON | IXOFF))
                if(settings.c_lflag &
                   ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN))
                    if(settings.c_cflag & ~CSIZE)
                        if(settings.c_cflag & CS8)
                            if(settings.c_cc[VMIN] == 1)
                                if(settings.c_cc[VTIME] == 0) return true;

		return false;
    }

    termios old_settings{}, new_settings{};
};

template <typename T>
class io_stream {
public:
    template <typename... Args>
    io_stream(Args &&... args) noexcept
        : io_impl(std::forward<Args>(args)...) {}

    int write(const std::pair<std::unique_ptr<char[]>, int> string) {
        return io_impl.write(string.first.get(), string.second);
    }

    int write(const short * buffer, const int size) const noexcept {
        return convert_and_write(buffer, size);
    }

    int write(const char * buffer, const int size) const noexcept {
        return io_impl.write(buffer, size);
    }

    int write(const std::string & s) const noexcept {
        return io_impl.write(s.c_str(), s.size());
    }

    int read(char * buffer, const int size) const noexcept {
        return io_impl.read(buffer, size);
    }

    int read(short * buffer, const int size) const noexcept {
        return read_converted(buffer, size);
    }

    int width() const noexcept { return io_impl.get_width(); }

    int height() const noexcept { return io_impl.get_height(); }

    int close() noexcept { return io_impl.close(); }

private:
    T io_impl;

    bool is_start_of_escape_sequence(const char to_test) const noexcept {
        return to_test == ascii::ESC;
    }

    void parse_args(const char * arg_buffer, const int /*size*/,
                    int * arguments, const short type) const noexcept {
        if(arg_buffer[0] == 0) {
            switch(type) {
            case csi_sequence::arrow_up:
            case csi_sequence::arrow_down:
            case csi_sequence::arrow_left:
            case csi_sequence::arrow_right:
                arguments[0] = 1;
                arguments[1] = -1;
                arguments[2] = -1;
                arguments[3] = -1;
                break;
            default:
                arguments[0] = 0;
                arguments[1] = -1;
                arguments[2] = -1;
                arguments[3] = -1;
            }
        } else {
            // TODO(robin): handle numeric arguments
        }
    }

    csi_sequence::esc_seq parse_csi() const noexcept {
        char                  in_buffer[1]     = {0};
        char                  arg_buffer[1024] = {0};
        int                   read             = 0;
        csi_sequence::esc_seq seq{};

        io_impl.read(in_buffer, sizeof(in_buffer));

        // numeric argument
        while((in_buffer[0] >= ascii::ZERO) &&
              (in_buffer[0] <= ascii::SEMICOLON)) {
            arg_buffer[read++] = in_buffer[0];
            io_impl.read(in_buffer, sizeof(in_buffer));
        }

        seq.type = csi_sequence::char_to_type(in_buffer[0]);
        parse_args(arg_buffer, sizeof(arg_buffer), seq.arguments, seq.type);

        return seq;
    }

    int read_maybe_escape_squence(short * buffer, const int size) const
        noexcept {
        char in_buffer[1];

        // don't wait -> only driver generated escape sequences are valid
        if(io_impl.read_nonblocking(in_buffer, 1) >= 1) {
            // check if this is really a escape sequence
            if(in_buffer[0] >= ascii::A && in_buffer[0] <= ascii::UNDERSCORE) {
                switch(in_buffer[0]) {
                case ascii::SQUARE_BRACKET_OPEN: {
                    csi_sequence::esc_seq seq = parse_csi();
                    switch(seq.type) {
                    case csi_sequence::arrow_up:
                    case csi_sequence::arrow_down:
                    case csi_sequence::arrow_left:
                    case csi_sequence::arrow_right: {
                        // NOTE(robin): loses sequences, when arguments[0] >
                        // size
                        int i = 0;
                        for(i = 0; (i < seq.arguments[0]) && (i < size); i++) {
                            buffer[i] = seq.type;
                        }
                        return i;
                    } break;

                    default:
                        buffer[0] = csi_sequence::unhandled;
                        return 1;
                    }
                } break;

                default:
                    buffer[0] = csi_sequence::unhandled;
                    return 1;
                    break;
                }
            } else {
                // find better solution?
                buffer[0] = ascii::ESC;
                return 1;
            }
        } else {
            buffer[0] = ascii::ESC;
            return 1;
        }
    }

    // NOTE(robin): implement bigger buffersize, if this is a performance
    // bottleneck
    int read_converted(short * buffer, const int size) const noexcept {
        int  num_read = 0;
        char in_buffer[1];

        while(num_read < size) {
            int read = io_impl.read(in_buffer, sizeof(in_buffer));

            if(read > 0) {
                if(is_start_of_escape_sequence(in_buffer[0])) {
                    num_read += read_maybe_escape_squence(&buffer[num_read],
                                                          size - num_read);
                } else {
                    buffer[num_read] = in_buffer[0];
                    num_read++;
                }
            } else {
                return num_read;
            }
        }

        return num_read;
    }

    int write_escape_sequence(const short seq) const noexcept {
        char buffer[3] = {ascii::ESC, ascii::SQUARE_BRACKET_OPEN};
        buffer[2]      = csi_sequence::type_to_char(seq);
        return io_impl.write(buffer, 3);
    }

    int convert_and_write(const short * buffer, const int size) const noexcept {
        int ret;
        int written = 0;

        for(int i = 0; i < size; i++) {
            if(buffer[0] > 255) {
                written += write_escape_sequence(buffer[i]);
            } else {
                if((ret = io_impl.write((char *)&buffer[i], 1)) != 1) {
                    return ret;
                }
                written += ret;
            }
        }

        return written;
    }
};

#endif
