//
// Created by richard on 2023/1/30.
//
#include <stdio.h>
#include <ntsid.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#define NUM 2

struct actor
{
    char name[30];
    char id[8];
};

int main(){
    pid_t pid;
    struct actor actress[NUM]={{"ChantalCarol","17022417"},{"CNBili","96110177"}};
    char *base="ffmpeg -hwaccel cuda -thread_queue_size 4096 -i https://b-hls-17.doppiocdn.com/hls/";
    char url[1024]="";
    for (int i=0;i<NUM;i++){
        if(actress[i].id==0)
            continue;
        if( (pid=fork())>0){
            time_t time_ptr;
            strcat(url,base);
            strcat(url,"/");
            strcat(url,actress[i].id);
            strcat(url,"/");
            strcat(url,actress[i].id);
            strcat(url,".m3u8");
            strcat(url," -b_ref_mode 0 -b:v 2.0M -c:v hevc_nvenc -vtag hvc1  ");
            strcat(url,actress[i].name);
            strcat(url,ctime(&time_ptr));
            strcat(url,".mp4");
            system(url);
    }

    }
exit(1);

}
