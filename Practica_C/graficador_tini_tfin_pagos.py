import pandas as pd
import matplotlib.pyplot as plt

# Leer el archivo de log y crear un dataframe de pandas
df = pd.read_csv('tiempo_ini_fin_pagos.log', delimiter=' ', header=None, names=['ID', 'Duracion'])

# Convertir la duración de las transacciones a microsegundos
df['Duracion'] = df['Duracion'] * 1000

# Graficar los datos en un gráfico de dispersión
plt.scatter(df['ID'], df['Duracion'])

# Establecer etiquetas en los ejes
plt.xlabel('Número de identificación')
plt.ylabel('Duración de la transacción (microsegundos)')

# Mostrar el gráfico
plt.show()
