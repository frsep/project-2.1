#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/param.h>
#include  <dirent.h>
#include  <time.h>
#include  <unistd.h>
#include "directs.h"
#include <string.h>
#include <stdlib.h>
#define MAX_PATH_LEN  100

char options[7][2] = {"-a","-i","-n","-o","-p","-r","-v"};
char a='N';
char n='N';
char v='N';
char r='N';
char p='N';
char i_switch='N';
char o='N';
char *i_pattern;
char *o_pattern;
int input_directories_count = 0;
struct directory *all_directories;
int read_input(int argc, char const *argv[]){
    int indx=1;
    while((indx < argc) && (argv[indx][0] == '-')){
        if(strcmp(argv[indx], "-a") == 0){
            a='Y';
        }
        else if(strcmp(argv[indx], "-i") == 0){
            i_switch='Y';
            i_pattern = malloc(strlen(argv[indx+1]) + 1);
            strcpy(i_pattern, argv[indx+1]);
            indx++;
        }
        else if(strcmp(argv[indx], "-n") == 0){
            n='Y';
            v='Y';
        }
        else if(strcmp(argv[indx], "-o") == 0){
            o='Y';
            o_pattern = malloc(strlen(argv[indx+1]) + 1);
            strcpy(o_pattern, argv[indx+1]);
            indx++;
        }
        else if(strcmp(argv[indx], "-p") == 0){
            p='Y';
        }
        else if(strcmp(argv[indx], "-r") == 0){
            r='Y';
        }
        else if(strcmp(argv[indx], "-v") == 0){
            v='Y';
        }
        else
        {
            printf("Invalid option: %s\n", argv[indx]);
            return -1;
        }
        indx++;
    }
    input_directories_count = 0;
    while (indx < argc) {
        if (input_directories_count == 0)
        {
            all_directories = (struct directory *)malloc(sizeof(struct directory));
            all_directories[input_directories_count].name = malloc(strlen(argv[indx]) + 1);
            all_directories[input_directories_count].isdirect = 'Y';
            strcpy(all_directories[input_directories_count].name, argv[indx]);
            input_directories_count++;
        }
        else
        {
            all_directories = (struct directory *)realloc(all_directories, sizeof(struct directory) * (input_directories_count + 1));
            all_directories[input_directories_count].name = malloc(strlen(argv[indx]) + 1);
            all_directories[input_directories_count].isdirect = 'Y';
            strcpy(all_directories[input_directories_count].name, argv[indx]);
            input_directories_count++;
        }
        indx++;
    }
    return 0;
}
/*
int read_input(int argc, char const *argv[]){
    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i], "-a") == 0)
        {
            a='Y';
        }
        else if(strcmp(argv[i], "-i") == 0){
            
        }
        else if(strcmp(argv[i], "-n") == 0){
            n='Y';
            v='Y';
        }
        else if(strcmp(argv[i], "-o") == 0){
        }
        else if(strcmp(argv[i], "-p") == 0){
            
        }
        else if(strcmp(argv[i], "-r") == 0){
            r='Y';
        }
        else if(strcmp(argv[i], "-v") == 0){
            v='Y';
        }
        else{
            if(input_directories_count==0){
                all_directories= (struct directory *)malloc(sizeof(struct directory));
            }
            else{
                all_directories= (struct directory *)realloc(all_directories, sizeof(struct directory)*(input_directories_count+1));
            }
            all_directories[input_directories_count].name = malloc(strlen(argv[i]) + 1);
            strcpy(all_directories[input_directories_count].name,argv[i]);
            all_directories[input_directories_count].isdirect = 'Y';
            input_directories_count ++;
        }
    }
    return 0;
}
*/
void copy_file(char *source, char *destination){
    FILE *fptr1, *fptr2;
    char c;
    fptr1 = fopen(source, "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", source);
        exit(0);
    }
    fptr2 = fopen(destination, "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", destination);
        exit(0);
    }
    while ((c = fgetc(fptr1))!= EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);
}

char is_it_real(char *name){
    if(strcmp(name,".")==0 || strcmp(name,"..")==0){
        return 'N';
    }
    else{
        return 'Y';
    }
}
char is_in_other(struct directory d1,struct directory *d2, int d2_size){
    for(int i = 0; i<d2_size;i++){
        if(strcmp(d1.name,d2[i].name)==0){
            if(d1.mod_time>d2[i].mod_time){
                return 'O';
            }
            return 'Y';
        }
    }
    return 'N';
}
char which_is_newer(struct directory d1,struct directory *d2, int d2_size){
    if(d1.mod_time>d2->mod_time){
        return 'Y';
    }
    return 'N';
    }
char is_it_hidden(char *path){
    if(path[0] == '.'){
        return 'Y';
    }
    else{
        return 'N';
    }
}
void compare_directs(struct directory *d1){
    for(int j=0;j<d1[0].n_files;j++){
        char compare = is_in_other(d1[0].directs[j],d1[1].directs,d1[1].n_files);
        if(compare=='Y'){
            printf("%s is in both\n",d1[0].directs[j].name);
        }
        else if(compare=='O'){ 
            printf("%s is in %s but is older in other\n",d1[0].directs[j].name,d1[1].name);
        }
        else{
            printf("%s is in %s but not in other\n",d1[0].directs[j].name,d1[1].name); 
        }
    }
}

