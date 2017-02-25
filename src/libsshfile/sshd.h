/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  sshd.h -- simple sshd implementation
*/

#ifndef __SSHD_H_
#define __SSHD_H_

#include "libssh/libssh.h"
#include "libssh/server.h"

#include "io.h"
#include "sshio.h"

namespace mons {
    class sshd {
    public:
        sshd(string addr, unsigned int port) noexcept;
        sshio accept() noexcept;

    private:
        ssh_bind sshbind;
        int authenticate(ssh_session session) noexcept;
    };
}

#endif
