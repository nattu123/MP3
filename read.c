#include<stdio.h>
#include "types.h"

Status check_operation(char **argv)
{
    if(strcmp(argv[1],"-v")) return view ;
    else if(strcmp(argv[1],"-e")) return edit; 
    else return unsupported; 
}