void read_in_direcotry(struct directory *outer_directories, int input_directories_count)
{
    for (int i = 0; i < input_directories_count; i++)
    {
        DIR *dirp;
        struct dirent *dp;
        int inner_count = 0;
        if (inner_count == 0)
        {
            outer_directories[i].directs = (struct directory *)malloc(sizeof(struct directory));
        }
        else
        {
            outer_directories[i].directs = (struct directory *)realloc(outer_directories[i].directs, sizeof(struct directory) * (inner_count + 1));
        }
        // printf("name is: %s, is it directory: %c, i: %d\n",outer_directories[i].name,outer_directories[i].isdirect,i);
        if (outer_directories[i].isdirect == 'Y')
        {
            outer_directories[i].n_files = -1;
            dirp = opendir(outer_directories[i].name);
            char fullpath[MAXPATHLEN];
            if (dirp == NULL)
            {
                ///////////perror( progname );
                if(v=='Y'){
                    printf("error oppening a directory\n");
                }
                else{
                    printf("error oppening directory %s\n",outer_directories[i].name);
                }
                ////exit(EXIT_FAILURE);
            }
            else
            {
                while ((dp = readdir(dirp)))
                {
                    struct stat stat_buffer;
                    stat(dp->d_name, &stat_buffer);
                    if (is_it_real(dp->d_name) == 'N')
                    {
                        continue;
                    }
                    if (a == 'N' && is_it_hidden(dp->d_name) == 'Y')
                    {
                        continue;
                    }
                    outer_directories[i].directs[inner_count].mod_time = stat_buffer.st_mtime;
                    if (dp->d_type == DT_DIR)
                    {
                        //////printf("2.%s\n",dp->d_name);
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name) + 1);
                        strcpy(outer_directories[i].directs[inner_count].name, dp->d_name);
                        outer_directories[i].n_files += 1;
                        // printf("directory:%s\n",outer_directories[i].directs[inner_count].name);
                        // printf("date:%ld\n",outer_directories[i].directs[inner_count].mod_time);
                        outer_directories[i].directs[inner_count].isdirect = 'Y';
                        if (r == 'Y')
                        {
                            read_in_direcotry(outer_directories[i].directs, inner_count);
                        }
                    }
                    else if (dp->d_type == DT_REG)
                    {
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name) + 1);
                        strcpy(outer_directories[i].directs[inner_count].name, dp->d_name);
                        outer_directories[i].n_files += 1;
                        // printf("file:%s\n",outer_directories[i].directs[inner_count].name);
                        // printf("date:%ld\n",outer_directories[i].directs[inner_count].mod_time);
                        outer_directories[i].directs[inner_count].isdirect = 'N';
                    }
                    else
                    {
                        if (v == 'Y')
                        {
                            printf("%s is unknown!\n", dp->d_name);
                        }
                        else
                        {
                            printf("unknown file!\n");
                        }
                    }
                    inner_count += 1;
                }
                closedir(dirp);
            }
        }
    }
}


/*
void read_in_direcotry(struct directory *outer_directories, int input_directories_count){
    for(int i = 0; i<input_directories_count;i++){
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
            outer_directories[i].n_files = -1;
            dirp = opendir(outer_directories[i].name);
            char  fullpath[MAXPATHLEN];
            if(dirp == NULL){
                ///////////perror( progname );
                //////printf("error oppening directory\n");
                ////exit(EXIT_FAILURE);
            }
            else{
                while((dp = readdir(dirp)) != NULL) {
                    struct stat  stat_buffer;
                    stat(dp->d_name,&stat_buffer);
                    if(is_it_real(dp->d_name)=='N'){
                            continue;
                        }
                    if(a=='N' && is_it_hidden(dp->d_name)=='Y'){
                        continue;
                    }
                    outer_directories[i].directs[inner_count].mod_time=stat_buffer.st_mtime;
                    if(dp->d_type == DT_DIR) {
                        //////printf("2.%s\n",dp->d_name);
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name)+1);
                        strcpy(outer_directories[i].directs[inner_count].name,dp->d_name);
                        outer_directories[i].n_files+=1;
                        //printf("directory:%s\n",outer_directories[i].directs[inner_count].name);
                        //printf("date:%ld\n",outer_directories[i].directs[inner_count].mod_time);
                        outer_directories[i].directs[inner_count].isdirect = 'Y';
                        if(r=='Y'){
                            read_in_direcotry(outer_directories[i].directs,inner_count);
                        }
                    }
                    else if( dp->d_type == DT_REG ) {
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name)+1);
                        strcpy(outer_directories[i].directs[inner_count].name,dp->d_name);
                        outer_directories[i].n_files+=1;
                        //printf("file:%s\n",outer_directories[i].directs[inner_count].name);
                        //printf("date:%ld\n",outer_directories[i].directs[inner_count].mod_time);
                        outer_directories[i].directs[inner_count].isdirect = 'N';
                    }
                    else {
                        if( v == 'Y'){
                            printf( "%s is unknown!\n", dp->d_name );
                        }
                        else{
                            printf( "unknown file!\n" );
                        }
                    }
                    inner_count+=1;
                }
                closedir(dirp);
            }
        }
    }
}
*/

int main(int argc, char const *argv[])
{
read_input(argc, argv);
for(int i = 0; i<input_directories_count;i++){
    printf("%s\n",all_directories[i].name);
}
read_in_direcotry(all_directories,input_directories_count);
compare_directs(all_directories);
}
