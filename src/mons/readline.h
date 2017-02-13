/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  readline.h -- readline implementation based on io_stream, no global state and
  utf8 support
*/

#ifndef _READLINE_H
#define _READLINE_H

#include <experimental/optional>
#include <functional>

#include "ascii.h"
#include "csi_sequence.h"
#include "io_stream.h"
#include "utf8.h"

struct history {
    history(const int size) noexcept : index(0), size_(size) {
        hist.reserve(size);
    }

    void add(utf8string s) noexcept {
        hist.emplace(hist.begin() + (index++ % size_), s);
    }

    int size() const noexcept { return hist.size(); }

    int find_first(utf8string s) {
        for(int i = hist.size() - 1; i >= 0; i--) {
            if(hist[i % size_].contains(s)) {
                return -(i % size_ - hist.size());
            }
        }

        return -1;
    }

    utf8string get(const int index) const noexcept {
        return hist[(hist.size() - index) % size_];
    }

private:
    int                     index;
    int                     size_;
    std::vector<utf8string> hist;
};

using completion_function =
    std::function<std::vector<utf8string>(const utf8string s)>;

static completion_function
complete_from_canidates(std::vector<utf8string> possible_canidates) {
    return [=](utf8string current) -> std::vector<utf8string> {
        std::vector<utf8string> canidates;

        for(const auto & c : possible_canidates) {
            if(c.begins_with(current)) canidates.push_back(c);
        }

        return canidates;

    };
}

template <typename T>
struct readline {
    template <typename... Args>
    readline(int hist_size, Args &&... args) noexcept
        : io(std::forward<Args>(args)...), h(hist_size) {}

    void searching_off() {
        if(state == searching) {
            io.write((short *)&csi_sequence::arrow_down, 1);
            io.write(csi_sequence::clear_line,
                     sizeof(csi_sequence::clear_line));
            io.write((short *)&csi_sequence::arrow_up, 1);

            state = normal;
        }
    }

    void searching_on() {
        if(state != searching) {
            before_search_pos = cursor_pos;

            io.write(crlf, 2);
            io.write((short *)&csi_sequence::arrow_up, 1);

            search_input.clear();

            state = searching;

            before_search = current_read;
            current_read.clear();
            cursor_pos  = 0;
            search_hint = "history search: ";
        }
    }

    void complete() {
        std::vector<utf8string> completions;

        for(const auto & c : completer) {
            auto canidates = c(current_read);
            completions.insert(completions.end(), canidates.begin(),
                               canidates.end());
        }

        if(completions.size() == 1) {
            current_read = completions[0];
            cursor_pos   = current_read.length();
            update_line();
            return;
        } else if(completions.size() == 0) {
            return;
        }

        // NOTE(robin): output a fancy table
        // NOTE(robin): do i want sorting?
        int width     = io.width();
        int max_width = 0;

        for(const auto s : completions) {
            if(max_width < s.length()) max_width = s.length();
        }

        // NOTE(robin): spacing behind the item
        max_width += 5;

        int rows     = width / max_width;
        int num_rows = 0;

        for(auto & c : completions) {
            if((num_rows++ % rows) == 0) { io.write(crlf, sizeof(crlf)); }

            io.write(c.raw_data(), c.raw_length());

            for(int i = 0; i < max_width - c.length(); i++) {
                io.write((short *)&csi_sequence::arrow_right, 1);
            }
        }

        io.write(crlf, sizeof(crlf));
    }

