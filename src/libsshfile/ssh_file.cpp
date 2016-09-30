/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  ssh_file.cpp -- c file over ssh
*/

#include "ssh_file.h"

ssh_file::ssh_file(ssh_channel * chan) noexcept {
    pipe(client_input);
    pipe(client_output);
    infile_   = fdopen(client_input[0], "r");
    outfile_  = fdopen(client_output[1], "w");
    int flags = fcntl(client_output[0], F_GETFL, 0);
    fcntl(client_output[0], F_SETFL, flags | O_NONBLOCK);

    if(!Platform::fork()) {
        close(client_input[0]);
        close(client_output[1]);
        char        buf[2048];
        int         read;
        std::string s;
        std::regex  ln("\n");

        while(ssh_channel_is_open(*chan)) {
            read = ssh_channel_read_timeout(*chan, buf, 2048, 0, 10);
            if(read) { write(client_input[1], buf, read); }

            read = ::read(client_output[0], buf, 2048);
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
        close(client_input[1]);
        close(client_output[0]);
    }
}

void ssh_file::disconnect() noexcept {
    close(client_output[1]);
    ssh_finalize();
}

FILE * ssh_file::outfile() noexcept { return outfile_; }
FILE * ssh_file::infile() noexcept { return infile_; }
