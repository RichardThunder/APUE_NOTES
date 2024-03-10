//
// Created by richard on 2023/2/7.
//
#include <sys/types.h>

#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int dup3(int,int);
int check_dup(int);
//int dup3(int fd1,int fd2){
//    int table_size= getdtablesize();
//    int newfd=-1;
//    //test fd is in randge of max table size
//    if(fd1<0 || fd1>=table_size || fd2< 0|| fd2 >= table_size) {
//        errno=EBADF;
//        exit(EXIT_FAILURE);
//    }
//    if(fd1==fd2) return fd2;
//    close(fd2);
//
//    //定义一个fd,如果小于newfd,就一直新建,直到相等,最后删除之前的描述符.
//    int fds[fd2+1];
//    int index=0;
//    for(; index<(fd2+1);index++){
//        //check fd is open
//        fds[index]=0;
//        if((newfd=dup(fd1))<0){
//            errno=EBADF;
//            exit(EXIT_FAILURE);
//        }
//
//        if(newfd == -1 || newfd== fd2 +1 ) {
//            break;
//
//        }
//        fds[newfd] = 1;}
//    for(int i =0;i<fd2+1;i++){
//        if(fds[i]==1 && i != fd1)
//        close(i);
//    }
//
//    return fd2;
//}
int dup3(int oldfd,int newfd){
    //相等直接返回
    if(newfd==oldfd)
        return oldfd;
    //超出范围处理
    int MaxTableSize=getdtablesize();
    if(oldfd<0 || oldfd >=MaxTableSize || newfd <0 || newfd >=MaxTableSize)
    {        errno=EBADF;
        perror("fd out of range");
        exit(EXIT_FAILURE);
    }
    //关闭将要使用的fd
    close(newfd);
    int fds[newfd+1];
    int index=oldfd+1;
    int nowfd=-1;
    //循环自增到目标fd,记录已使用的fd,以便于关闭.
    for(;index<newfd+1;index++){
       fds[index]=-1;
        if((nowfd=dup(oldfd))<0)
       {    errno=EBADF;
            break;
       }
        fds[nowfd]=1;
    }
    //清理不需要的fd
    for(index=oldfd+1;index<newfd;index++){
        if(fds[index]==1)
            close(index);
    }
    return newfd;
}
int main(int argc, char * argv[]){
    //close fd 2,  2>&1
    int fd = strtol(argv[1],NULL,10);
    dup3(STDOUT_FILENO,fd);
    if(write(fd,"this is new fd,output relocated to STDOUT_FILENO\n",
             strlen("this is new fd,output relocated to STDOUT_FILENO\n"))<0){
        printf("%d",fd);
        perror("write error");
    }
    //else printf("write sucess");

    exit(1);
}

