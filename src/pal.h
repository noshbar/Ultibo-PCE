#pragma once

#include "/usr/include/newlib/sys/types.h"
#include "/usr/include/newlib/sys/utime.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "/usr/include/newlib/sys/timespec.h"

// **************************************************************** SOCKETS

#if 0
#define	SOCK_STREAM	1		/* stream socket */
#define	SOCK_DGRAM	2		/* datagram socket */
#define	SOCK_RAW	3		/* raw-protocol interface */
#define	SOMAXCONN	128
#define	AF_INET		2		/* internetwork: UDP, TCP, etc. */

typedef long SOCKET;
typedef size_t socklen_t;

struct hostent
{
	char  *h_name;
	char **h_aliases;
	int    h_addrtype; 
	int    h_length;
	char **h_addr_list;
	#define h_addr h_addr_list[0] /* Address, for backward compatibility.*/
};

struct in_addr {
  union {
    struct {
      unsigned char s_b1;
      unsigned char s_b2;
      unsigned char s_b3;
      unsigned char s_b4;
    } S_un_b;
    struct {
      unsigned short s_w1;
      unsigned short s_w2;
    } S_un_w;
    unsigned long S_addr;
  } S_un;
};

struct sockaddr {
        unsigned short  sa_family;
        char    sa_data[14];
};

struct sockaddr_in{  
    short sin_family;  
    unsigned short sin_port;  
    struct in_addr sin_addr;  
    char sin_zero[8];  
};  

int closesocket(int);
int	accept(int, struct sockaddr *, socklen_t *);
int	bind(int, const struct sockaddr *, socklen_t);
int	connect(int, const struct sockaddr *, socklen_t);
int	getpeername(int, struct sockaddr *, socklen_t *);
int	getsockname(int, struct sockaddr *, socklen_t *);
int	getsockopt(int, int, int, void *, socklen_t *);
int	listen(int, int);
ssize_t	recv(int, void *, size_t, int);
ssize_t	recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
ssize_t	send(int, const void *, size_t, int);
ssize_t	sendto(int, const void *,
	    size_t, int, const struct sockaddr *, socklen_t);
int	setsockopt(int, int, int, const void *, socklen_t);
int	shutdown(int, int);
int	sockatmark(int);
int	socket(int, int, int);
int	socketpair(int, int, int, int *);
uint16_t htons(uint16_t hostshort);
struct hostent *gethostbyname(const char *name);

#endif

// ************************************************************* DIRENT

typedef void DIR;

struct dirent
{
	char *d_name;
};

int            closedir(DIR *);
DIR           *opendir(const char *);
struct dirent *readdir(DIR *);
int            readdir_r(DIR *, struct dirent *, struct dirent **);
void           rewinddir(DIR *);
void           seekdir(DIR *, long int);
long int       telldir(DIR *);

// ************************************************************** TIME

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

typedef unsigned int Uint32;

int utime(const char *filename, const struct utimbuf *times);

int nanosleep(const struct timespec *req, struct timespec *rem);
char* strdup(const char *str1);
int strcasecmp(const char *s1, const char *s2);

// *************************************************************** PAL

#define SDL_LIL_ENDIAN  1234
#define SDL_BIG_ENDIAN  4321
#define SDL_BYTEORDER   SDL_LIL_ENDIAN

//typedef void SDL_mutex;
//typedef void SDL_cond;

int Pal_getScreenPitch();
void* Pal_getScreenBuffer();
void Pal_drawBuffer(void *buffer, int width, int height, int pitch);
void Pal_clearScreen();
void Pal_error(const char *message);
void Pal_setScreenSize(int width, int height); // limits drawing within fullscreen
int Pal_getScreenBytesPerPixel();

int Pal_peekKey();
void Pal_getKey(int *value, int *down);


/*void* SDL_CreateThread(const char *name, void *arg);
void SDL_KillThread(void *handle);
void SDL_LockMutex(SDL_mutex *mutex);
void SDL_UnlockMutex(SDL_mutex *mutex);
SDL_mutex* SDL_CreateMutex();
void SDL_DestroyMutex(SDL_mutex *mutex);
void SDL_Delay(int amount);

SDL_cond* SDL_CreateCond(void);
void SDL_DestroyCond(SDL_cond* cond);
int SDL_CondWait(SDL_cond*  cond, SDL_mutex* mutex);
int SDL_CondSignal(SDL_cond* cond);*/



#ifdef __cplusplus
}
#endif
