
# 🧠 Planificación de Procesos - Simulador en C

Este repositorio contiene la implementación de **cuatro algoritmos clásicos de planificación de procesos** en C, con estructura uniforme, lectura desde archivo, y visualización de resultados mediante tabla y diagrama de Gantt.

Cada carpeta contiene un programa independiente que:
- Lee los procesos desde `procesos.txt`
- Ejecuta la simulación del algoritmo correspondiente
- Muestra:
  - El **diagrama de Gantt**
  - La **tabla de resultados** (CT, TAT, WT, RT)
  - Los **promedios finales** de cada métrica

---

## ⚙️ Compilación y Ejecución

Puedes ejecutar cada simulador de dos formas: desde **CLion** o desde **Git Bash** (en Windows).

---

### 🧩 Opción 1: Usar CLion (Recomendada)

1. Abre el proyecto en CLion.  
2. Selecciona la carpeta del algoritmo (por ejemplo `fcfs/`) y abre `main.c`.  
3. Pulsa **Run ▶️** para compilar y ejecutar.  
4. Asegúrate de que `procesos.txt` está en la raíz del proyecto (`../procesos.txt` en el código).

---

### 💻 Opción 2: Usar Git Bash

Si tienes **Git Bash** instalado, puedes compilar y ejecutar desde terminal con los mismos comandos de Linux:

#### 🔹 FCFS
```bash
cd fcfs
gcc main.c -o fcfs_sim
./fcfs_sim ../procesos.txt
````

#### 🔹 SJF

```bash
cd ../sjf
gcc main.c -o sjf_sim
./sjf_sim ../procesos.txt
```

#### 🔹 SRTF

```bash
cd ../srtf
gcc main.c -o srtf_sim
./srtf_sim ../procesos.txt
```

#### 🔹 Round Robin

```bash
cd ../rr
gcc main.c -o rr_sim
./rr_sim ../procesos.txt
```

> ⚙️ En Windows CMD o PowerShell, reemplaza `./nombre` por `nombre.exe`.
> Ejemplo: `rr_sim.exe ../procesos.txt`

---

## 🕑 Parámetros y detalles extra

### 🔸 Quantum (Round Robin)

El **quantum** puede modificarse directamente en el código fuente (`main.c` dentro de `rr/`):

```c
int quantum = 2; // Cambia este valor según desees
```

---

## 📊 Métricas mostradas

Cada simulador calcula y muestra los siguientes valores:

| Métrica | Descripción                              | Fórmula       |
| ------- | ---------------------------------------- | ------------- |
| **CT**  | Tiempo de finalización (Completion Time) | —             |
| **TAT** | Tiempo de retorno (Turnaround Time)      | `CT - AT`     |
| **WT**  | Tiempo de espera (Waiting Time)          | `TAT - BT`    |
| **RT**  | Tiempo de respuesta (Response Time)      | `Inicio - AT` |

---

## 🧩 Créditos

**Autor:** Jim Bryan Jordan Segovia Valencia
**Universidad:** Universidad Nacional Mayor de San Marcos (UNMSM)
**Curso:** Sistemas Operativos
**Tema:** Algoritmos de Planificación de Procesos

