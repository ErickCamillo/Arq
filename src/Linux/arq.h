#ifndef ARQ_H      
#define ARQ_H 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int sendfile_arq(int sock, char *filename, int rbytes); 
int downloadfile_arq(int sock, char *output, int rbytes); 
char *getfilename_arq(char *path); 
long long getfilesize_arq(FILE *file); 
int copyfile_arq(char *src , char *dest, int rbytes);