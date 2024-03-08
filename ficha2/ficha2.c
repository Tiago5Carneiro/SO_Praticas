#include <fcntl.h>  	/* O_RDONLY, O_WRONLY, O_CREAT, O_APPEND, O_RDWR , O_TRUNC, O_EXCL, O_* */
#include <unistd.h>		/* chamadas ao sistema: defs e decls essenciais */
#include <stdio.h> 		/* printf */
#include <time.h> 		
#include <sys/wait.h> 	/* wait */
#include <stdlib.h> 	/* rand */

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
	printMatrix(matriz,n);

	int i;
	int j;

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
	if (empty==0) printf("Number not found\n");
}

//Mesmo que anterior mas imprimir as linhas na ordem crescente
void exercicio6(int n){

	int matriz[LINHAS][COLUNAS];
	printMatrix(matriz,n);
	int i;
	int j;

	i = 0;
	while(i<LINHAS){
		if(fork()==0){
			j = 0;
			while (j<COLUNAS){
				if (matriz[i][j]==n){
					_exit(i+1);
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
			linhas[status-1]=1;
		}
	} 
	if (empty==0) printf("Number was not found in any line\n");
	else {
		i = 0;
		while (i<LINHAS){
			if (linhas[i]!=0) printf("Number found in line : %d\n",i+1);
			i++;
		}
	}
}

//optional : Fazer o exercicio anterior mas imprimir a matriz para um ficheiro binario e ir la 
void optional(int n){

	int fd = open("ficha2/matriz.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
	int fd1 = fd;
	int matriz[LINHAS][COLUNAS];
	int i;
	int j;
	
	printMatrix(matriz,n);
	
	write(fd1,matriz,sizeof(int)*LINHAS*COLUNAS);

	i = 0;
	while(i<LINHAS){
		if(fork()==0){
			int line[COLUNAS]={0};
			j = 0;
			lseek(fd,sizeof(int)*COLUNAS*(i),SEEK_SET);
			read(fd,line,sizeof(int)*COLUNAS);
			while (j<COLUNAS){
				if (line[j]==n)_exit(i+1);
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
			linhas[status-1]=(status != 0);
		}
	} 
	if (empty==0) printf("Number was not found in any line\n");
	else {
		i = 0;
		while (i<LINHAS){
			if (linhas[i]!=0) printf("Number found in line : %d\n",i+1);
			i++;
		}
	}

}

int main(int argc, char* argv[]){

    if (argc < 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("\tmode 1 : exercicio1\n");
        printf("\tmode 2 : exercicio2\n");
        printf("\tmode 3 : exercicio3\n");
        printf("\tmode 4 : exercicio4\n");
        printf("\tmode 5 : exercicio5\n");
        printf("\tmode 6 : exercicio6\n");
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
        case 7:
        	int n3 = rand() % 100;
        	printf("Random number to search in the matrix: %d\n", n3);
        	optional(n3);
        	break;
        default:
            printf("Invalid mode\n");
            return 1;
    }

	return 0;
}