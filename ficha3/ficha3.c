#include <fcntl.h>      /* O_RDONLY, O_WRONLY, O_CREAT, O_APPEND, O_RDWR , O_TRUNC, O_EXCL, O_* */
#include <unistd.h>     /* chamadas ao sistema: defs e decls essenciais */
#include <stdio.h>      /* printf */
#include <time.h>       
#include <sys/wait.h>   /* wait */
#include <stdlib.h>     /* rand */
#include <string.h>

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
    }
    else{
        wait(&status);
    }

    return 0;
}

// Versao simplificada da funcao system
int exercicio3(char * func,char* argv){

    execvp(func,argv);

}

// Controlador para exectuar n executaveis e esperar ate todos darem o resultado nulo
// Pode sair parecido no teste
// ./ficha3/ficha3 4 ./a.out ./b.out ./c.out
int exercicio4(char* argv[]){

    int i=0;
    int status=0;
    int j = 1;

    while(i<3 || j<10){
        if(fork()==0){
            char path[50];
            strcpy(path,"ficha3/skeleton/");
            strcat(path,argv[i]);
            execlp(argv[i],argv[i],NULL);
            perror(argv[i]);
            _exit(4);
        }
        sleep(1);
        wait(&status);
        status = WEXITSTATUS(status);
        if (status==4) return 0;
        if (status!=0) j++;
        else {
            printf("%s :\n %d\n",argv[i],j);
            i++;
            j=1;
        }
            
    }
    return 0;
}

int main(int argc, char* argv[]){

    if (argc < 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("\tmode 1 : exercicio1\n");
        printf("\tmode 2 : exercicio2\n");
        printf("\tmode 3 : exercicio3 <command> <arg1> <arg2> ...\n");
        printf("\tmode 4 : exercicio4 <exec1> <exec2> ...\n");
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
        case 3: 
            exercicio3(argv[2],&argv[2]);
            break;
        case 4: 
            exercicio4(&argv[2]);
            break;
        default:
            printf("Invalid mode\n");
            return 1;
    }

	return 0;
}