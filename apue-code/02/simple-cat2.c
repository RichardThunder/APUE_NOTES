//
// Created by richard on 2023/2/7.
//
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFSIZE 512
int main (int argc, char **argv){
    int n;
    char buf[BUFFSIZE];
    int flags;
    if((flags= fcntl(STDOUT_FILENO,F_GETFL,0))<0){
        perror("can't get file descriptor flags");
        exit(EXIT_FAILURE);
    }
    flags |=O_SYNC;
    if(fcntl(STDOUT_FILENO,F_SETFL,flags)<0){
        perror("can't set file descriptor flags");
        exit(EXIT_FAILURE);
    }
    while ((n = read(STDIN_FILENO,buf,BUFFSIZE))>0){
        if(write(STDOUT_FILENO,buf,n)!=n){
            //为什么是n? 如果最后末尾读取的少于buffsize,那么写入时就不相等从而产生错误
            perror("write error");
            exit(EXIT_FAILURE);
        }
    }
    exit(1);
}