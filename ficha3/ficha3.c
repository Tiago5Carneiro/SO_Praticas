#include <fcntl.h>      /* O_RDONLY, O_WRONLY, O_CREAT, O_APPEND, O_RDWR , O_TRUNC, O_EXCL, O_* */
#include <unistd.h>     /* chamadas ao sistema: defs e decls essenciais */
#include <stdio.h>      /* printf */
#include <time.h>       
#include <sys/wait.h>   /* wait */
#include <stdlib.h>     /* rand */

// Executar ls -l 
int exercicio1(){

    execl("/usr/bin/ls","ls","-l",NULL);

    return 0;
}

// Filho executar ls -l
int exercicio2(){  

    int status = 0;

    if (fork()== 0){
        execl("/usr/bin/ls","ls","-l",NULL);
        _exit(0);
    }
    else{
        wait(&status);
    }

    return 0;
}

// Versao simplificada da funcao system
int exercicio3(char * func, char * arg[]){

    

    return 0;
}

int main(int argc, char* argv[]){

    if (argc < 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("\tmode 1 : exercicio1\n");
        return 1;
    }

    int mode = (int)argv[1][0] - '0';

    switch (mode) {
        case 1:
            exercicio1();
            break;
        case 2:
            exercicio2();
            break;
        default:
            printf("Invalid mode\n");
            return 1;
    }

	return 0;
}