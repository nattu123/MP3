#include<stdio.h>
#include "types.h"
#include "read.h"
#include "edit.h"

int main(int argc, char **argv)
{
    Tag tag;
    Tag_Edit tag_edit;
    if(argc)
    {
        
        if(check_operation(argv)==view)
        {
            printf("------------------------------------\n");
            printf("----- MP3 TAG VIWER AND EDITOR -----");
            printf("------------------------------------\n");
            if(read_and_validate(argv,&tag)==success);
            {
                printf("read and validate args succesfull \n ");
                if(view_tag(&tag)==success)
                {
                    printf("INFO : DONE \n");
                }
            }
        }
        else if(check_operation(argv)==edit)
        {
            printf("------------------------------------\n");
            printf("----- MP3 TAG VIEWER AND EDITOR -----");
            printf("------------------------------------\n");
            if(read_and_validate_edit_args(argv,&tag_edit)==success);
            {
                if(edit_tag(&tag_edit)==success);
                printf("INFO :Done \n");
            }

        }
        else 
        {
             printf("operation not supported \n ");
        }
    }
    
}