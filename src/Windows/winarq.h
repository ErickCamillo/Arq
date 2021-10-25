#ifndef WINARQ_H      
#define WINARQ_H 
#endif

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sendfile_arq(int sock, char *filename, int rbytes); 
int downloadfile_arq(int sock, char *output, int rbytes); 
char *getfilename_arq(char *path); 
long long getfilesize_arq(FILE *file); 
int copyfile_arq(char *src , char *dest, int rbytes);