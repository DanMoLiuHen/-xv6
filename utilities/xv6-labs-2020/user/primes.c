#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

void sieve(int pipe_left[2])
{
    int num;
    //get first number,it must be prime
    read(pipe_left[0],&num,sizeof(num));
    if(num==-1)//all numbers are processed ,then exit
        exit(0);
    printf("prime %d\n",num);

    int pipe_right[2];
    pipe(pipe_right);//save the input of next stage 
    if(fork()==0)
    {//child process
        //child only need read ,so close write
        close(pipe_right[1]);
        //pipeleft is parent ,child don't need,close
        close(pipe_left[0]);
        sieve(pipe_right);//start next stage
    }
    else 
    {//parent process
        close(pipe_right[0]);
        int buf;
        while(read(pipe_left[0],&buf,sizeof(buf))&&buf!=-1)//get number from pipileft
        {
            if(buf%num!=0)//filter
                //write remaining numbers in pipiright
                write(pipe_right[1],&buf,sizeof(buf));
        }
        buf=-1;
        write(pipe_right[1],&buf,sizeof(buf));
        wait(0);
        exit(0);
    }
}

int main()
{
    int input_pipe[2];
    pipe(input_pipe);//input all numbers from 2 to 35

    if(fork()==0)
    {//child
        close(input_pipe[1]);//don't need write
        sieve(input_pipe);
        exit(0);
    }
    else
    {//main process
        close(input_pipe[0]);
        int i;
        for (i=2;i<=35;i++)//create [2,35]
            write(input_pipe[1],&i,sizeof(i));
        //mark finish with -1
        i=-1;
        write(input_pipe[1],&i,sizeof(i));
    }
    wait(0);//wait for first stage 
    exit(0);
}