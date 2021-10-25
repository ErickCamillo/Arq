#include "arq.h"

// Copia o arquivo de src para dest
int copyfile_arq(char *src , char *dest, int rbytes)
{
    char *buf = malloc(rbytes) , *filename = dest;
    int bytesread , byteswrite , error;
    FILE *src_file , *dest_file;

    src_file = fopen(src , "rb");
    if(src_file == NULL) return errno;

    if(dest == NULL) filename = getfilename_arq(src);

    dest_file = fopen(filename , "wb");
    if(dest_file == NULL) return errno;

    bytesread = byteswrite = error = 0;

    while((bytesread = fread(buf , 1 , rbytes , src_file)) > 0)
    {
        if(bytesread < 0)
        {   
            error = errno;
            break;
        }

        byteswrite = fwrite(buf, 1 , bytesread, dest_file);
        if(byteswrite < 0)
        {
            error = errno;
            break;
        }
    }

    fclose(src_file);
    fclose(dest_file);
    free(buf);
    return error;
}

// Retorna o nome do arquivo
char *getfilename_arq(char *path)
{
    char *filename = path;

    for(;*path != '\0';path++) if(*path == '/') filename = path + 1;

    return filename;
}

// Retorna o tamanho do arquivo. Em caso de erros retorna -1
long long getfilesize_arq(FILE *file)
{
    if(file == NULL) return 2;

    fseek(file , 0 , SEEK_END);
    long long filesize = ftell(file);
    fseek(file , 0 , SEEK_SET);

    return filesize;
}

// Envia um arquivo para o socket passado em sock.
int sendfile_arq(int sock ,char *filename, int rbytes)
{
    char *fname = getfilename_arq(filename);
    long long fsize = 0, respbytes;
    int status = 0, flag = false;

    if(sock == -1) return -1;
    
    FILE *file = fopen(filename , "rb");
    if(file == NULL) 
    {
        int erro = errno;
        char errobuf[100];

        snprintf(errobuf , sizeof(errobuf) , "errofile:%i", erro);
        send(sock , errobuf , sizeof(errobuf) , 0);

        return erro;
    }

    fsize = getfilesize_arq(file);
    if(fsize == 2)
    {
        fclose(file);
        return 2;
    }

    // Prepando buffer para receber tamanho e nome do arquivo e envia-lo
    int digits;
    long long value = fsize;
    for(digits = 0; value != 0; value = value / 10, ++digits);

    char infobuf[strlen(fname) + digits + 2];
    snprintf(infobuf , sizeof(infobuf) , "%s:%lli", fname , fsize);

    respbytes = 0;
    respbytes = send(sock, infobuf , strlen(infobuf), 0);
    if(respbytes <= 0) 
    {
        fclose(file);
        return errno;
    }
    
    respbytes = 0;
    respbytes = recv(sock , &flag , sizeof(flag), 0);
    if(respbytes > 0 && flag == true)
    {
        char *fbuffer = (char *) malloc(rbytes);
        long long bytes  = 0;

        while((bytes = fread(fbuffer , 1 , rbytes , file)) > 0)
        {
            if(bytes < 0)
            {
                status = errno;
                break;
            }

            respbytes = send(sock , fbuffer , bytes , 0);
            if(respbytes <= 0)
            {
                status = errno;
                break;
            }

            memset(fbuffer , 0 , rbytes);
            bytes = respbytes = 0;
        }
        
        free(fbuffer);
    }
    else status = errno;

    fclose(file);
    return status;

}

// Obtem um arquivo do socket passado em sock
int downloadfile_arq(int sock, char *output, int rbytes)
{
    FILE *file;
    long long respbytes = 0 , fsize = 0;
    int status = 0 , flag = false;
    char infobuf[1024] , fname[1024];

    if(sock == -1) return -1;

    respbytes = recv(sock, infobuf , sizeof(infobuf), 0);
    if(respbytes > 0 && !(strncmp(infobuf , "errofile:", 9)) == 0)
    {
        // Obtendo informações do arquivo
        int i;
        char *temp;
        for(i = 0; infobuf[i] != ':'; ++i) fname[i] = infobuf[i];
        fname[i] = '\0';
        temp = (strstr(infobuf , ":") + 1);
        fsize = atoi(temp);

        if( output != NULL)
        {
            file = fopen(output , "w+b");
            if(file == NULL) return errno;
        }
        else
        {
            file = fopen(fname , "w+b");
            if(file == NULL) return errno;
        }

        respbytes = 0;
        flag = true;
        respbytes = send(sock , (const char *)&flag , sizeof(flag), 0);
        if(respbytes <= 0 ) 
        {   
            fclose(file);
            return errno;
        }
    }
    else if(respbytes > 0 && (strncmp(infobuf , "errofile:", 9)) == 0)
    {
        int erro;
        char *tmp;

        tmp = (strstr(infobuf , ":") + 1);
        erro = atoi(tmp);

        return erro; 
    }
    else return errno;

    char *fbuffer = (char *) malloc(rbytes);
    long long bytes = 0 , full = 0;

    while(full < fsize)
    {
        respbytes = recv(sock , fbuffer , rbytes , 0);
        if(respbytes <= 0)
        {
            status = errno;
            break;
        } 

        bytes = fwrite(fbuffer , 1 , respbytes ,file);
        if(bytes < 0 )
        {
            status = errno;
            break;
        }

        full += respbytes;  
        memset(fbuffer , 0 , rbytes);
        bytes = respbytes = 0;
    }

    free(fbuffer);
    fclose(file);
    return status;
}