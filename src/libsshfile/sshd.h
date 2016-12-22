/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  sshd.h -- simple sshd implementation
*/

#ifndef _SSHD_H
#define _SSHD_H

#include "io/log.h"
#include "libssh/libssh.h"
#include "libssh/server.h"
#include "sshio.h"

class sshd {
public:
	sshd(std::string addr, unsigned int port) noexcept;
	sshio accept() noexcept;

private:
    ssh_bind sshbind;
    int authenticate(ssh_session session) noexcept;
};

#endif
