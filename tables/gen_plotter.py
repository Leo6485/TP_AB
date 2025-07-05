import pandas as pd
import matplotlib.pyplot as plt
import numpy as np # Importa a biblioteca numpy

plt.style.use('seaborn-v0_8-whitegrid')

df = pd.read_csv('generations.csv')

print(df.head())

dados_agregados = []

for geracao, grupo in df.groupby('Geracao'):
    fitnesses = []
    for i in range(1, 201):
        coluna_fitness = f'Individuo {i}'
        if coluna_fitness in grupo.columns:
            valores_fitness = grupo[coluna_fitness].dropna().tolist()
            fitnesses.extend([valor for valor in valores_fitness if valor > 0])

    if fitnesses:
        media = sum(fitnesses) / len(fitnesses)
        mediana = sorted(fitnesses)[len(fitnesses) // 2]
        melhor = min(fitnesses)
        pior = max(fitnesses)
        desvio_padrao = np.std(fitnesses) # Calcula o desvio padrão
        dados_agregados.append([geracao, media, mediana, melhor, pior, desvio_padrao])
    else:
        dados_agregados.append([geracao, float('nan'), float('nan'), float('nan'), float('nan'), float('nan')])

df_agregado = pd.DataFrame(dados_agregados, columns=['Geração', 'Custo Médio', 'Custo Mediano', 'Menor Custo', 'Maior Custo', 'Desvio Padrão'])

geracoes = df_agregado['Geração']

# Plotando o gráfico
plt.figure(figsize=(12, 6))
plt.plot(geracoes, df_agregado['Custo Médio'], label='Custo Médio', color='orange')
plt.plot(geracoes, df_agregado['Custo Mediano'], label='Custo Mediano', color='yellow', linestyle='--')
plt.plot(geracoes, df_agregado['Menor Custo'], label='Menor Custo', color='green')
plt.plot(geracoes, df_agregado['Maior Custo'], label='Maior Custo', color='red')
#plt.plot(geracoes, df_agregado['Desvio Padrão'], label='Desvio Padrão', color='blue') # Adiciona o desvio padrão

# Personalização do gráfico
plt.title('Evolução do Fitness das Formigas')
plt.xlabel('Geração')
plt.ylabel('Valor do Desvio Padrão')
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.show()