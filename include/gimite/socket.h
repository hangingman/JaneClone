//---------------------------------------------------------------------------
/*
                 gimite/socket Ver.1.5  by Gimite Ichikawa
  
  This is a socket library which supports TCP/IP and UDP/IP.
  It supports Windows (using WinSock2) and UNIX (using POSIX Socket).
  
  See http://gimite.net/gimite/cppmess/socket.html (Japanese) for more
  information.
  
  This software is public domain.
  
  Gimite Ichikawa http://gimite.net/
  
*/
//---------------------------------------------------------------------------
#ifndef GIMITE_socketH
#define GIMITE_socketH
//---------------------------------------------------------------------------
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define GIMITE_WIN32
#endif
//---------------------------------------------------------------------------
#include <iostream>
#include <ios>//streamsize
#include <streambuf>//basic_streambuf
#include <string>//char_traits, basic_string
#include <vector>//vector
#ifdef GIMITE_WIN32
#  include <WinSock2.h>
#  pragma comment(lib,"ws2_32.lib")
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <unistd.h>
#endif
//---------------------------------------------------------------------------
namespace gimite{
//---------------------------------------------------------------------------
#ifdef GIMITE_WIN32
  typedef SOCKET socket_t;
  typedef int sock_result_t;
  typedef int sock_len_t;
  typedef unsigned long sock_uint32_t;
#else
  typedef int socket_t;
  typedef ssize_t sock_result_t;
  typedef socklen_t sock_len_t;
  typedef u_int32_t sock_uint32_t;
#endif
//---------------------------------------------------------------------------
// Initializes socket system.
inline bool startup_socket(){
#ifdef GIMITE_WIN32
  WSADATA wsaData;
  return WSAStartup(0x0002, &wsaData)==0;
#else
  return true;
#endif
}

//---------------------------------------------------------------------------
// Cleans up socket system.
inline void cleanup_socket(){
#ifdef GIMITE_WIN32
  WSACleanup();
#endif
}

//---------------------------------------------------------------------------
// IP address class.
class ip_address
{
public:
  explicit ip_address(sock_uint32_t addr= INADDR_NONE){
#ifdef GIMITE_WIN32
    addr_.S_un.S_addr= addr;
#else
    addr_.s_addr= addr;
#endif
  }
  explicit ip_address(const in_addr& addr): addr_(addr){}
  ip_address(const std::string& host){
    solve_address(host.c_str());
  }
  ip_address(const char* host){
    solve_address(host);
  }
  ip_address& operator=(const std::string& host){
    solve_address(host.c_str());
    return *this;
  }
  ip_address& operator=(const char* host){
    solve_address(host);
    return *this;
  }
  // in_addr structure which represents IP address.
  in_addr addr()const{
    return addr_;
  }
  // 32bit integer which represents IP address.
  sock_uint32_t as_int()const{
#ifdef GIMITE_WIN32
    return addr_.S_un.S_addr;
#else
    return addr_.s_addr;
#endif
  }

private:
  in_addr addr_;
  
  // Gets IP address from host name or string representation of IP address.
  void solve_address(const char* host){
    sock_uint32_t uaddr= ::inet_addr(host);
    if (uaddr==INADDR_NONE){
      hostent* ent= ::gethostbyname(host);
      if (ent && ent->h_addr_list[0])
        uaddr= *(sock_uint32_t*)ent->h_addr_list[0];
    }
#ifdef GIMITE_WIN32
    addr_.S_un.S_addr= uaddr;
#else
    addr_.s_addr= uaddr;
#endif
  }
  
