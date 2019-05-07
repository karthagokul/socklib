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


#include "socket.h"
#include <future>
#include <string.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* Here we need to differentiate between windows or posix */

class SocketInternal
{
public:
    SocketInternal();
    ~SocketInternal();
    bool init(const Type &aTye);
    bool bind();
    bool setHost(const Connection &aHost);
    bool isValid() const { return sock_fd != -1; }
    void setReadHandler(std::function<void(const std::string &adata)> callback);
    void setNewConnectionHandler(std::function<void(const Connection &)> callback);
    bool write(const std::string &aData);
    bool connect();
    void close();
    int sock_fd;
    sockaddr_in m_addr;
    Connection hostInfo;
    std::function<void(const std::string &adata)> readCallBack;
    std::function<void(const Connection &)> newConnectionCallBack;
    std::thread runner;
    int read_from_client (int filedes);
    void exec();
    bool listen();
};

void SocketInternal::setNewConnectionHandler(std::function<void (const Connection &)> callback)
{
  newConnectionCallBack=callback;
}

SocketInternal::SocketInternal()
  :sock_fd(-1)
{
  LOG("");
  memset ( &m_addr,0,sizeof ( m_addr ) );
}

bool SocketInternal::connect()
{
  m_addr.sin_addr.s_addr = inet_addr(hostInfo.ipAddress.c_str());
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons(hostInfo.port);
  LOG("Connecting to "<<hostInfo.ipAddress<<"And "<<hostInfo.port);
  //Connect to remote server
  if (::connect(sock_fd , (struct sockaddr *)&m_addr , sizeof(m_addr))< 0)
  {
    ERROR("Connect");
    return false;
  }
  return true;
}

bool SocketInternal::listen()
{
  runner=std::thread(&SocketInternal::exec,this);
  return true;
}


void  SocketInternal::exec()
{
  LOG("");
  fd_set active_fd_set, read_fd_set;
  struct sockaddr_in clientname;
  socklen_t size;
  if (::listen (sock_fd, 1) < 0)
  {
    ERROR ("listen");
    exit (EXIT_FAILURE);
  }

  /* Initialize the set of active sockets. */
  FD_ZERO (&active_fd_set);
  FD_SET (sock_fd, &active_fd_set);

  while (1)
  {
    /* Block until input arrives on one or more active sockets. */
    read_fd_set = active_fd_set;
    if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
    {
      ERROR ("select");
      exit (EXIT_FAILURE);
    }

    /* Service all the sockets with input pending. */
    for (int i = 0; i < FD_SETSIZE; ++i)
      if (FD_ISSET (i, &read_fd_set))
      {
        if (i == sock_fd) //Server Socket
        {
          /* Connection request on original socket. */
          int newsock;
          size = sizeof (clientname);
          newsock = ::accept (sock_fd,(struct sockaddr *) &clientname,&size);
          if (newsock < 0)
          {
            ERROR ("accept");
            exit (EXIT_FAILURE);
          }
          if(newConnectionCallBack)
          {
            Connection c;
            c.ipAddress=inet_ntoa (clientname.sin_addr);
            c.port= ntohs (clientname.sin_port);
            newConnectionCallBack(c);
          }
          FD_SET (newsock, &active_fd_set);
        }
        else
        {
          /* Data arriving on an already-connected socket. */
          if (read_from_client (i) < 0)
          {
            ::close (i);
            FD_CLR (i, &active_fd_set);
          }
        }
      }
  }
}

#define MAXMSG  1024 // This could be tuned for better performance to increase the read chunks
int SocketInternal::read_from_client (int filedes)
{
  LOG("");
  char buffer[MAXMSG];
  int nbytes;

  nbytes = read (filedes, buffer, MAXMSG);
  if (nbytes < 0)
  {
    /* Read error. */
    ERROR("read");
    exit (EXIT_FAILURE);
  }
  else if (nbytes == 0)
    /* End-of-file. */
    return -1;
  else
  {
    /* Data read. */
    LOG("Server: got message:"<<buffer);
    return 0;
  }
}


bool SocketInternal::init(const Type &aType)
{
  LOG("");
  bool ret=true;
  if(aType==Tcp)
  {
    sock_fd = socket ( AF_INET,SOCK_STREAM,0 );
  }
  else
  {
    sock_fd=socket(AF_INET, SOCK_DGRAM, 0);
  }
  if (!isValid() )
  {
    ERROR("Invalid Socket");
    return false;
  }

  // TIME_WAIT - argh
  int on = 1;
  if (setsockopt ( sock_fd, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
  {
    ERROR("Set Socket Options ");
    return false;
  }
  return ret;
}

bool SocketInternal::bind()
{
  LOG("");
  if(!isValid())return false;
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( hostInfo.port );
  LOG("Binding Port : "<<hostInfo.port);
  if(-1== ::bind(sock_fd,(struct sockaddr *)&m_addr,sizeof(m_addr)))
    return false;

  return true;
}

void SocketInternal::setReadHandler(std::function<void(const std::string &adata)> callback)
{
  LOG("");
  readCallBack=callback;
}

bool SocketInternal::write(const std::string &aData)
{
  LOG("");
  int status = ::send ( sock_fd, aData.c_str(), aData.size(), MSG_NOSIGNAL );
  if(status >0)
    return false;
  return true;
}

void SocketInternal::close()
{
  LOG("");
  if(isValid())
    ::close(sock_fd);
}


bool SocketInternal::setHost(const Connection &aHost)
{
  LOG("");
  hostInfo=aHost;
  return true;
}

SocketInternal:: ~SocketInternal()
{
  LOG("");
  close();
}

Socket::Socket()
  :d(new SocketInternal())
{

}

bool Socket::init(const Type &aType)
{
  return d->init(aType);
}

bool Socket::setHost(const Connection &aHost)
{
  return d->setHost(aHost);
}

bool Socket::bind()
{
  return d->bind();
}

bool Socket::isValid() const
{
  return d->isValid();
}
void Socket::setReadHandler(std::function<void(const std::string &adata)> callback)
{
  return d->setReadHandler(callback);
}

bool Socket::write(const std::string &aData)
{
  LOG("TODO");
}

bool Socket::listen()
{
  return d->listen();
}

void Socket::close()
{
  return d->close();
}
void Socket::setNewConnectionHandler(std::function<void(const Connection &)> callback)
{
  return d->setNewConnectionHandler(callback);
}
Socket::~Socket()
{

}
