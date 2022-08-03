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

void ordenacao(long, int, FILE *);
void intercala(char *, char *, char *);
void teste(char *);
char *criaNomeArquivosIntercala(int);

int main(void) {
  FILE *entrada;
  long tamanho, qtdRegistros;
  int qtdArq;
  char nomeArqResultante[128];

  printf("Digite a quantidade de divisões no arquivo: ");
  scanf("%d", &qtdArq);

  if (qtdArq % 2 != 0) {
    printf("Qtd de arquivos precisa ser um número par");
    return 0;
  }

  entrada = fopen("cep.dat", "rb");

  fseek(entrada, 0, SEEK_END);

  tamanho = ftell(entrada);
  qtdRegistros = tamanho / sizeof(Endereco);

  rewind(entrada);

  ordenacao(qtdRegistros, qtdArq, entrada);

  fclose(entrada);

  sprintf(nomeArqResultante, "%s", criaNomeArquivosIntercala(qtdArq));

  teste(nomeArqResultante);

  return 0;
}

int compara(const void *e1, const void *e2) {
  return strncmp(((Endereco *)e1)->cep, ((Endereco *)e2)->cep, 8);
}

void ordenacao(long qtdRegistros, int numArq, FILE *entrada) {
  int tamanhoSaida;
  FILE *saida;
  char nomeArq[32];
  int i;

  for (i = 0; i < numArq; i++) {
    Endereco *e;

    if (i == 0) {
      snprintf(nomeArq, sizeof(char) * 32, "cep-%d.dat", i + 1);

      saida = fopen(nomeArq, "wb");

      tamanhoSaida = ((qtdRegistros / numArq) + (qtdRegistros % numArq));

      e = (Endereco *)malloc(tamanhoSaida * sizeof(Endereco));

      fread(e, sizeof(Endereco), tamanhoSaida, entrada);

      qsort(e, tamanhoSaida, sizeof(Endereco), compara);

      fwrite(e, sizeof(Endereco), tamanhoSaida, saida);
    } else {
      snprintf(nomeArq, sizeof(char) * 32, "cep-%d.dat", i + 1);

      saida = fopen(nomeArq, "wb");

      tamanhoSaida = qtdRegistros / numArq;

      e = (Endereco *)malloc(tamanhoSaida * sizeof(Endereco));

      fread(e, sizeof(Endereco), tamanhoSaida, entrada);

      qsort(e, tamanhoSaida, sizeof(Endereco), compara);

      fwrite(e, sizeof(Endereco), tamanhoSaida, saida);
    }

    free(e);
    fclose(saida);
  }
}

char *criaNomeArquivosIntercala(int qtdArquivos) {
  int i, j;
  int n = 1, passos = 1, controle = 1;
  char arq1[128] = "", arq2[128] = "";

  for (i = 1; i < ((qtdArquivos * 2) - 1); i++) {
    int pos = 0;

    for (j = 0; j < passos; j++) {
      if (controle == 1) {
        pos += sprintf(&arq1[pos], "%d", n);

        if (j < passos - 1) {
          pos += sprintf(&arq1[pos], "-");
        }
      } else {
        pos += sprintf(&arq2[pos], "%d", n);

        if (j < passos - 1) {
          pos += sprintf(&arq2[pos], "-");
        }
      }

      if (n == qtdArquivos) {
        passos = passos * 2;
        n = 1;
        break;
      }

      n++;
    }

    if (controle == 2) {
      char nomeArqA[128], nomeArqB[128], nomeArqSaida[128];

      sprintf(&nomeArqA[0], "cep-%s.dat", arq1);
      sprintf(&nomeArqB[0], "cep-%s.dat", arq2);
      sprintf(&nomeArqSaida[0], "cep-%s-%s.dat", arq1, arq2);

      controle = 0;

      intercala(nomeArqA, nomeArqB, nomeArqSaida);

      if (i == ((qtdArquivos * 2) - 2)) {
        return nomeArqSaida;
      }

      strcpy(arq1, "");
      strcpy(arq2, "");
    }

    controle++;
  }

  return "";
}

void intercala(char *nomeArqEntradaA, char *nomeArqEntradaB,
               char *nomeArqSaida) {

  FILE *saida, *entradaA, *entradaB;
  Endereco ea, eb;

  entradaA = fopen(nomeArqEntradaA, "rb");
  entradaB = fopen(nomeArqEntradaB, "rb");
  saida = fopen(nomeArqSaida, "wb");

  fread(&ea, sizeof(Endereco), 1, entradaA);
  fread(&eb, sizeof(Endereco), 1, entradaB);

  while (!feof(entradaA) && !feof(entradaB)) {
    if (compara(&ea, &eb) < 0) {
      fwrite(&ea, sizeof(Endereco), 1, saida);
      fread(&ea, sizeof(Endereco), 1, entradaA);
    } else {
      fwrite(&eb, sizeof(Endereco), 1, saida);
      fread(&eb, sizeof(Endereco), 1, entradaB);
    }
  }

  while (!feof(entradaA)) {
    fwrite(&ea, sizeof(Endereco), 1, saida);
    fread(&ea, sizeof(Endereco), 1, entradaA);
  }

  while (!feof(entradaB)) {
    fwrite(&eb, sizeof(Endereco), 1, saida);
    fread(&eb, sizeof(Endereco), 1, entradaB);
  }

  fclose(entradaA);
  fclose(entradaB);

  remove(nomeArqEntradaA);
  remove(nomeArqEntradaB);

  fclose(saida);
}

void teste(char *nomeArqResultante) {
  FILE *entrada;
  Endereco e;
  int i;

  entrada = fopen(nomeArqResultante, "rb");

  fread(&e, sizeof(Endereco), 1, entrada);

  for (i = 0; i < 50; i++) {
    printf("\n%.8s", e.cep);
    fread(&e, sizeof(Endereco), 1, entrada);
  }

  fclose(entrada);
}