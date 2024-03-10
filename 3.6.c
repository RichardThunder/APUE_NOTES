//
// Created by richard on 2023/2/7.
//
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#define BUFFSIZE 512
int main(int argc, char* argv[]){
    char buf[]="abcdef";
    char buf2[BUFFSIZE];
    const int fd=open(argv[1],O_APPEND| O_RDWR);
    if(fd<0){
        perror("open file failed");
       exit(0);
    }
    printf("fd=%d\n",fd);
    int n;
    if(lseek(fd,0,SEEK_SET)<0){
        perror("lseek error");
        exit(EXIT_FAILURE);
    }
    if(read(fd,buf2,BUFFSIZE)<0){
        perror("read error");
        exit(EXIT_FAILURE);
    }
   fprintf(stdout,buf2);
   if(write(fd,"abcdfe",strlen("abcdef"))<0){
   perror("write error");
       exit(EXIT_FAILURE);
   }
   exit(1);

}