  // Comparisons and equality.
  friend bool operator<(const ip_address& lhs, const ip_address& rhs){
    return lhs.as_int()<rhs.as_int();
  }
  friend bool operator>(const ip_address& lhs, const ip_address& rhs){ return rhs<lhs; }
  friend bool operator<=(const ip_address& lhs, const ip_address& rhs){ return !(lhs>rhs); }
  friend bool operator>=(const ip_address& lhs, const ip_address& rhs){ return !(lhs<rhs); }
  friend bool operator==(const ip_address& lhs, const ip_address& rhs){ return lhs>=rhs && lhs<=rhs; }
  friend bool operator!=(const ip_address& lhs, const ip_address& rhs){ return !(lhs==rhs); }
  
  // Outputs to stream.
  friend std::ostream& operator<<(std::ostream& os, const ip_address& addr){
    sock_uint32_t uaddr= addr.as_int();
    for (int i= 0; i<4; ++i){
      if (i>0) os << ".";
      os << (int)(unsigned char)(uaddr >> i*8);
    }
    return os;
  }
  
};

//---------------------------------------------------------------------------
// Structure which represents IP address and port number.
struct socket_address
{
  explicit socket_address(const ip_address& i= ip_address(), int p= 0)
    : ip(i), port(p){}
  
  // IP address.
  ip_address ip;
  // Port number.
  int port;
  
  // Comparisons and equality.
  friend bool operator<(const socket_address& lhs, const socket_address& rhs){
    if (lhs.ip<rhs.ip) return true;
    if (lhs.ip>rhs.ip) return false;
    return lhs.port<rhs.port;
  }
  friend bool operator>(const socket_address& lhs, const socket_address& rhs){ return rhs<lhs; }
  friend bool operator<=(const socket_address& lhs, const socket_address& rhs){ return !(lhs>rhs); }
  friend bool operator>=(const socket_address& lhs, const socket_address& rhs){ return !(lhs<rhs); }
  friend bool operator==(const socket_address& lhs, const socket_address& rhs){
    return lhs>=rhs && lhs<=rhs;
  }
  friend bool operator!=(const socket_address& lhs, const socket_address& rhs){ return !(lhs==rhs); }
  
  // Outputs to stream.
  friend std::ostream& operator<<(std::ostream& os, const socket_address& saddr){
    os << saddr.ip << ":" << saddr.port;
    return os;
  }
  
};

//---------------------------------------------------------------------------
// streambuf class for stream socket.
class socket_streambuf: public std::streambuf
{
  typedef std::streambuf::int_type int_type;
  typedef std::streambuf::traits_type traits_type;
  
public:
  explicit socket_streambuf(socket_t sock= -1, std::size_t buf_size= 1024)
      : sock_(sock), buffer_(buf_size){}
  socket_streambuf(const ip_address& host, int port, 
        std::size_t buf_size= 1024)
      : sock_(-1), buffer_(buf_size){
    open(host, port);
  }
  virtual ~socket_streambuf(){
    close();
  }
  // Connection.
  bool open(const ip_address& host, int port){
    if (is_open()) close();
    if (host.as_int()==INADDR_NONE) return false;
    sock_= ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock_==-1) return false;
    sockaddr_in addr;
    addr.sin_family= AF_INET;
    addr.sin_port= htons((unsigned short)port);
    addr.sin_addr= host.addr();
    if (::connect(sock_, (const sockaddr*)&addr, sizeof(sockaddr_in))==-1)
      close();
    return is_open();
  }
  // Disconnection.
  void close(){
    if (sock_==-1) return;
#ifdef GIMITE_WIN32
    ::closesocket(sock_);
#else
    ::close(sock_);
#endif
    sock_= -1;
  }
  // Releases socket handle without closing.
  socket_t release(){
    socket_t sock= sock_;
    sock_= -1;
    return sock;
  }
  // Sending.
  int send(const void* buffer, int size){
    return ::send(sock_, (const char*)buffer, size, 0);
  }
  // Receiving.
  int recv(void* buffer, int size){
    return ::recv(sock_, (char*)buffer, size, 0);
  }
  // Connected correctly?
  bool is_open()const{ return sock_!=-1; }
  // Socket handle.
  socket_t socket()const{ return sock_; }

protected:
  // Sends one character.
  virtual int_type overflow(int_type c= std::char_traits<char>::eof()){
    if (!traits_type::eq_int_type(c, traits_type::eof())){
      char cc= char(c);
      if (traits_type::eq_int_type(xsputn(&cc, 1), 0))
        return traits_type::eof();
    }
    return 'a';
  }
  // Sends n characters.
  virtual std::streamsize xsputn(const char_type* s, std::streamsize n){
    if (!is_open()) return 0;
    sock_result_t res= ::send(sock_, s, int(n), 0);
    return res==-1? 0 : res;
  }
  // Called when buffer for receiving is empty.
  virtual int_type underflow(){
    if (!is_open()) return traits_type::eof();
    if (!gptr() || gptr()>=egptr()){
      if (gptr() && gptr()>eback())
        buffer_[0]= *(gptr()-1);
      else
        buffer_[0]= '\0';
      sock_result_t size= ::recv(sock_, &buffer_[1], int(buffer_.size()-1), 0);
      if (size<=0)
        return traits_type::eof();
      else
        setg(&buffer_[0], &buffer_[1], &buffer_[size+1]);
    }
    return traits_type::to_int_type(*gptr());
  }

private:
  socket_t sock_;
  std::vector<char> buffer_;

};

