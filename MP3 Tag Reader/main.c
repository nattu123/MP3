#include<stdio.h>
#include "types.h"

int main(int argc, char **argv)
{
    if(argc)
    {
        if(check_operation(argv)==view)
        {
            
        }
        else if(check_operation(argv)==edit)
        {

        }
        else 
        {
             printf("operation not supported \n ");
        }
    }
    
}