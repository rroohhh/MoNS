/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  sshd.cpp -- simple sshd implementation
*/

#define KEYS_FOLDER "/etc/ssh/"

#include "sshd.h"

namespace mons {
    sshd::sshd(string addr, unsigned int port) noexcept {
        sshbind = ssh_bind_new();

        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, addr.c_str());
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &port);
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY,
                             KEYS_FOLDER "ssh_host_rsa_key");

        // TODO(robin): Why is this needed here?
        FILE * logfile = fopen("mons.log", "aw");
        log::log_outputs.erase("stdout");
        log::log_outputs["logfile"] = logfile;

        if(ssh_bind_listen(sshbind) < 0) {
            log::err("Error listening to socket: {}\n", ssh_get_error(sshbind));
            exit(EXIT_FAILURE);
        }
    }

    sshio sshd::accept() noexcept {
        ssh_session session;
        ssh_message message;
        ssh_channel chan  = nullptr;
        int         auth  = 0;
        int         shell = 0;
        int         r;
        int         width, height;

        session = ssh_new();
        r       = ssh_bind_accept(sshbind, session);

        if(r == SSH_ERROR) {
            log::err("Error accepting a connection: {}\n",
                     ssh_get_error(sshbind));
            return {nullptr, nullptr};
        }

        if(ssh_handle_key_exchange(session) != 0) {
            log::err("ssh_handle_key_exchange: {}\n", ssh_get_error(session));
            return {nullptr, nullptr};
        }

        /* proceed to authentication */
        auth = authenticate(session);
        if(auth == 0) {
            log::err("Authentication error: {}\n", ssh_get_error(session));
            ssh_disconnect(session);
            return {nullptr, nullptr};
        }

        /* wait for a channel session */
        do {
            message = ssh_message_get(session);
            if(message != nullptr) {
                if(ssh_message_type(message) == SSH_REQUEST_CHANNEL_OPEN &&
                   (ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_SHELL ||
                    ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_PTY)) {
                    chan =
                        ssh_message_channel_request_open_reply_accept(message);
                    ssh_message_free(message);
                    break;
                } else {
                    log::warn("message unhandled {}\n",
                              ssh_message_type(message));
                    ssh_message_reply_default(message);
                    ssh_message_free(message);
                }
            } else {
                if(chan == nullptr) {
                    log::err("Error: client did not ask for a channel session "
                             "({})\n",
                             ssh_get_error(session));
                    ssh_finalize();
                    return {nullptr, nullptr};
                }
            }
        } while(chan == nullptr);

        do {
            message = ssh_message_get(session);
            if(message != nullptr) {
                if(ssh_message_type(message) == SSH_REQUEST_CHANNEL &&
                   (ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_SHELL ||
                    ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_PTY)) {
                    width  = ssh_message_channel_request_pty_width(message);
                    height = ssh_message_channel_request_pty_height(message);
                    shell  = 1;
                    ssh_message_channel_request_reply_success(message);
                    ssh_message_free(message);
                    break;
                }
                ssh_message_reply_default(message);
                ssh_message_free(message);
            } else {
                if(shell == 0) {
                    log::err("Error: No shell requested ({})\n",
                             ssh_get_error(session));
                    return {nullptr, nullptr};
                }
            }
        } while(!shell);

        return {session, chan, width, height};
    }

    int sshd::authenticate(ssh_session session) noexcept {
        ssh_message message;

        do {
            message = ssh_message_get(session);
            if(message == nullptr) { break; }
            switch(ssh_message_type(message)) {
            case SSH_REQUEST_AUTH:
                switch(ssh_message_subtype(message)) {
                case SSH_AUTH_METHOD_NONE:
                    ssh_message_auth_reply_success(message, 0);
                    ssh_message_free(message);
                    return 1;
                default:
                    log::warn("User {} wants to auth with unknown auth {}\n",
                              ssh_message_auth_user(message),
                              ssh_message_subtype(message));
                    ssh_message_auth_set_methods(message, SSH_AUTH_METHOD_NONE);
                    ssh_message_reply_default(message);
                    break;
                }
                break;
            default:
                log::warn("message unhandled {}\n", ssh_message_type(message));
                ssh_message_auth_set_methods(message, SSH_AUTH_METHOD_NONE);
                ssh_message_reply_default(message);
            }
            ssh_message_free(message);
        } while(true);
        return 0;
    }
}
