/*****************************************************************************
 *
 *
 * Created: 07-05-2019 2019 by gokul
 *
 * Copyright 2019 gokul. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/


#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include "socket.h"

class TcpClientSocket:public Socket
{
  public:
    TcpClientSocket();
    bool connect(const Connection &host);
    bool disconnect();
    ~TcpClientSocket();
};

#endif // TCPCLIENTSOCKET_H
