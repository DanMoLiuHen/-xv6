#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

/*
* argc :the number of instructions
* argc: sleep time (char*)
*/
int main(int argc,char* argv[])
{
    //error handling
    //if the number of instructions entered is not equal to 2
    if(argc!=2)
    {
        fprintf(2,"Usage:sleep <ticks>\n");
        exit(1);
    }
    int time=atoi(argv[1]);//get int 
    // pauses the specified number of ticks
    sleep(time);
    exit(0);

}