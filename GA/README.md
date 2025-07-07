# Algoritmo Genético para Resolver o Problema de Agendamento de Tarefas em Linha (FSSP)

Grupo: Leonardo Guimarães de Oliveira, Guilherme Souza Barros, Vítor Oliveira Amorim.
Disciplina: Algoritmos Bioinspirados
Professora: Carolina Ribeiro Xavier

## Sumário
- [Introdução](#introdução)
- [Compilação e Execução](#compilação-e-execução)
    - [Exemplo de Compilação](#exemplo-de-compilação)
    - [ID das Soluções Disponíveis](#id-das-soluções-disponíveis)
    - [ID das Seleções de Pais Disponíveis](#id-das-seleções-de-pais-possíveis)
    - [ID dos Cruzamentos Disponíveis](#id-dos-cruzamentos-disponíveis)
    - [Exemplo de Execução](#exemplo-de-execução)
- [Entrada e Saída](#entrada-e-saída)
    - [Entrada](#entrada)
    - [Saída](#saida)
    - [Exemplo de Saída](#exemplo-de-saída-do-programa)
- [Observações](#observações)

## Introdução

Este trabalho implementa um Algoritmo Genético (AG) para encontrar uma solução aproximada para o **Problema de Agendamento de Tarefas em Linha (Flow Shop Scheduling Problem - FSSP)**.

O FSSP consiste em um conjunto de *N* tarefas que devem ser processadas em um conjunto de *M* máquinas. Todas as tarefas devem passar por todas as *M* máquinas na mesma ordem. O objetivo é determinar a ordem das tarefas para todas as máquinas que minimize o *makespan*, ou seja, o tempo total para que todas as tarefas sejam concluídas.

## Compilação e Execução

A compilação do programa é feita via `Makefile`, que gera um executável chamado `./ag`.

### Exemplo de Compilação
``` bash
make
```

Caso seja necessário apagar os arquivos binários do programa, basta utilizar o comando `make clean`.

A execução permite escolher entre diferentes variações do algoritmo, usando a flag `-s` seguida do **ID da solução** desejada. Se nenhum ID for especificado, a **solução padrão (ID 1)** é executada.

Além disso, é possível selecionar qual maneira de seleção de pais e de cruzamento serão utilizados. Para identificar a forma de seleção desejada, o **ID da seleção** deverá ser precedido pela flag `-p`. Já para identificar a forma de cruzamento desejada, o **ID do cruzamento** deverá ser precedido pela flag `-c`.

### ID das Soluções Disponíveis
- `1`: solução simples (padrão)
- `2`: solução que aplica um teste fatorial nos parâmetros do algoritmo genético e salva os resultados em um arquivo `.csv`.
- `3`: solução que armazena os valores de fitness dos indivíduos em um arquivo `.csv`.

### ID das Seleções de Pais Possíveis
- `1`: seleção por torneio (padrão).
- `2`: seleção por roleta.

Dentre as formas de seleção de pais, foi observado que a seleção por torneio apresentou os melhores resultados.

### ID dos Cruzamentos Disponíveis
- `1`: cruzamento PMX (Partially Mapped Crossover) (padrão).
- `2`: cruzamento OX (Ordered Crossover).

Dentre as formas de cruzamento implementadas, foi observado que o cruzamento PMX apresentou os melhores resultados.

Além disso, é possível especificar o **ID do arquivo de entrada**, utilizando a flag `-i`. Para mais informações, consulte a seção [Entrada](#entrada).

### Exemplo de Execução
```bash
./ag -i 1 -c 1 -p 1 -s 1
```

## Entrada e Saída
### Entrada

O arquivo de entrada deve estar localizado na pasta `inputs/`, localizado na raíz do projeto. Os arquivos de entrada devem seguir o formato `fssp_instance_XX.txt`. Por padrão, é assumido o arquivo de entrada: `fssp_instance_01.txt`.

Este arquivo pode ser substituído. Para isso, o *ID* do arquivo de entrada deve ser precedido pela flag `-i`.

Para um exemplo de uso desta flag, vá para a seção [Exemplo de Execução](#exemplo-de-execucao)

### Saida

O arquivo de saída terá nome `resultado_instancia_XX.txt`, onde *XX* será substituído pelo *ID* do arquivo de entrada executado.

A saída depende da solução escolhida:
- **Solução 1 (básica)**: ;
- **Solução 2 (teste fatorial)**: salva os resultados de cada conjunto de parâmetros no arquivo `data/tables/results.csv`;
- **Solução 3 (dados sobre as gerações)**: salva o fitness de cada geração no arquivo `data/tables/generations.csv`.

### Exemplo de Saída do Programa
```
Melhor Makespan: 903
Melhor Sequencia: 0 6 8 3 23 26 12 27 2 1 28 11 24 9 10 16 19 7 25 18 13 20 15 21 4 5 14 22 29 17
Tempo de Execucao (segundos): 40.535916
```

## Observações

- Os parâmetros para testes fatoriais são definidos diretamente no código fonte. Ajuste como necessário antes de compilar.