#ifndef READ_H
#define READ_H 
#include<stdio.h>
#include "types.h"

typedef unsigned int uint;

typedef struct 
{
    FILE *fptr_mp3;
    uint  tag_size;
    char *tag_dat; 
}Tag;

Status read_and_validate(char **argv,Tag *tag);
Status view_tag(Tag *tag);
Status display_data(Tag *tag);
OperationType check_operation(char **argv);
#endif