# CEFET-Arquivos-C
Repositório criado para a matéria de Organização e Estrutura de Arquivos - CEFET/RJ.


### Busca Binária
Arquivo: `AchaCep.C`

Finalidade: Achar um cep através da busca binária.

Utilização: Na mesma pasta do arquivo "AchaCep.C", precisa existir um arquivo "cep_ordenado.dat" contendo os valores de todos os CEPs ordenados, que será usado para realizar a busca. O programa recebe como argumento o cep que será buscado.


### Ordenação Externa
**Arquivo**: `ExternalSort.c`

**Finalidade**: Realizar ordenação externa no arquivo "cep.dat".

**Utilização**: Na mesma pasta do arquivo "ExternalSort.c", precisa existir um arquivo "cep.data" contendo os valores de todos os CEPs, que será utilizado para fazer a ordenação externa. Na execuão do programa o usuário precisará informar por linha de comando a quantidade de arquivo serão gerados para divisão do arquivo principal, e para o programa funcionar de maneira adquada, o valor precisa ser potencia de 2.

**Resultado**: Na finalização da execução o programa gera como resultado um único arquivo, apagando os arquivos temporários e imprimindo em tela os primeiros valores da lista ordenada, para um teste rápido.
