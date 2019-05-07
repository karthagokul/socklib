/*****************************************************************************
 *
 *
 * Created: 06-05-2019 2019 by gokul
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


#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <functional>

class SocketInternal;

#define LOG(msg) std::cout << __PRETTY_FUNCTION__<< " " << msg << std::endl ;
#define ERROR(msg)std::cerr <<"[ERROR ] " <<__FILE__<<" Line: "<<__LINE__ << " "<< msg << std::endl ;

/**
 * @brief The Connection struct
 */
struct Connection
{
    std::string ipAddress;
    uint port;
};

enum Type
{
  Tcp,
  Udp
};

class Socket
{
  protected:
    Socket();
    bool init(const Type &aType);
    bool setHost(const Connection &aHost);
    bool bind();
    bool connect();
    bool listen();
    ~Socket();

  public:
    bool isValid() const;
    void setNewConnectionHandler(std::function<void(const Connection &)> callback);
    void setReadHandler(std::function<void(const std::string &adata)> callback);
    bool write(const std::string &aData);
    void close();

  protected:
    SocketInternal *d;
};

#endif // SOCKET_H
