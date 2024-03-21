#include <fcntl.h>      /* O_RDONLY, O_WRONLY, O_CREAT, O_APPEND, O_RDWR , O_TRUNC, O_EXCL, O_* */
#include <unistd.h>     /* chamadas ao sistema: defs e decls essenciais */
#include <stdio.h>      /* printf */
#include <time.h>       
#include <sys/wait.h>   /* wait */
#include <stdlib.h>     /* rand */

#define LINHAS 15
#define COLUNAS 40
#define TRACOS COLUNAS*4+1

void printMatrix(int matriz[LINHAS][COLUNAS],int n){
    int i;
    int j;
    for (int z = 0;z<TRACOS;z++)printf("\033[1;37m-\033[1;0m");
    printf("\n");
    for (i = 0;i<LINHAS;i++){
        for (j = 0;j<COLUNAS;j++){

            matriz[i][j] = rand()%100;
            if (matriz[i][j]>9) {
                if (matriz[i][j] == n) printf("\033[1;37m|\033[1;0m \033[1;32m%d\033[1;0m",matriz[i][j]);
                else printf("\033[1;37m|\033[1;0m \033[1;31m%d\033[1;0m",matriz[i][j]);
            }
            else if (matriz[i][j] == n) printf("\033[1;37m|\033[1;0m \033[1;32m%d\033[1;0m ",matriz[i][j]);
            else printf("\033[1;37m|\033[1;0m \033[1;31m%d\033[1;0m ",matriz[i][j]);
        }
        printf("|\n");
        for (int z = 0;z<TRACOS;z++)printf("\033[1;37m-\033[1;0m");
        printf("\n");
    }
}

// Criar pai e filho, criar pipe, pai mandar para filho um inteiro gerado por ele 
void exercicio1(){

    int pip[2];

    if (pipe(pip)== -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    if ((pid=fork())==-1){
        perror("Filho");
        exit(EXIT_FAILURE);
    }

    if(pid==0){

        int n;
        
        // WARNING : nao e preciso abrir os pipes para escrever ou abrir 
        /*
        if (open(pip[0],O_RDONLY)== -1 ){
            perror("Open pipe ler");
            exit(EXIT_FAILURE);
        }
        */
        if (read(pip[0],&n,sizeof(int))==-1){
            perror("Ler pipe");
            exit(EXIT_FAILURE);
        }
        
        printf("Number : %d\n",n);
        _exit(0);
    
    }
    else{
        
        int n = rand()%100;

        // WARNING : nao e preciso abrir os pipes para escrever ou abrir 
        /*
        if (open(pip[1],O_WRONLY)==-1){
            perror("Open pipe write");
            exit(EXIT_FAILURE);
        }
        */

        if (write(pip[1],&n,sizeof(int))==-1){
            perror("Write pipe");
            exit(EXIT_FAILURE);
        }
        
        int status;
        wait(&status);
    
    }
}

// Exercicio 1 mas pai le do filho, (a) - sequencia de inteiros , (b) - leitura do pipe ate receber end-of-file

void exercicio2(char mode){

    int pip[2];
    int n;
    int status;
    pid_t pid;
    if (pipe(pip)== -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    switch(mode){

        // (a)
        case 'a' :

            if ((pid=fork())==-1){
                perror("Filho");
                exit(EXIT_FAILURE);
            }
        
            if(pid==0){
                
                for(int i=0;i<10;i++){
                    n = rand()%100;
                    
                    if (write(pip[1],&n,sizeof(int))==-1){
                        perror("Write pipe");
                        exit(EXIT_FAILURE);
                    }                    
                }
                _exit(0);
            
            }
            else{    
                int numbers[10];
                for(int i = 0 ; i < 10 ; i++ ){
                    if (read(pip[0],numbers+i,sizeof(int))==-1){
                        perror("Ler pipe");
                        exit(EXIT_FAILURE);
                    }
                    printf("Number : %d\n",numbers[i]);
                }
                wait(&status);
            
            }


            break;

        // (b)
        case 'b' :

            if ((pid=fork())==-1){
                perror("Filho");
                exit(EXIT_FAILURE);
            }
        
            if(pid==0){
                
                for(int i=0;i<rand()%50;i++){
                    n = rand()%100;
                    
                    if (write(pip[1],&n,sizeof(int))==-1){
                        perror("Write pipe");
                        exit(EXIT_FAILURE);
                    }                    
                }
                close(pip[1]);
                _exit(0);
            
            }
            else{    
                int numbersB[100];
                int rV=1;
                close(pip[1]);
                for(int i = 0 ; (rV = read(pip[0],numbersB+i,sizeof(int))) > 0 ; i++ ){
                    printf("Number : %d\n",numbersB[i]);
                }

                if (rV==-1){
                        perror("Ler pipe");
                        exit(EXIT_FAILURE);
                }
            }

            break;
        default :
            if ((pid=fork())==-1){
                perror("Filho");
                exit(EXIT_FAILURE);
            }
        
            if(pid==0){
                
               n = rand()%100;
                
                if (write(pip[1],&n,sizeof(int))==-1){
                    perror("Write pipe");
                    exit(EXIT_FAILURE);
                }

                _exit(0);
            
            }
            else{    

                if (read(pip[0],&n,sizeof(int))==-1){
                    perror("Ler pipe");
                    exit(EXIT_FAILURE);
                }
                
                printf("Number : %d\n",n);
                wait(&status);
            
            }


            break;

    }

}

int main(int argc, char* argv[]){

    if (argc < 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("\tmode 1 : exercicio1\n");
        printf("\tmode 7 : optional\n");
        return 1;
    }

    srand(time(NULL));

    int mode = (int)argv[1][0] - '0';

    switch (mode) {
        case 1:
            exercicio1();
            break;
        case 2:
            if (argv[2][0] != NULL)exercicio2(argv[2][0]);
            else exercicio2('c');
            break;
        default:
            printf("Invalid mode\n");
            return 1;
    }

	return 0;
}