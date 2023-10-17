#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/param.h>
#include  <dirent.h>
#include  <unistd.h>
#include "directs.h"
#include <string.h>
#include <stdlib.h>
#define MAX_PATH_LEN  100

char options[7][2] = {"-a","-i","-n","-o","-p","-r","-v"};
char a='N';
char n='N';
char v='N';
char *i_name;
char *o_name;
int input_directories_count = 0;
struct directory *all_directories;

int read_input(int argc, char const *argv[]){
    for(int i=0;i<argc;i++)
    {
        if(strcmp(argv[i], "-a") == 0)
        {
            a='Y';
        }
        else if(strcmp(argv[i], "-i") == 0){
            i_name = malloc(strlen(argv[i+1]) + 1);
            strcpy(i_name,argv[i+1]);
        }
        else if(strcmp(argv[i], "-n") == 0){
            n='Y';
            v='Y';
        }
        else if(strcmp(argv[i], "-o") == 0){
            o_name = malloc(strlen(argv[i+1]) + 1);
            strcpy(o_name,argv[i+1]);
        }
        else if(strcmp(argv[i], "-p") == 0){
            
        }
        else if(strcmp(argv[i], "-r") == 0){
            
        }
        else if(strcmp(argv[i], "-v") == 0){
            v='Y';
        }
        else{
            all_directories = (struct directory *)malloc(sizeof(struct directory));
            all_directories[input_directories_count].name = malloc(strlen(argv[i]) + 1);
            strcpy(all_directories[input_directories_count].name,argv[i]);
            input_directories_count ++;
        }
    }
    return 0;
}

void read_in_direcotry(struct directory *all_directories, int input_directories_count){
    for(int i = 0; i<input_directories_count;i++){
        DIR             *dirp;
        struct dirent   *dp;

        dirp       = opendir(all_directories[i].name);
        if(dirp == NULL) {
            ///////////perror( progname );
            exit(EXIT_FAILURE);
        }
        char  fullpath[MAXPATHLEN];
        int inner_count = 0;
        while((dp = readdir(dirp)) != NULL) {
            struct stat  stat_buffer;
            struct stat  *pointer = &stat_buffer;
            if(stat(fullpath, pointer) != 0) {
                ///////////////perror( progname );
            }
            else if( S_ISDIR( pointer->st_mode )) {
                printf("%s",all_directories[i].directs[inner_count].name);
                all_directories[i].directs[inner_count].name = malloc(strlen(fullpath)+1);
                strcpy(all_directories[i].directs[inner_count].name,fullpath);
                all_directories[i].directs[inner_count].isdirect = "Y";
                read_in_direcotry(all_directories[i].directs,inner_count);
            }
            else if( S_ISREG( pointer->st_mode )) {
                printf("%s",all_directories[i].directs[inner_count].name);
                all_directories[i].directs[inner_count].name = malloc(strlen(fullpath)+1);
                strcpy(all_directories[i].directs[inner_count].name,fullpath);
                all_directories[i].directs[inner_count].isdirect = "N";
            }
            else {
                printf( "%s is unknown!\n", fullpath );
            }
            inner_count+=1;
        }
    closedir(dirp);
    }
    /*for(int i = 0; i<input_directories_count;i++){
        int file_count = 0;
        int direct_count = 0;
        DIR *dir;
        struct dirent *entry;
        dir = opendir(all_directories[i].name);
        if (dir == NULL){
            printf("error oppening directory\n");
        }
        while(entry == readdir(dir) != NULL){
            if(entry->d_type == DT_REG){
                all_directories[i].file[file_count] = entry;
                file_count ++;
            }
            else if(entry->d_type == DT_DIR){
                all_directories[i].directs[direct_count] = entry;
                direct_count ++;
            }
        }
    }*/
}


int main(int argc, char const *argv[])
{
read_input(argc, argv);
read_in_direcotry(all_directories,input_directories_count);
}
