

#include <stdio.h>
#include "directs.h"
#include <string.h>

struct hash_struct{
    char * name;
    int key;
    int state=0;
}

struct hash_struct[1000];


int double_hash(int hash_key){
    int new_indx;
    int max_step = 50;
    new_indx = 53-(hash_key%53);
    return new_indx;
}


int Hash_fuc(char*text){
    int hashcode = 0;
    int hashindx;
    for(int i=0; i<strlen(text), i++){
        hashcode = (hashcode * 31) + text[i];
    }
    hashindx = hashcode % sizeof(hash_struct);
    if(hash_struct[hashindx].state = 0){
        hash_struct[hashindx].state = 1;
        hash_struct[hashindx].name= text;
    }
    else{
        while(hash_struct[hashindx].state != 0){
            hashindx += double_hash(hash_key);
        }
    }
}