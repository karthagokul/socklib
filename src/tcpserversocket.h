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


#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#include "socket.h"

class TcpServerSocket:public Socket
{
  public:
    TcpServerSocket();
    ~TcpServerSocket();
    /**
     * @brief init
     * @param aHost
     * @return
     */
    bool init(const Connection &aHost);
    /**
     * @brief start
     * @return
     */
    bool start();
    /**
     * @brief stop
     * @return
     */
    bool stop();
};

#endif // TCPSOCKETSERVER_H
