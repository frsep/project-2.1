#include <stdio.h>
#include <dirent.h>
#ifndef direct
#define direct

struct directory{
    char *name;
    struct directory *directs;
    char isdirect; 
    long mod_time;
    int n_files;
};
#endif