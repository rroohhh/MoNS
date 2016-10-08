/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  ssh_file.cpp -- c file over ssh
*/

#include "ssh_file.h"

ssh_file::ssh_file(ssh_channel * chan) noexcept {
    // pipe(client_input);
    // pipe(client_output);
    // infile_   = fdopen(client_input[0], "r");
    // outfile_  = fdopen(client_output[1], "w");
    // int flags = fcntl(client_output[0], F_GETFL, 0);
    // fcntl(client_output[0], F_SETFL, flags | O_NONBLOCK);

    int master;

    struct winsize win;
    win.ws_col    = 80;
    win.ws_row    = 24;
    win.ws_xpixel = 480;
    win.ws_ypixel = 192;

	// struct termios termios;
	// tcgetattr(0, &termios);
	// termios.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);

    if(!::forkpty(&master, nullptr, nullptr, &win)) {
		int flags = fcntl(0, F_GETFL, 0);
		fcntl(0, F_SETFL, flags | FNDELAY);
		flags = fcntl(1, F_GETFL, 0);
		fcntl(1, F_SETFL, flags | FNDELAY);
        // close(client_input[0]);
        // close(client_output[1]);
        char        buf[2048];
        int         read;
        std::string s;
        std::regex  ln("\n");

        // printf("client_input[0] = %d\n", client_input[0]);
        // printf("client_input[1] = %d\n", client_input[1]);
        // printf("client_output[0] = %d\n", client_output[0]);
        // printf("client_output[1] = %d\n", client_output[1]);

        while(ssh_channel_is_open(*chan)) {
            read = ssh_channel_read_timeout(*chan, buf, 2048, 0, 10);
            if(read) { int write = ::write(1, buf, read); }

            read = ::read(0, buf, 128);
            if(read == 0) {
                ssh_disconnect(ssh_channel_get_session(*chan));
                exit(EXIT_SUCCESS);
            } else if(read > 0) {
                s.assign(buf, read);
                s = std::regex_replace(s, ln, "\r\n");

                ssh_channel_write(*chan, s.c_str(), s.size());
            }
        }

        exit(EXIT_SUCCESS);
    } else {
		int flags = fcntl(master, F_GETFL, 0);
		fcntl(master, F_SETFL, flags | FNDELAY);
		// fcntl(STDIN_FILENO, F_SETFL, FNDELAY);


        infile_  = fdopen(master, "r");
		// outfile_ = infile_;
        outfile_ = fdopen(master, "w");
        // close(client_input[1]);
        // close(client_output[0]);
    }
}

void ssh_file::disconnect() noexcept {
    close(client_output[1]);
    ssh_finalize();
}

FILE * ssh_file::outfile() noexcept { return outfile_; }
FILE * ssh_file::infile() noexcept { return infile_; }
