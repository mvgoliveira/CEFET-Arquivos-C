#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco {
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2) {
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

void quickSort(long qtdRegistros, int numArq, FILE *entrada) {
  int tamanhoSaida;
  FILE *saida;
  char nomeArq[32];
  int i;
  
  for (i = 0; i < numArq; i++) {
    Endereco *e;
    
    if (i == 0) {
      snprintf(nomeArq, sizeof(char) * 32, "cep-%d.dat", i + 1);
      
      saida = fopen(nomeArq, "wb");
      
      tamanhoSaida = ((qtdRegistros/numArq) + (qtdRegistros % numArq));

      e = (Endereco*) malloc(tamanhoSaida * sizeof(Endereco));
      
      fread(e, sizeof(Endereco), tamanhoSaida, entrada);

      qsort(e, tamanhoSaida, sizeof(Endereco), compara);
      
      fwrite(e, sizeof(Endereco), tamanhoSaida, saida);
    } else {
      snprintf(nomeArq, sizeof(char) * 32, "cep-%d.dat", i + 1);
      
      saida = fopen(nomeArq, "wb");
      
      tamanhoSaida = qtdRegistros/numArq;
      
      e = (Endereco*) malloc(tamanhoSaida * sizeof(Endereco));
      
      fread(e, sizeof(Endereco), tamanhoSaida, entrada);

      qsort(e, tamanhoSaida, sizeof(Endereco), compara);
      
      fwrite(e, sizeof(Endereco), tamanhoSaida, saida);
    }

    free(e);
    fclose(saida);
  }
}

char *intercala(int numArq, char *arq1, int arq2) {
  char nomeArq[32];
  
  snprintf(nomeArq, sizeof(char) * 32, "%s-%d", arq1, arq2);
  
  if (arq2 <= numArq) {
    char nomeArqEntradaA[32];
    char nomeArqEntradaB[32];
    char nomeArqSaida[32];
    
    FILE *saida, *entradaA, *entradaB;
    Endereco ea, eb;
    
    snprintf(nomeArqEntradaA, sizeof(char) * 32, "cep-%s.dat", arq1);
    snprintf(nomeArqEntradaB, sizeof(char) * 32, "cep-%d.dat", arq2);
    snprintf(nomeArqSaida, sizeof(char) * 32, "cep-%s.dat", nomeArq);

    entradaA = fopen(nomeArqEntradaA, "rb");
    entradaB = fopen(nomeArqEntradaB, "rb");
    saida = fopen(nomeArqSaida, "wb");

    fread(&ea, sizeof(Endereco), 1, entradaA);
	  fread(&eb, sizeof(Endereco), 1, entradaB);

    while(!feof(entradaA) && !feof(entradaB)) {
  		if(compara(&ea,&eb)<0) {
  			fwrite(&ea, sizeof(Endereco), 1, saida);
  			fread(&ea, sizeof(Endereco), 1, entradaA);
  		} else {
			  fwrite(&eb, sizeof(Endereco), 1, saida);
			  fread(&eb, sizeof(Endereco), 1, entradaB);
		  }
	  }

  	while(!feof(entradaA)) {
  		fwrite(&ea, sizeof(Endereco), 1, saida);
  		fread(&ea, sizeof(Endereco), 1, entradaA);		
  	}
    
  	while(!feof(entradaB)) {
  		fwrite(&eb, sizeof(Endereco), 1, saida);
  		fread(&eb, sizeof(Endereco), 1, entradaB);		
  	}

    fclose(entradaA);
    fclose(entradaB);
    fclose(saida);
    
    snprintf(nomeArq, sizeof(char) * 32, "%s", intercala(numArq, nomeArq, arq2 + 1));
  }
  
  return nomeArq;
}

int main(void) {
  FILE *entrada;
  long tamanho, qtdRegistros;
  int n;

  printf("Digite a quantidade de divisões no arquivo: ");
  scanf("%d", &n);

  entrada = fopen("cep.dat", "rb");

  fseek(entrada, 0, SEEK_END);
  
  tamanho = ftell(entrada);
  qtdRegistros = tamanho / sizeof(Endereco);

  rewind(entrada);

  quickSort(qtdRegistros, n, entrada);
  
  fclose(entrada);
  
  intercala(4, "1", 2);
    
  return 0;
}