#include <stdio.h>
#include <stdlib.h>

// Estructura de un proceso - NO EXPROPIATIVO
typedef struct {
    int id;
    int tiempo_llegada;       // AT
    int rafaga_ejecucion;     // BT
    int tiempo_finalizacion;  // CT
    int tiempo_retorno;       // TAT = CT - AT
    int tiempo_espera;        // WT = TAT - BT
    int tiempo_respuesta;     // RT = inicio - AT
    int completado;           // bandera (0=no, 1=sí)
} Proceso;

// Encuentra el proceso disponible con la ráfaga más corta
int encontrar_proceso_sjf(Proceso *procesos, int n, int tiempo_actual) {
    int idx = -1;
    int rafaga_minima = 1e9;

    for (int i = 0; i < n; i++) {
        if (!procesos[i].completado && procesos[i].tiempo_llegada <= tiempo_actual) {
            if (procesos[i].rafaga_ejecucion < rafaga_minima) {
                rafaga_minima = procesos[i].rafaga_ejecucion;
                idx = i;
            }
            // Desempate por tiempo de llegada o ID
            else if (procesos[i].rafaga_ejecucion == rafaga_minima && procesos[i].tiempo_llegada < procesos[idx].tiempo_llegada) {
                idx = i;
            }
        }
    }

    return idx;
}

void simular_sjf(Proceso *procesos, int n) {
    int completados = 0;
    int tiempo_actual = 0;
    float total_espera = 0, total_retorno = 0, total_respuesta = 0;

    printf("\n--- Diagrama de Gantt y Cálculos (SJF) ---\nTiempo:\n");

    while (completados < n) {
        int idx = encontrar_proceso_sjf(procesos, n, tiempo_actual);

        if (idx == -1) {
            // No hay proceso disponible aún
            printf("%d - [IDLE] - ", tiempo_actual);
            tiempo_actual++;
            continue;
        }

        int inicio = tiempo_actual;
        tiempo_actual += procesos[idx].rafaga_ejecucion;
        procesos[idx].tiempo_finalizacion = tiempo_actual;
        procesos[idx].tiempo_retorno = procesos[idx].tiempo_finalizacion - procesos[idx].tiempo_llegada;
        procesos[idx].tiempo_espera = procesos[idx].tiempo_retorno - procesos[idx].rafaga_ejecucion;
        procesos[idx].tiempo_respuesta = inicio - procesos[idx].tiempo_llegada;
        procesos[idx].completado = 1;
        completados++;

        printf("%d - [P%d: %d] - ", inicio, procesos[idx].id, tiempo_actual);

        total_retorno += procesos[idx].tiempo_retorno;
        total_espera += procesos[idx].tiempo_espera;
        total_respuesta += procesos[idx].tiempo_respuesta;
    }

    printf("%d\n", tiempo_actual);

    printf("\n------------------ TABLA DE RESULTADOS SJF ------------------\n");
    printf("| ID | AT | BT | CT | TAT | WT | RT |\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("| %2d | %2d | %2d | %2d | %3d | %2d | %2d |\n",
               procesos[i].id,
               procesos[i].tiempo_llegada,
               procesos[i].rafaga_ejecucion,
               procesos[i].tiempo_finalizacion,
               procesos[i].tiempo_retorno,
               procesos[i].tiempo_espera,
               procesos[i].tiempo_respuesta);
    }

    printf("-------------------------------------------------------------\n");
    printf("Promedio TAT: %.2f\n", total_retorno / n);
    printf("Promedio WT : %.2f\n", total_espera / n);
    printf("Promedio RT : %.2f\n", total_respuesta / n);
}

int main() {
    FILE *archivo = fopen("../procesos.txt", "r");
    if (archivo == NULL) {
        perror("Error al abrir procesos.txt");
        return 1;
    }

    Proceso *procesos = NULL;
    int n = 0, id, at, bt;

    while (fscanf(archivo, "%d %d %d", &id, &at, &bt) == 3) {
        n++;
        procesos = realloc(procesos, n * sizeof(Proceso));
        procesos[n - 1].id = id;
        procesos[n - 1].tiempo_llegada = at;
        procesos[n - 1].rafaga_ejecucion = bt;
        procesos[n - 1].completado = 0;
    }
    fclose(archivo);

    if (n > 0) simular_sjf(procesos, n);
    else printf("No se encontraron procesos.\n");

    free(procesos);
    return 0;
}
