#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h>  /* O_RDONLY, O_WRONLY, O_CREAT, O_APPEND, O_RDWR , O_TRUNC, O_EXCL, O_* */
#include <stdlib.h> /* Malloc */
#include <string.h> /* stcmp */
#include <stdio.h>  /* printf */
 
#define BUFF_SIZE 1024

char buff[BUFF_SIZE];
void perror(const char *s);

struct Pessoa{
	char nome[50];
	char idade[2];
 };  


// Funcao que le do stdin e escreve para o stdout
int mycat(){
	int n;
	while((n = read(0,buff,sizeof(buff))) > 0 ){
		write(1,buff,n);
	}
	return 0;
}

// Funcao que recebe um nome de um ficheiro e dado um ficheiro 
int mycp(char* ficheiro, char* path){
	
    // Abrir descritor dos dois ficheiros, com tag para criar o novo
	int file = open(ficheiro,O_RDONLY);

	// Erro se ficheiro nao abrir 
	if (file == -1){
		perror(ficheiro);
		return -1;
	}

	// Abrir descritor de novo ficheiro
	int newFile = open(path ,O_CREAT | O_WRONLY,0644);

	// Erro se ficheiro nao abrir 
	if (newFile == -1){
		perror(path);
		return -1;
	}

	// Escrever info de ficheiro para a copia
	int n;
	while((n = read(file,buff,sizeof(buff))) > 0 ){
		write(newFile,buff,n);
	}
	close(file);
	close(newFile);
	return 0;
}

// Escreve a informacao de um objeto pessoa no stdout
int printPessoa(struct Pessoa pessoa){
	int i = 1;
	while(buff[i] != '\"'){
					i++;
				}
	i++; 
	write(1,pessoa.nome + 1,sizeof(char)*i);
	write(1," ",sizeof(char));
	write(1,pessoa.idade,sizeof(pessoa.idade));
	write(1,"\n",sizeof(char));
	return 0;
}


// Adiciona uma pessoa ao ficheiro binario
int adicionaPessoa(char* nome, char* idade){

	struct Pessoa novaPessoa;
	
	strcpy(novaPessoa.nome,nome);

	strcpy(novaPessoa.idade,idade);
	int fd = open("/tmp/pessoas.bin", O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd == -1) {
    	perror(nome);
        return -1;
    }

    write(fd,&novaPessoa,sizeof(struct Pessoa));

    close(fd);
    return 0;
}

// Lista as primeiras n pessoas que estao no ficheiro binario
int listaPessoas(char* number){

	int fd = open("/tmp/pessoas.bin", O_RDONLY | O_APPEND, 0644);

    if (fd == -1) {
    	perror("Lista");
        return -1;
    }

	struct Pessoa novaPessoa;
	
	int i = atoi(number);
	int d = 1;

	for(int n = 0; n < i && d != 0; n++){
		d = read(fd,&novaPessoa,sizeof(struct Pessoa));
		printPessoa(novaPessoa);
	}
	return 0;
}

// Atualiza uma pessoa que esteja presente no ficheiro binario
int atualizaPessoa(char* nome, char* idade){
	struct Pessoa novaPessoa;
	strcpy(novaPessoa.nome,nome);
	strcpy(novaPessoa.idade,idade);

	int fd = open("/tmp/pessoas.bin", O_RDWR | O_CREAT | O_APPEND, 0644);

    if (fd == -1) {
    	perror(nome);
        return -1;
    }

    while(read(fd,&novaPessoa,sizeof(struct Pessoa))){
    	//novaPessoa.nome=strdup(nome);

    		strcpy(novaPessoa.idade,idade);
    		
    		lseek(fd,-sizeof(struct Pessoa),SEEK_CUR);
    		write(fd,&novaPessoa,sizeof(struct Pessoa));

    		close(fd);
    		return 0;
    	
    }


    close(fd);
    return -1;
}

// Abre o CLI da struct pessoas
int handlePessoas(){

	while(read(0,buff,sizeof(buff))){
		write(1,buff,BUFF_SIZE);
		switch (buff[8]){

			case 'i' : 
				int i = 9;
				char* nome = &buff[i];
				i++;
				while(buff[i] != '\"'){
					i++;
				}
				i++;
				adicionaPessoa(nome,&buff[i++]);
				break;

			case 'o' : 
				
				int i2 = 9;
				char* nome2 = &buff[i2];

				while(buff[i2] != ' '){
					i2++;
				}
				i2++;
				atualizaPessoa(nome2,&buff[i2++]);
				break;
			case 'l' : 
				listaPessoas(&buff[10]);
				break;
				
			default : 
				
				break;
		}

	}
	return 0;
}

int main(int argc, char* argv[]){

	if (argc < 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("\tmode 1 : mycat\n");
        printf("\tmode 2 : mycp <file> <path>\n");
        printf("\tmode 3 : handlePessoas\n");
        return 1;
    }

	int mode = (int)argv[1][0] - '0';

	switch (mode){
		case 1 	: 
			mycat();
			break;
		case 2 	: mycp(argv[2],argv[3]);
			break; 
		case 3 	: 
			handlePessoas();
			break;
		default : 
			break;
	}
	
	return 0;

}