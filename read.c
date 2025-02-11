#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "read.h"

OperationType check_operation(char **argv)
{
    if(strcmp(argv[1],"-v")==0)         return view ;
    else if(strcmp(argv[1],"-e")==0)    return edit; 
    else                                return unsupported; 
}   

Status read_and_validate(char **argv, Tag *tag)
{
    if(argv[2]==NULL)
    {
        fprintf(stderr,"INFO : View -> ./Mp3_tag_reader -v <filname.mp3>\n");
        return failure;
    }
    else 
    {
        tag->fptr_mp3 = fopen(argv[2],"rb");
        if(tag->fptr_mp3 == NULL)
        {
            perror("fopen : ");
        }
        else
        {
            char str[3];
            fread(str,1,3,tag->fptr_mp3);
            if(strncmp(str,"ID3",3)==0)
            {
                printf("   Version ID: V2.3       \n");
                printf("                          \n");
            }
            else 
            {
                printf("not ID3\n");
                return failure;
            }
        }
        rewind(tag->fptr_mp3);
    }
    return success;
}

Status view_tag(Tag *tag)
{
    fseek(tag->fptr_mp3,10,SEEK_SET);
    printf("TITLE : \n");
    if(display_data(tag)==failure)
    {
        printf("Title Frame ID conteccts cannoot be displayed ");
    }
    if(display_data(tag)==failure)
    {
        printf("Artist Frame ID contents content cannot be displayed ");
    }
    if(display_data(tag)==failure)
    {
        printf("Album Frame ID content cannot be displayed ");
    }
    if(display_data(tag)==failure)
    {
        printf("Year Frame ID content cannot be displayed ");

    }
    if(display_data(tag)==failure)
    {
        printf("Content Frame ID content cannot be displayed ");

    }
    if(display_data(tag)==failure)
    {
        printf("entaroo ethoo\n");
    }

}

Status display_data(Tag *tag)
{
    printf("----%ld----\n",ftell(tag->fptr_mp3));
    char str[5];
    fread(str,1,4,tag->fptr_mp3);
    str[4]='\0';
    printf("%s\n ",str);

    // size 

    fread(&tag->tag_size,4,1,tag->fptr_mp3);
    printf("%x\n",tag->tag_size);
    char* pos = (char*) (&tag->tag_size);
                for (int i = 0; i < ((sizeof (uint)) / 2); i++)
                {
                    pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                    pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                    pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                }

    printf("%s size = %d\n",str,tag->tag_size);
    fseek(tag->fptr_mp3,3,SEEK_CUR);
    tag->tag_dat  = (char *)malloc(tag->tag_size);
    fread(tag->tag_dat,1,tag->tag_size-1,tag->fptr_mp3);
    for(int i = 0;i<tag->tag_size;i++)
    {
        printf("%c",*(tag->tag_dat+i));
    }
    free(tag->tag_dat);
    printf("\n");
    return success ; 
}