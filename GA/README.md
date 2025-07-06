# Algoritmo Genético para Resolver o Problema do Caixeiro Viajante (AG)

Grupo: Leonardo Guimar

Este projeto implementa um algoritmo genético com representação binária para encontrar uma solução aproximada do **Problema do Caixeiro Viajante (AG)**. O objetivo é selecionar a melhor combinação de arestas a serem utilizadas para visitar todos os vértices exatamente uma vez e retornar para o vértice inicial, de forma a minimizar o custo total das arestas utilizadas.

## Compilação e Execução

A compilação do programa é feita via `Makefile`, que gera um executável chamado `./ag`.

### # Exemplo de Compilação
``` bash
make
```

A execução permite escolher entre diferentes variações do algoritmo, usando a flag `-s` seguida do **ID da solução** desejada. Se nenhum ID for especificado, a **solução padrão (ID 1)** é executada.

Além disso, é possível selecionar qual maneira de seleção de pais e de cruzamento serão utilizados. Para identificar a forma de seleção desejada, o **ID da seleção** deverá ser precedido pela flag `-p`. Já para identificar a forma de cruzamento desejada, o **ID do cruzamento** deverá ser precedido pela flag `-c`.

### ID das Soluções Disponíveis
- `1`: solução básica (padrão)
- `2`: solução que aplica um teste fatorial nos parâmetros do algoritmo genético e salva os resultados em um arquivo `.csv`.
- `3`: solução que armazena os valores de fitness dos indivíduos em um arquivo `.csv`.

### ID das Seleções de Pais Possíveis
- `1`: seleção por torneio (padrão).
- `2`: seleção por roleta.

### ID dos Cruzamentos Disponíveis
- `1`: cruzamento OX (Ordered Crossover) (padrão).
- `2`: cruzamento PMX (Partially Mapped Crossover).

Além disso, é possível especificar os arquivos de entrada com as especificações do problema. Para mais informações, consulte a seção [Entrada](#entrada).

### Exemplo de Execução
```bash
./ag -i sbg128_dist.txt -c 2 -p 2 -s 1
```

## Entrada e Saída
### Entrada

O arquivo de entrada deve estar localizado na pasta `inputs/`, localizado na raíz do projeto. Por padrão, é assumido o arquivo de entrada: `lau15_dist.txt`.

Este arquivo pode ser substituído. Para isso, o nome do novo arquivo deve ser precedido pela flag `-i`.

Para um exemplo de uso desta flag, vá para a seção [Exemplo de Execução](#exemplo-de-execucao)

### Saida

A saída depende da solução escolhida:
- **Solução 1 (básica)**: imprime na saída padrão o fitness mínimo encontrado e a sequência de visitação das cidades (vértices);
- **Solução 2 (teste fatorial)**: salva os resultados de cada conjunto de parâmetros no arquivo `data/results.csv`;
- **Solução 3 (dados sobre as gerações)**: salva o fitness de cada geração no arquivo `data/generations.csv`.

### Exemplo de Saída do Programa
```
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                  Resumo dos Resultados
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Menor distancia: 291
 Caminho: 14 -> 8 -> 4 -> 6 -> 2 -> 11 -> 13 -> 9 -> 7 -> 5 -> 3 -> 10 -> 0 -> 12 -> 1 -> 14
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
```

## Observações

- Os parâmetros para testes fatoriais são definidos diretamente no código fonte. Ajuste como necessário antes de compilar.
- Certifique-se que os arquivos de entrada estão corretamente formatados, como nos arquivos disponibilizados em [CITIES](https://people.sc.fsu.edu/~jburkardt/datasets/cities/cities.html)

