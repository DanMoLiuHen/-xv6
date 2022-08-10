#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"kernel/fs.h"

//find target recursively
void find(char const *path,char const*target)
{
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st;

    //open path,if return < 0 ,error
    if((fd=open(path,0))<0)
    {
        fprintf(2,"find: cannot open %s\n",path);
        exit(1);
    }

    if(fstat(fd,&st)<0)
    {
        fprintf(2,"find: cannot stat %s\n",path);
        exit(1);
    }

    switch(st.type)
    {
        //file type ,error
        case T_FILE:
            //prompt information of error report 
            fprintf(2,"Usage: find dir file\n");
            exit(1);
        case T_DIR:
            //if path is too long, error
            if(strlen(path)+1+DIRSIZ+1>sizeof(buf))
            {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf,path);
            p=buf+strlen(buf);
            *p++ = '/';
            //read fd, if number of bytes returned 
            //by read is equal to the length of de,then cycle
            while(read(fd,&de,sizeof(de))==sizeof(de))
            {
                //don't recurse "." and ".."
                if(de.inum==0||strcmp(de.name,".")==0||strcmp(de.name,"..")==0)
                    continue;
                memmove(p,de.name,DIRSIZ);
                p[DIRSIZ]=0;
                if(stat(buf,&st)<0)
                {
                    printf("find: cannot stat %s\n",buf);
                    continue;
                }
                //if directory type, search recursively
                if(st.type==T_DIR)
                    find(buf,target);
                //if it is file type and the name is same as
                //the file name to be found
                else if(st.type==T_FILE)
                {
                    if(strcmp(de.name,target)==0)
                    //print path
                        printf("%s\n",buf);
                }
            }
            break;
    }
    close(fd);
}

int main(int argc,char*argv[])
{
    //if number of parameters is not 3,error
    if(argc!=3)
    {
        fprintf(2,"Usage: find dir file\n");
        exit(1);
    }
    char const *path=argv[1];
    char const *target=argv[2];
    find(path,target);
    exit(0);
}