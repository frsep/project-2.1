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
    for(int i=1;i<argc;i++)
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
            if(input_directories_count==0){
                all_directories = (struct directory *)malloc(sizeof(struct directory));
            }
            else{
                all_directories = (struct directory *)realloc(all_directories, sizeof(struct directory)*(input_directories_count+1));
            }
            all_directories[input_directories_count].name = malloc(strlen(argv[i]) + 1);
            strcpy(all_directories[input_directories_count].name,argv[i]);
            all_directories[input_directories_count].isdirect = 'Y';
            all_directories[input_directories_count].ishidden = 'N';
            input_directories_count ++;
        }
    }
    return 0;
}

void is_it_hidden(char *path){
    char *token = strtok(path, "/");
    while(token != NULL){
        token = strtok(NULL, "/");
    }
    if(token[0] == '.'){
        all_directories[input_directories_count].ishidden = 'Y';
    }
    else{
        all_directories[input_directories_count].ishidden = 'N';
    }
}
int limit = 0;
void read_in_direcotry(struct directory *outer_directories, int input_directories_count){
    for(int i = 0; i<input_directories_count;i++){
        limit ++;
        DIR             *dirp;
        struct dirent   *dp;
        int inner_count = 0;
        if(inner_count==0){
                outer_directories[i].directs = (struct directory *)malloc(sizeof(struct directory));
            }
            else{
                outer_directories[i].directs = (struct directory *)realloc(outer_directories[i].directs, sizeof(struct directory)*(inner_count+1));
            }
        //printf("name is: %s, is it directory: %c, i: %d\n",outer_directories[i].name,outer_directories[i].isdirect,i);
        if(outer_directories[i].isdirect=='Y'){
            dirp       = opendir(outer_directories[i].name);
            char  fullpath[MAXPATHLEN];
            if(dirp == NULL) {
                ///////////perror( progname );
                //////printf("error oppening directory\n");
                ////exit(EXIT_FAILURE);
            }
            else{
                while((dp = readdir(dirp)) != NULL || limit <30) {
                    struct stat  stat_buffer;
                    struct stat  *pointer = &stat_buffer;
                    if(dp->d_type == DT_DIR) {
                        //////printf("2.%s\n",dp->d_name);
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name)+1);
                        strcpy(outer_directories[i].directs[inner_count].name,dp->d_name);
                        //////printf("%s",outer_directories[i].directs[inner_count].name);
                        outer_directories[i].directs[inner_count].isdirect = 'Y';
                        //printf("going to %s\n",outer_directories[i].directs[inner_count].name);
                        read_in_direcotry(outer_directories[i].directs,inner_count);
                    }
                    else if( dp->d_type == DT_REG ) {
                        //////printf("3.%s\n",dp->d_name);
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name)+1);
                        strcpy(outer_directories[i].directs[inner_count].name,dp->d_name);
                        //////printf("%s",outer_directories[i].directs[inner_count].name);
                        outer_directories[i].directs[inner_count].isdirect = 'N';
                    }
                    else {
                        //////printf( "%s is unknown!\n", dp->d_name );
                    }
                    inner_count+=1;
                }
                closedir(dirp);
            }
        }
    }
}


int main(int argc, char const *argv[])
{
read_input(argc, argv);

for(int i = 0; i<input_directories_count;i++){
    printf("%s\n",all_directories[i].name);
}

read_in_direcotry(all_directories,input_directories_count);
}
