//
// Created by richard on 2023/2/10.
//
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

char*
getType(const struct stat sb){
    if(S_ISREG(sb.st_mode))
        return "regular file";
    else if(S_ISDIR(sb.st_mode))
        return "directory";
    else if (S_ISCHR(sb.st_mode))
        return "character special";
    else if (S_ISBLK(sb.st_mode))
        return "block special";
    else if (S_ISFIFO(sb.st_mode))
        return "FIFO";
    else if (S_ISLNK(sb.st_mode))
        return "symbolic link";
    else if (S_ISSOCK(sb.st_mode))
        return "socket";
    else
        return "unknown";
}

int main(int argc, char* argv[]){
    DIR* dp;
    struct dirent *dirp;
    if(argc !=2){
        fprintf(stderr,"usage error, %s dir_name\n",argv[0]);
        exit(EXIT_FAILURE);
    }


    if((dp= opendir(argv[1]))==NULL){
        fprintf(stderr,"unable to open dir %s",argv[1]);
        exit(EXIT_FAILURE);
    }
    if(chdir(argv[1])== -1 ){
        fprintf(stderr,"can not chdir to %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    while((dirp= readdir(dp))!=NULL) {
        struct stat sb;
        char *statType;
        printf("%s:", dirp->d_name);
        if (stat(dirp->d_name, &sb) == -1) {
            fprintf(stderr, "can not stat file %s:%s\n", dirp->d_name, strerror(errno));
            statType = "unknown";
        } else {
            statType = getType(sb);
        }
        if (lstat(dirp->d_name, &sb) == -1) {
            fprintf(stderr, "cannot lstat file %s:%s\n", dirp->d_name, strerror(errno));
            continue;
        } else if (S_ISLNK(sb.st_mode))
            printf("symlink to ");
        printf("%s\n", statType);
    }
    (void)closedir(dp);
    return EXIT_FAILURE;
}