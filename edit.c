#include "edit.h"
#include<string.h>

Status read_and_validate_edit_args(char **argv,Tag_Edit *tag)
{
    printf("readinng and validate edit args \n");
    if(argv[2]==NULL)
    {
        printf ("INFO: For editing  the Tags -> ./mp3_tag_reader -v <file_name.mp3>\n");
        return failure;
    }
    else
    {
        if(strcmp(argv[2],"-t")==0)
        {
            strcpy(tag->Frame_ID,"TIT2");
        }
        else if(strcmp(argv[2],"-A")==0)
        {
            strcpy(tag->Frame_ID,"TPE1");
        }
        else if(strcmp(argv[2],"-a")==0)
        {
            strcpy(tag->Frame_ID,"TALB");
        }
        else if(strcmp(argv[2],"-y")==0)
        {
            strcpy(tag->Frame_ID,"TYER");
        }
        else if(strcmp(argv[2],"-G")==0)
        {
            strcpy(tag->Frame_ID,"TCON");
        }
        else if(strcmp(argv[2],"-c")==0)
        {
            strcpy(tag->Frame_ID,"COMM");
        }
        else
        {
            printf("Unsupported argument.\n");
            printf("Arguments available :\n");
            printf ("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\n");
        }
    }
    printf("Frame ID That has to be changed is %s\n",tag->Frame_ID);
}