    std::experimental::optional<utf8string>
    getline(utf8string prompt = {}) noexcept {
        current_read.clear();
        search_input.clear();
        cursor_pos = 0;
        hist_index = 0;
        state      = normal;

        this->prompt = prompt;

        do {
            short buffer[1];

            update_line();

            io.read(buffer, 1);

            if((buffer[0] == ascii::ETX) | (buffer[0] == ascii::EOT)) {
                if(state != searching) {
                    break;
                } else {
                    searching_off();
                    current_read = before_search;
                    cursor_pos   = before_search_pos;
                    continue;
                }
            }

            switch(buffer[0]) {
            case csi_sequence::arrow_up: {
                searching_off();

                hist_index += (hist_index < h.size());

                if(hist_index > 0) {
                    if(hist_index == 1) {
                        old_cursor_pos = cursor_pos;
                        old_read       = current_read;
                    }

                    current_read = h.get(hist_index);
                    cursor_pos   = current_read.length();
                }

                update_line();

                break;
            }
            case csi_sequence::arrow_down: {
                searching_off();

                bool was_bigger = hist_index > 0;
                hist_index -= was_bigger;

                if(hist_index == 0 && was_bigger) {
                    current_read = old_read;
                    cursor_pos   = old_cursor_pos;
                } else if(was_bigger) {
                    current_read = h.get(hist_index);
                    cursor_pos   = current_read.length();
                }

                update_line();

                break;
            }
            case csi_sequence::arrow_left: {
                searching_off();
                cursor_pos -= (cursor_pos > 0);
            } break;
            case csi_sequence::arrow_right: {
                searching_off();
                cursor_pos += (cursor_pos < current_read.length());
            } break;
            case ascii::HT: {
                if(state == searching) {
                    searching_off();
                } else {
                    complete();
                    previous_written = "";
                    continue;
                }
                previous_written = "";
            } break;
            case ascii::DEL:
                if(state == searching) {
                    if(search_input.length() > 0) {
                        search_input.erase(search_input.length() - 1);
                    }

                    int index = h.find_first(search_input);

                    if(index != -1) {
                        search_hint  = "history search: ";
                        current_read = h.get(index);
                        hist_index   = index;
                    } else {
                        search_hint = "history search (not found): ";
                    }
                } else {
                    if((cursor_pos - 1) < current_read.length() &&
                       cursor_pos > 0) {
                        current_read.erase(cursor_pos - 1);
                        cursor_pos -= (cursor_pos > 0);
                    }
                }
                break;
            case ascii::CR: {
                utf8string ret = current_read;

                current_read.clear();

                searching_off();
                io.write(crlf, 2);
                previous_written = "";

                return ret;
                break;
            }
            case ascii::DC2: {
                if(state != searching) {
                    searching_on();
                } else {
                    searching_off();
                }
                break;
            }
            default:
                utf8char to_read;
                to_read.chars[0] = buffer[0];

                // NOTE(robin): minus one, because we read already one char
                if((to_read.length() - 1) > 0) {
                    io.read(&to_read.chars[1], to_read.length() - 1);
                }

                if(state == searching) {
                    search_input.add(to_read);

                    int index = h.find_first(search_input);

                    if(index != -1) {
                        search_hint  = "history search: ";
                        current_read = h.get(index);
                        hist_index   = index;
                    } else {
                        search_hint = "history search (not found): ";
                    }
                } else {
                    current_read.insert(cursor_pos, to_read);
                    cursor_pos++;
                }
                break;
            }
        } while(true);

        return {};
    }

    void update_line() noexcept {
        utf8string to_write =
            previous_written.delta_update(prompt + current_read);

        int i = 0;

        for(i = 0; i < to_write.length(); i++) {
            if(!to_write[i].empty()) break;
        }

        io.write(csi_sequence::goto_x(i));

        for(int j = i; j < to_write.length(); j++) {
            if(!(to_write[j].empty()))
                io.write(to_write[j].chars, 4);
            else {
                io.write((short *)&csi_sequence::arrow_right, 1);
            }
        }

        previous_written = prompt + current_read;

        if(state == searching) {
            io.write((short *)&csi_sequence::arrow_down, 1);

            io.write(csi_sequence::clear_line,
                     sizeof(csi_sequence::clear_line));
            io.write(csi_sequence::beginning_of_line,
                     sizeof(csi_sequence::beginning_of_line));

            io.write(search_hint.c_str(), search_hint.size());
            io.write(search_input.raw_data(), search_input.raw_length());

            io.write((short *)&csi_sequence::arrow_up, 1);
        }

        io.write(csi_sequence::goto_x(cursor_pos + (int)prompt.length()));
        /*
                 * io.write(csi_sequence::beginning_of_line,
         *          sizeof(csi_sequence::beginning_of_line));
                 *
         * for(int i = 0; i < cursor_pos + (int)prompt.length(); i++) {
         *     io.write((short *)&csi_sequence::arrow_right, 1);
         * }
                 */
    }

    void history_add(const utf8string s) noexcept { h.add(s); }

    void add_completer(completion_function f) { completer.push_back(f); }

    void finish() { io.close(); }

private:
    enum readline_state { normal, searching, completing };

    readline_state state;

    int cursor_pos;
    int hist_index;
    int old_cursor_pos;
    int before_search_pos;

    utf8string old_read;
    utf8string current_read;
    utf8string previous_written;

    utf8string prompt;

    utf8string  before_search;
    utf8string  search_input;
    std::string search_hint;

    io_stream<T> io;
    history      h;

    char crlf[2] = {ascii::CR, ascii::LF};

    std::vector<completion_function> completer;
};

#endif
