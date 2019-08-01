/* minimal winsock2.h which is a mix of the codes from
     https://groups.google.com/forum/#!msg/comp.lang.c/Kfncg_OpkaA/WUNQskElBwAJ
	 and
	 http://www.jbox.dk/sanos/source/include/sys/select.h.html
   a work of kenlo88@gmail.com
*/

#ifndef _WINSOCK2_H 
#define _WINSOCK2_H 

#include <_mingw.h> 
#include <windows.h> 

#ifndef FD_SETSIZE 
#define FD_SETSIZE 64 
#endif 

typedef unsigned int SOCKET; 

typedef struct fd_set { 
  unsigned int   fd_count; 
  SOCKET  fd_array[FD_SETSIZE]; 
} fd_set; 

#define _SS_MAXSIZE 128
#define _SS_ALIGNSIZE (8)

#define _SS_PAD1SIZE (_SS_ALIGNSIZE - sizeof (short))
#define _SS_PAD2SIZE (_SS_MAXSIZE - (sizeof (short) + _SS_PAD1SIZE + _SS_ALIGNSIZE))

typedef struct sockaddr_storage {
  short   ss_family;
  char    __ss_pad1[_SS_PAD1SIZE];
  __int64 __ss_align;
  char    __ss_pad2[_SS_PAD2SIZE];
} SOCKADDR_STORAGE, *PSOCKADDR_STORAGE;

extern int __stdcall __WSAFDIsSet(SOCKET,fd_set *); 

#define FD_CLR(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count ; __i++) { \
        if (((fd_set FAR *)(set))->fd_array[__i] == fd) { \
            while (__i < ((fd_set FAR *)(set))->fd_count-1) { \
                ((fd_set FAR *)(set))->fd_array[__i] = \
                    ((fd_set FAR *)(set))->fd_array[__i+1]; \
                __i++; \
            } \
            ((fd_set FAR *)(set))->fd_count--; \
            break; \
        } \
    } \
} while(0)

#define FD_SET(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count; __i++) { \
        if (((fd_set FAR *)(set))->fd_array[__i] == (fd)) { \
            break; \
        } \
    } \
    if (__i == ((fd_set FAR *)(set))->fd_count) { \
        if (((fd_set FAR *)(set))->fd_count < FD_SETSIZE) { \
            ((fd_set FAR *)(set))->fd_array[__i] = (fd); \
            ((fd_set FAR *)(set))->fd_count++; \
        } \
    } \
} while(0)

#ifndef FD_ZERO 
#define FD_ZERO(set) (((fd_set *)(set))->fd_count=0) 
#endif 

#ifndef FD_ISSET 
#define FD_ISSET(fd, set) __WSAFDIsSet((SOCKET)(fd), (fd_set *)(set)) 
#endif 

#ifndef _TIMEVAL_DEFINED /* also in sys/time.h */ 
#define _TIMEVAL_DEFINED 
struct timeval { 
    long tv_sec; 
    long tv_usec; 
}; 
#endif 

struct in_addr { 
    union { 
        struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b; 
        struct { unsigned short s_w1,s_w2; } S_un_w; 
        unsigned long S_addr; 
    } S_un; 
}; 
#define s_addr  S_un.S_addr 

struct sockaddr_in { 
    short sin_family; 
    unsigned short sin_port; 
    struct in_addr sin_addr; 
    char sin_zero[8]; 
}; 

struct sockaddr { 
    unsigned short sa_family; 
    char sa_data[14]; 
}; 

struct hostent { 
    char *h_name; 
    char **h_aliases; 
    short h_addrtype; 
    short h_length; 
    char **h_addr_list; 
}; 
#define h_addr h_addr_list[0] 

#define WSADESCRIPTION_LEN  256 
#define WSASYS_STATUS_LEN   128 
typedef struct WSAData { 
    WORD wVersion; 
    WORD wHighVersion; 
    char szDescription[WSADESCRIPTION_LEN+1]; 
    char szSystemStatus[WSASYS_STATUS_LEN+1]; 
    unsigned short iMaxSockets; 
    unsigned short iMaxUdpDg; 
    char *lpVendorInfo; 
} WSADATA; 
typedef WSADATA *LPWSADATA; 

#define INVALID_SOCKET (SOCKET)(~0) 

#define SOCK_STREAM  1 
#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_RAW 3
#define SOCK_RDM 4
#define SOCK_SEQPACKET 5

#define SO_DEBUG 0x0001
#define SO_ACCEPTCONN 0x0002
#define SO_REUSEADDR 0x0004
#define SO_KEEPALIVE 0x0008
#define SO_DONTROUTE 0x0010
#define SO_BROADCAST 0x0020
#define SO_USELOOPBACK 0x0040
#define SO_LINGER 0x0080
#define SO_OOBINLINE 0x0100

#define SO_SNDBUF 0x1001
#define SO_RCVBUF 0x1002

#define AF_INET 2 
#define MSG_PEEK 0x2 
#define INADDR_ANY (unsigned long)0x00000000 
#define INADDR_LOOPBACK 0x7f000001 
#define SOL_SOCKET 0xffff 
#define AF_INET6 23
#define FIONBIO _IOW('f',126,u_long)
#define PF_INET AF_INET
#define SO_ERROR 0x1007
#define SO_EXCLUSIVEADDRUSE ((int)(~SO_REUSEADDR))
#define SO_LINGER 0x0080
#define SO_KEEPALIVE 0x0008
#define IPPROTO_TCP 6
#define IPPROTO_IPV6 41
#define TCP_NODELAY 0x0001

#define SD_RECEIVE  0x00 
#define SD_SEND     0x01 
#define SD_BOTH     0x02 

#define h_errno WSAGetLastError() 

#define WSABASEERR 10000 
#define TRY_AGAIN (WSABASEERR+1002) 

typedef unsigned short u_short;
struct linger {
        u_short l_onoff;
        u_short l_linger;
};

SOCKET __stdcall accept(SOCKET, struct sockaddr *, int *); 
int __stdcall bind(SOCKET, const struct sockaddr *, int); 
int __stdcall closesocket(SOCKET); 
int __stdcall connect(SOCKET, const struct sockaddr *, int); 
struct hostent *__stdcall gethostbyname(const char *); 
int __stdcall gethostname(char *, int); 
int __stdcall getpeername(SOCKET, struct sockaddr *, int *); 
int __stdcall getsockname(SOCKET, struct sockaddr *, int *); 
unsigned long __stdcall htonl(unsigned long); 
unsigned short __stdcall htons(unsigned short); 
int __stdcall listen(SOCKET, int); 
unsigned long __stdcall ntohl(unsigned long); 
unsigned short __stdcall ntohs(unsigned short); 
int __stdcall recv(SOCKET, char *, int, int); 
int __stdcall recvfrom(SOCKET, char *, int, int, struct sockaddr *, int *); 
int __stdcall select(int nfds, fd_set *, fd_set *, fd_set *, const struct timeval *); 
int __stdcall send(SOCKET, const char *, int, int); 
int __stdcall sendto(SOCKET, const char *, int, int, const struct sockaddr *, int); 
int __stdcall setsockopt(SOCKET, int, int, const char *, int); 
int __stdcall shutdown(SOCKET, int); 
int __stdcall WSACleanup(void); 
int __stdcall WSAGetLastError(void); 
int __stdcall WSAStartup(WORD, LPWSADATA); 
int __stdcall getsockopt(SOCKET s,int level,int optname,char *optval,int *optlen);

#endif 
