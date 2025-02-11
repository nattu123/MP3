#ifndef EDIT_H
#define EDIT_H
#include<stdio.h>
#include "types.h" 

typedef unsigned int uint;

typedef struct 
{
    FILE *fptr_mp3_edit;
    char Frame_ID[5] ;
    uint size_new_data;
    FILE *fptr_temp;
    char *new_frame_data;
    
}Tag_Edit;

Status read_and_validate_edit_args(char **argv,Tag_Edit *tag);
Status edit_tag(Tag_Edit *tag);
Status copy_data(Tag_Edit *tag,int flag);
Status copy_remaining_data(Tag_Edit *tag);
Status rename_and_close_files(Tag_Edit *tag,char **argv);

#endif