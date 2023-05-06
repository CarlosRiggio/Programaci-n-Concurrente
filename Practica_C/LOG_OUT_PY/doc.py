import pandas as pd
import matplotlib.pyplot as plt

# Cargar datos desde el archivo CSV
data = pd.read_csv('doc.csv')

# Calcular la diferencia entre las columnas T_Out y T_In
data['T_Dif'] = pd.to_datetime(data['T_Out']) - pd.to_datetime(data['T_In'])

# Crear una figura y un eje
fig, ax = plt.subplots()

# Crear la gráfica de barras
ax.bar(data['Tipo_Proceso'], data['T_Dif'].dt.total_seconds())

# Establecer los títulos de la gráfica y los ejes
ax.set_title('Tiempo de procesamiento por tipo de proceso')
ax.set_xlabel('Tipo de proceso')
ax.set_ylabel('Tiempo (segundos)')

# Mostrar la gráfica
plt.show()
