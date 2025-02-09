#ifndef EDIT_H
#define EDIT_H
#include<stdio.h>
#include "types.h" 

typedef unsigned int uint;

typedef struct 
{
    FILE *fptr_mp3_edit;
    char Frame_ID[5] ;
    
}Tag_Edit;

Status read_and_validate_edit_args(char **argv,Tag_Edit *tag);

#endif