//---------------------------------------------------------------------------
// stream class for stream socket.
class socket_stream: public std::iostream
{
public:
  // Constructor which doesn't connect now.
  socket_stream(): std::iostream(0){
    rdbuf(&sbuf_);
    setstate(std::ios::badbit);
  }
  // Constructor which connects now.
  socket_stream(const ip_address& host, int port)
      : std::iostream(0), sbuf_(host, port){
    rdbuf(&sbuf_);
    if (!sbuf_.is_open()) setstate(std::ios::badbit);
  }
  // Construct which accepts existing socket handle.
  explicit socket_stream(socket_t sock)
      : std::iostream(0), sbuf_(sock){
    rdbuf(&sbuf_);
    if (!sbuf_.is_open()) setstate(std::ios::badbit);
  }
  virtual ~socket_stream(){ rdbuf(0); }
  // Connection.
  void open(const ip_address& host, int port){
    clear();
    sbuf_.open(host, port);
    if (!sbuf_.is_open()) setstate(std::ios::badbit);
  }
  // Disconnection.
  void close(){
    setstate(std::ios::badbit);
    sbuf_.close();
  }
  // Releases socket handle without closing.
  socket_t release(){
    setstate(std::ios::badbit);
    return sbuf_.release();
  }
  // Sending.
  int send(const void* buffer, int size){ return sbuf_.send(buffer, size); }
  // Receiving.
  int recv(void* buffer, int size){ return sbuf_.recv(buffer, size); }
  // Socket handle.
  socket_t socket()const{ return sbuf_.socket(); }
  
private:
  socket_streambuf sbuf_;

};

