#include"kernel/types.h"
#include"user/user.h"
#include"stddef.h"

int main(int argc,char*argv[])
{
    //create a pipe will result in an int array with a length of 2
    int parent_to_child[2],child_to_parent[2];
    pipe(parent_to_child);//create a pipeline for parent to child process
    pipe(child_to_parent);

    char buf[8];//store ping and pong
    
    if(fork()!=0)
    {//parent process
        write(parent_to_child[1],"ping",4);//parent send data
        wait(NULL);//wait for child finish
        read(child_to_parent[0],buf,4);//child receivs data 
        printf("%d: received %s\n",getpid(),buf);//print message
    }
    else
    {//child process
        read(parent_to_child[0],buf,4);//child read pipe ,receive parent's data
        printf("%d: received %s\n",getpid(),buf);//print data and pid
        write(child_to_parent[1],"pong",4);//child send data to parent
    }
    exit(0);
}
