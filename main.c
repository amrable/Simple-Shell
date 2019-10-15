#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t global_pid;

void handler(int sig)
{
    pid_t pid=wait(NULL);

    if(pid==-1){
        return;
    }
    FILE *fp;

    fp = fopen("trace.log", "a+");
    fprintf(fp,"Executed successfully %d\n",pid);
    fclose(fp);
}

int main()
{
    printf(">>>>>> S I M P L E    S H E L L <<<<<<\n");
    char    command[100];
    char * ptr;
    char* token;
    char * args[100];
    while(1){

        //scan command
        gets(command);

        //skip white spaces
        int end=strlen(command)-1;

        while(command[end]==' '){
            end--;
            if(end==0)break;
        }



        //check if the last char is & or not
        int dontwait=(int) command[end]=='&' ;
        if(dontwait == 1) {
            command[end]='\0';
        }

        //if(dontwait)printf("Found & don't wait\n");
        //else printf("Wait\n");



        //generate args list
        int i=0;

        token= strtok(command," ");


        while (token != NULL) {
            args[i++]=token;
            token = strtok(NULL, " ");
        }
        //i-=(int)dontwait==1;
        args[i]=NULL;

        //execute
        if(!strcmp(args[0],"exit")){
            exit(0);
        }else if (!strcmp(args[0],"cd")){
            chdir(args[1]);
        }else{


            pid_t pid=fork();

            if(pid==0){//child
                if(-1 == execvp(args[0],args)){
                    printf("Invalid commadn.\n");
                }
                exit(0);
            }else{//parent

                if(!dontwait){
                    wait(NULL);
                     FILE *fp;

                    fp = fopen("trace.log", "a+");
                    fprintf(fp,"Executed successfully %d\n",pid);
                    fclose(fp);
                }else{
                    signal(SIGCHLD,handler);
                }

            }
        }

    }
    return 0;
}
