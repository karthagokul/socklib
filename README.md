## socklib

The socklib is a c++ socket library provides a high level abstraction on the platform specific api (TODO)

### installation

```cmake ../ && make

### Usage

```markdown
TCPServer socket;
Connection c;
c.ipAddress="127.0.0.1"
c.port=2020
socket.sethost(c);
socket.start(); //to run a socket server

