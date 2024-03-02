#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#define LINHAS 25
#define COLUNAS 40
#define TRACOS COLUNAS*4+1

//Dar print ao meu pid e ao pid do interpretador
void exercicio1(){
	pid_t pid  = getpid();
	pid_t ppid = getppid();

	printf("Pid : %d\nPid pai : %d\n",pid,ppid);

}

//Criar filho, pai dar pid filho, filho dar pid pai
void exercicio2(){
	pid_t pid;

	if ((pid = fork())==0){
		//codigo filho
		printf("ppid = %d\n",getppid());
		_exit(0);
	}
	else{
		//codigo pai
		printf("pid = %d\n",pid);
	}
}

//Criar 10 filhos sequencialmente, esperar por cada um e devolver o seu exitstatus
void exercicio3(){
	int i = 0;
	int status;

	//Criar 10 filhos e esperar
	while(i<10){
		if (fork()==0){
			printf("ppid = %d pid = %d\n",getppid(),getpid());
			_exit(i);
		}
		else{
			i++;
			wait(&status);

			//WEITSTATUS devolve o valor real do status
			status = WEXITSTATUS(status);
			printf("status = %d\n",status);
		}
	}
}

//Criar 10 filhos em concorrencia 
void exercicio4(){
	int i = 0;
	int status;

	//Criar 10 filhos e esperar
	while(i<10){
		if (fork()==0){
			printf("ppid = %d pid = %d\n",getppid(),getpid());
			_exit(i);
		}
		else{
			i++;
			
		}
	}
	while(i>0){
		wait(&status);
	
		//WEITSTATUS devolve o valor real do status
		status = WEXITSTATUS(status);
		printf("status = %d\n",status);
		i--;
	}
}


//Criar matriz com valores aleatorios, fazer filhos iguais as linhas para encontrarem se o valor dado existe em alguma delas
void exercicio5(int n){

	int matriz[LINHAS][COLUNAS];
	int i;
	int j;
	for (int z = 0;z<TRACOS;z++)printf("-");
	printf("\n");
	for (i = 0;i<LINHAS;i++){
		for (j = 0;j<COLUNAS;j++){

			matriz[i][j] = rand()%100;
			if (matriz[i][j]>9)printf("| %d",matriz[i][j]);
			else printf("| %d ",matriz[i][j]);
		}
		printf("|\n");
		for (int z = 0;z<TRACOS;z++)printf("-");
		printf("\n");
	}

	i = 0;
	while(i<LINHAS){
		if(fork()==0){
			j = 0;
			while (j<COLUNAS){
				if (matriz[i][j]==n){
					printf("Number found!!! Line : %d Column : %d\n",i+1,j+1);
					_exit(0);
				}
				else j++;
			}
			_exit(1);
		}
		else{
			i++;
		}
	}	
	int status;
	int empty = 0;
	while(i>0){
		wait(&status);
		i--;
		if ((status = WEXITSTATUS(status))==0) empty++;
	} 
	if (empty==0) printf("Numero nao encontrado\n");
}

//Mesmo que anterior mas imprimir as linhas na ordem crescente
void exercicio6(int n){

	int matriz[LINHAS][COLUNAS];
	int i;
	int j;
	for (int z = 0;z<TRACOS;z++)printf("-");
	printf("\n");
	for (i = 0;i<LINHAS;i++){
		for (j = 0;j<COLUNAS;j++){

			matriz[i][j] = rand()%100;
			if (matriz[i][j]>9)printf("| %d",matriz[i][j]);
			else printf("| %d ",matriz[i][j]);
		}
		printf("|\n");
		for (int z = 0;z<TRACOS;z++)printf("-");
		printf("\n");
	}

	i = 0;
	while(i<LINHAS){
		if(fork()==0){
			j = 0;
			while (j<COLUNAS){
				if (matriz[i][j]==n){
					_exit(i);
				}
				else j++;
			}
			_exit(0);
		}
		else{
			i++;
		}
	}	
	int status;
	int empty = 0;
	int linhas[LINHAS]={0} ;
	while(i>0){
		wait(&status);
		i--;
		if ((status = WEXITSTATUS(status))!=0){
			empty++;
			linhas[LINHAS-1-i]=LINHAS-i;
		}
	} 
	if (empty==0) printf("Number was not found in any line\n");
	else {
		i = 0;
		while (i<LINHAS){
			if (linhas[i]!=0) printf("Number found in line : %d\n",linhas[i]);
			i++;
		}
	}
}

//optional : Fazer o exercicio anterior mas imprimir a matriz para um ficheiro binario e ir la 

void optional(int n){


}

int main(int argc, char* argv[]){

    if (argc < 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    int mode = (int)argv[1][0] - '0';

    switch (mode) {
        case 1:
            exercicio1();
            break;
        case 2:
            exercicio2();
            break;
        case 3:
            exercicio3();
            break;
        case 4:
            exercicio4();
            break;
        case 5:
            int n = rand() % 100;
            printf("Random number to search in the matrix: %d\n", n);
            exercicio5(n);
            break;
        case 6:
            int n2 = rand() % 100;
            printf("Random number to search in the matrix: %d\n", n2);
            exercicio6(n2);
            break;    
        default:
            printf("Invalid mode\n");
            break;
    }

	return 0;
}

