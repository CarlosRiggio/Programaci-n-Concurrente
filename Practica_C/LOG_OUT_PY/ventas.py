import matplotlib.pyplot as plt
import numpy as np

# Leer el archivo de registro
with open("ventas.log", "r") as f:
    datos = f.readlines()

# Convertir los datos a un formato de lista de tuplas
datos = [tuple(linea.strip().split(",")) for linea in datos]

# Obtener las fechas y las ventas como arreglos NumPy
fechas = np.array([fila[0] for fila in datos])
ventas = np.array([float(fila[1]) for fila in datos])

# Crear la figura y el gráfico
fig, ax = plt.subplots()

# Establecer el título y los ejes de la gráfica
ax.set_title("Ventas diarias")
ax.set_xlabel("Fecha")
ax.set_ylabel("Ventas")

# Graficar los datos
ax.plot(fechas, ventas)

# Rotar las etiquetas del eje x para que sean legibles
plt.xticks(rotation=45)

# Mostrar la gráfica
plt.show()