//---------------------------------------------------------------------------
// Socket which needs binding.
class bound_socket
{
public:
  // Normal constructor.
  explicit bound_socket(int type)
    : type_(type), sock_(-1), is_bound_(false){}
  // Constructor which accepts existing socket handle.
  bound_socket(int type, socket_t sock, bool is_bound)
    : type_(type), sock_(sock), is_bound_(is_bound){}
  virtual ~bound_socket(){
    close();
  }
  // Closes socket.
  void close(){
    is_bound_= false;
    if (sock_==-1) return;
#ifdef GIMITE_WIN32
    ::closesocket(sock_);
#else
    ::close(sock_);
#endif
    sock_= -1;
  }
  // Releases socket handle without closing.
  socket_t release(){
    socket_t sock= sock_;
    sock_= -1;
    is_bound_= false;
    return sock;
  }
  // Specifies existing socket.
  void socket(socket_t sock, bool is_bound){
    close();
    sock_= sock;
    is_bound_= is_bound;
  }
  // Type of socket.
  int type()const{ return type_; }
  // Socket handle.
  socket_t socket()const{ return sock_; }
  // Correctly bound?
  operator const void*()const{ return is_bound_? this : NULL; }
  bool operator!()const{ return !is_bound_; }

protected:
  // Binding.
  bool pbind(int port){
    if (is_bound_) return false;
    if (sock_==-1){
      sock_= ::socket(AF_INET, type_, 0);
      if (sock_==-1) return false;
    }
    sockaddr_in addr;
    addr.sin_family= AF_INET;
    addr.sin_port= htons(port);
    addr.sin_addr= ip_address(sock_uint32_t(INADDR_ANY)).addr();
      // NOTE: Type of INADDR_ANY is not sock_uint32_t in Cygwin GCC 3.3.1,
      //       so cast is required here to avoid disambiguity.
    if (::bind(sock_, (const sockaddr*)&addr, sizeof(addr))==-1)
      return false;
    is_bound_= true;
    return true;
  }
  
private:
  const int type_;
  socket_t sock_;
  bool is_bound_;
  
};

//---------------------------------------------------------------------------
// Server side stream socket.
class server_stream_socket: public bound_socket
{
public:
  // Constructor which doesn't bind now.
  server_stream_socket()
    : bound_socket(SOCK_STREAM){}
  // Constructor which binds now.
  explicit server_stream_socket(int port, int backlog= 5)
      : bound_socket(SOCK_STREAM){
    bind(port, backlog);  
  }
  // Constructor which accepts existing socket handle.
  server_stream_socket(socket_t sock, bool is_bound)
    : bound_socket(SOCK_STREAM, sock, is_bound){}
  // Binding.
  bool bind(int port, int backlog= 5){
    if (!pbind(port)) return false;
    if (::listen(socket(), backlog)!=-1) return true;
    close();
    return false;
  }
  // Accepting connection request.
  socket_t accept(){
    return ::accept(socket(), 0, 0);
  }
  
};

//---------------------------------------------------------------------------
// Datagram socket.
class datagram_socket: public bound_socket
{
public:
  // Constructor which doesn't bind now.
  datagram_socket()
    : bound_socket(SOCK_DGRAM){}
  // Constructor which binds now.
  explicit datagram_socket(int port)
      : bound_socket(SOCK_DGRAM){
    bind(port);
  }
  // Constructor which accepts existing socket handle.
  datagram_socket(socket_t sock, bool is_bound)
    : bound_socket(SOCK_DGRAM, sock, is_bound){}
  // Binding.
  bool bind(int port){
    return pbind(port);
  }
  // Receives data.
  int recvfrom(void* buffer, int size, socket_address* addr= 0,
      int flags= 0){
    sockaddr_in saddr;
    sock_len_t addr_len= sizeof(saddr);
    int result= ::recvfrom(socket(), (char*)buffer, size, flags,
      (sockaddr*)&saddr, &addr_len);
      // NOTE: Cast is required here because second parameter of Winsock recvfrom is char*.
    if (addr){
      addr->ip= ip_address(saddr.sin_addr);
      addr->port= ntohs(saddr.sin_port);
    }
    return result;
  }
  // Sends data.
  int sendto(const void* buffer, int size, const socket_address* addr,
      int flags= 0){
    sockaddr_in saddr;
    saddr.sin_family= AF_INET;
    saddr.sin_addr= addr->ip.addr();
    saddr.sin_port= htons(addr->port);
    return ::sendto(socket(), (const char*)buffer, size, flags,
      (const sockaddr*)&saddr, sizeof(saddr));
      // NOTE: Cast is required here because second parameter of Winsock sendto is char*.
  }
  
};

// Name used before Ver.1.4. For compatibility.
typedef datagram_socket diagram_socket;

//---------------------------------------------------------------------------
}//namespace gimite
//---------------------------------------------------------------------------
#endif
