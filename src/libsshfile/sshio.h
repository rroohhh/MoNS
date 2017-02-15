/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  sshio.h -- ssh io wrapper
*/

#ifndef _SSHIO_H
#define _SSHIO_H

#include <cstdio>

struct sshio {
	sshio(ssh_session session, ssh_channel chan, int width = 100,
		  int height = 100) noexcept
		: session(session), chan(chan), dims{width, height} {
		ssh_set_message_callback(session, message_callback, dims);
	}

	sshio(const sshio & other) noexcept
		: session(other.session), chan(other.chan),
		  dims{other.dims[0], other.dims[1]} {
		ssh_set_message_callback(session, message_callback, dims);
	}

	sshio & operator=(const sshio & other) noexcept {
		session = other.session;
		chan    = other.chan;
		dims[0] = other.dims[0];
		dims[1] = other.dims[1];
		ssh_set_message_callback(session, message_callback, dims);

		return *this;
	}

	int close() noexcept {
		ssh_disconnect(session);
		return 0;
	}

	int write(const char * buffer, const int size) const noexcept {
		return ssh_channel_write(chan, buffer, size);
	}

	int read(char * buffer, const int size) const noexcept {
		return ssh_channel_read(chan, buffer, size, 0);
	}

	int read_nonblocking(char * buffer, const int size) const noexcept {
		return ssh_channel_read_nonblocking(chan, buffer, size, 0);
	}

	int get_width() const noexcept { return dims[0]; }

	int get_height() const noexcept { return dims[1]; }

private:
	static int message_callback(ssh_session  /*session*/, ssh_message msg,
								void * data) {
		int * dims = (int *)data;
		if((ssh_message_subtype(msg) == SSH_CHANNEL_REQUEST_PTY) ||
		   (ssh_message_subtype(msg) == SSH_CHANNEL_REQUEST_WINDOW_CHANGE)) {
			dims[0] = ssh_message_channel_request_pty_width(msg);
			dims[1] = ssh_message_channel_request_pty_height(msg);
		}
		return 1;
	}
	ssh_session session;
	ssh_channel chan;
	int         dims[2];
};

#endif
