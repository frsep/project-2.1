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

int main(int argc, char const *argv[])
{
    DIR direct *dirct;  
    dirct= opendir(argv[1]);
    if (dirct == NULL) 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    }
    struct dirent *entry;
    while((entry=readdir(dirct) ))
    {
        printf("File : %s, %d\n",
                entry->d_name,
                entry->d_type
              );
    }
}
