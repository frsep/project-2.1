#include <stdio.h>
#include <dirent.h>
#ifndef direct
#define direct

struct directory{
    char *name;
    struct directory *directs;
    char isdirect; 
    char switch_type;
    long mod_time;
};
#endif