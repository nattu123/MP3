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
            return failure;
        }
    }
    printf("Frame ID That has to be changed is %s\n",tag->Frame_ID);

    if(argv[3]==NULL)
    {
        printf("the new value to be updated to the frame id %s is not present \n",tag->Frame_ID);
        printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
        return failure ; 
    }
    else 
    {
        tag->size_new_data  = strlen(argv[3])+1;
        tag->new_frame_data = argv[3];
        printf(" the new data length  of frame Id %s is %d\n",tag->Frame_ID,tag->size_new_data);
    }

    if(argv[4]==NULL)
    {
        printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
        return failure;
    }
    else
    {
        tag->fptr_mp3_edit = fopen(argv[4],"r");
        if(tag->fptr_mp3_edit == NULL)
        {
            printf("could not open file %s",argv[4]);
            return failure ;
        }
        else
        {
            char str[3];
            fread(str,1,3,tag->fptr_mp3_edit);
            if(strncmp(str,"ID3",3)==0)
            {
                printf ("     Version ID: v2.3     \n");
                printf ("--------------------------\n");

            }
            else
            {
                printf("not a mp3 file \n");
                return failure;
            }
        }
    }
}

Status edit_tag(Tag_Edit *tag)
{
    tag->fptr_temp = fopen("temp.mp3","wb");

    fseek(tag->fptr_mp3_edit,0,SEEK_SET);
    fseek(tag->fptr_temp,0,SEEK_SET);

    // copied header(first 10 bytes)
    char str[10];
    if(fread(str,1,10,tag->fptr_mp3_edit)<10)
    {
        printf("Error in reading from source mp3 \n");
        return failure;
    }
    if(fwrite(str,1,10,tag->fptr_temp)<10)
    {
        printf("Error in writing to temp.mp3");
        return failure;
    }
    
    char *frame_tag[6] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};
 
    for(int i=0;i<6;i++)
    {

        if(fread(str,1,4,tag->fptr_mp3_edit)<4)
        {
            printf("Error : unable to read from mp3 \n");
            return failure;
        }
        if(fwrite(str,1,4,tag->fptr_temp)<4)
        {
            printf("unable to write to mp3");
            return failure;
        }

        if(strncmp(tag->Frame_ID,frame_tag[i],4)==0)
        {
            printf("\n\nTag frame being edited : %s\n",tag->Frame_ID);
            if(copy_data(tag,1)==failure)
            {
                printf("error in fn copy data \n");
                return failure;
            }
        }
        else
        {
            printf("\nTag frame = %s\n",frame_tag[i]);
            if(copy_data(tag,0)==failure)
            {
                printf("error in function copydata\n");
            }
        }

    }
    copy_remaining_data(tag);

}

Status copy_data(Tag_Edit *tag, int flag )
{

    printf("\n\n%ld  %ld\n\n",ftell(tag->fptr_temp),ftell(tag->fptr_mp3_edit));
    if(flag)
    {
        uint old_size,new_size; 

        if(fread(&old_size,4,1,tag->fptr_mp3_edit)<1)
        {
            printf("could not read from source file \n");
            return failure;
        }
        else
        {
            char *pos = (char *) &old_size ;
            for(int i=0;i<(sizeof(uint)/2);i++)
            {
                pos[i] = pos[i]^pos[sizeof(uint)-i-1];
                pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            }
            printf("the old size = %d\n\n",old_size);
        }
        
        new_size = tag->size_new_data-1;
        char *pos = (char *) &tag->size_new_data;
        for(int i=0;i<(sizeof(uint)/2);i++)
        {
            pos[i] = pos[i]^pos[sizeof(uint)-i-1];
            pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
        }

        if(fwrite(&tag->size_new_data,4,1,tag->fptr_temp)<1)
        {
            printf("error in writing to mp3 file");
            return failure;
        }
        // copying flags 
        char str[3];
        fread(str,1,3,tag->fptr_mp3_edit);
        fwrite(str,1,3,tag->fptr_temp);

        // copying new data
        printf("\n\nnew frame data : %s  new frame size %d \n\n",tag->new_frame_data,new_size);
        if(fwrite(tag->new_frame_data,1,new_size,tag->fptr_temp)<new_size)
        {
            printf("error in writing to the file");
            return failure;
        }
        fseek(tag->fptr_mp3_edit,old_size-1,SEEK_CUR);
        printf("-- %ld --- ",ftell(tag->fptr_temp));
        printf("-- %ld --- ",ftell(tag->fptr_mp3_edit));

    }
    else
    {

        char str[100];
        // copying size 

        uint size;
        fread(&size,4,1,tag->fptr_mp3_edit);
        fwrite(&size,4,1,tag->fptr_temp);

        printf("  \n\n od size  : %x ",size);
        char *pos = (char *) &size ;
        for(int i=0;i<(sizeof(uint)/2);i++)
        {
            pos[i] = pos[i]^pos[sizeof(uint)-i-1];
            pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];       
        }
        printf("  \n\n new size : %d ",size);
        // copy flags 

        fread(str,1,3,tag->fptr_mp3_edit);
        fwrite(str,1,3,tag->fptr_temp);

        // copy frame data 
        fread(str,1,size-1,tag->fptr_mp3_edit);
        fwrite(str,1,size-1,tag->fptr_temp);
    }   
    return success;
}

Status copy_remaining_data(Tag_Edit *tag)
{
    while(!feof(tag->fptr_mp3_edit))
    {
        char ch; 
        ch =fgetc(tag->fptr_mp3_edit);
        fputc(ch,tag->fptr_temp);
    }
    return success;
}

Status rename_files(Tag_Edit *tag)
{
    fclose(tag->fptr_temp);
    if(remove("temp.mp3")==0)
    {
        printf("File deleted succesfully\n");
    }
    else
    {
        printf("Unable to delete file \n");
        return failure;
    }
    return success;
}