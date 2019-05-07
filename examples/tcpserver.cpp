/*****************************************************************************
 *
 *
 * Created: 04-05-2019 2019 by gokul
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


#include <iostream>
#include <unistd.h>
#include "tcpserversocket.h"
#include <functional>

void onnewconnection(const Connection &c)
{
  std::cout<<c.ipAddress<<"oye, new connection";
}

int main()
{
  TcpServerSocket serverSocket;
  using namespace std::placeholders; // for `_1`
  serverSocket.setNewConnectionHandler(std::bind(&onnewconnection,_1));
  //class memober var
   //serverSocket.setNewConnectionHandler(std::bind(&onnewconnection,this,_1));
  Connection c;
  c.ipAddress="127.0.0.1";
  c.port=5050;
  if(serverSocket.init(c))
  {
    if(serverSocket.start())
    {
      std::cout<<"Server is Running "<<std::endl;
    }
  }

  sleep(2);


  while(1)
  {

  }

  return 0;
}

