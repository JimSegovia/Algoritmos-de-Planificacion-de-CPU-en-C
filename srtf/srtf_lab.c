#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Estructura del proceso - EXPROPIATIVO
typedef struct {
    int id;
    int tiempo_llegada;      // AT
    int rafaga_ejecucion;    // BT
    int restante;            // Tiempo restante
    int tiempo_finalizacion; // CT
    int tiempo_retorno;      // TAT = CT - AT
    int tiempo_espera;       // WT = TAT - BT
    int tiempo_respuesta;    // RT = Primer inicio - AT
    int iniciado;            // Flag si ya comenzó
} Proceso;

// Simulación SRTF (Shortest Remaining Time First)
void simular_srtf(Proceso *procesos, int n) {
    int completados = 0, tiempo_actual = 0, menor_restante;
    int proceso_actual = -1;
    float total_TAT = 0, total_WT = 0, total_RT = 0;
    int ultimo_tiempo = 0;

    printf("\n--- Diagrama de Gantt y Cálculos ---\nTiempo:\n");

    while (completados < n) {
        menor_restante = INT_MAX;
        proceso_actual = -1;

        // Buscar el proceso con menor tiempo restante disponible en este momento
        for (int i = 0; i < n; i++) {
            if (procesos[i].tiempo_llegada <= tiempo_actual && procesos[i].restante > 0) {
                if (procesos[i].restante < menor_restante) {
                    menor_restante = procesos[i].restante;
                    proceso_actual = i;
                }
            }
        }

        // Si no hay procesos listos
        if (proceso_actual == -1) {
            printf("%d - [IDLE] - ", tiempo_actual);
            tiempo_actual++;
            continue;
        }

        // Si el proceso empieza por primera vez
        if (!procesos[proceso_actual].iniciado) {
            procesos[proceso_actual].iniciado = 1;
            procesos[proceso_actual].tiempo_respuesta = tiempo_actual - procesos[proceso_actual].tiempo_llegada;
        }

        // Mostrar cambio de proceso en el diagrama de Gantt
        if (ultimo_tiempo != proceso_actual) {
            printf("%d - [P%d] - ", tiempo_actual, procesos[proceso_actual].id);
            ultimo_tiempo = proceso_actual;
        }

        // Ejecutar 1 unidad de tiempo
        procesos[proceso_actual].restante--;
        tiempo_actual++;

        // Si el proceso termina
        if (procesos[proceso_actual].restante == 0) {
            procesos[proceso_actual].tiempo_finalizacion = tiempo_actual;
            procesos[proceso_actual].tiempo_retorno =
                procesos[proceso_actual].tiempo_finalizacion - procesos[proceso_actual].tiempo_llegada;
            procesos[proceso_actual].tiempo_espera =
                procesos[proceso_actual].tiempo_retorno - procesos[proceso_actual].rafaga_ejecucion;

            total_TAT += procesos[proceso_actual].tiempo_retorno;
            total_WT += procesos[proceso_actual].tiempo_espera;
            total_RT += procesos[proceso_actual].tiempo_respuesta;
            completados++;
        }
    }
    printf("%d\n", tiempo_actual);

    // Tabla de resultados
    printf("\n------------------ TABLA DE RESULTADOS SRTF ------------------\n");
    printf("| ID | AT | BT | CT | TAT | WT | RT |\n");
    printf("--------------------------------------------------------------\n");
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

    printf("--------------------------------------------------------------\n");
    printf("Promedio TAT: %.2f\n", total_TAT / n);
    printf("Promedio WT : %.2f\n", total_WT / n);
    printf("Promedio RT : %.2f\n", total_RT / n);
}

int main() {
    FILE *archivo = fopen("../procesos.txt", "r");
    if (!archivo) {
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
        procesos[n - 1].restante = bt;
        procesos[n - 1].iniciado = 0;
    }

    fclose(archivo);

    if (n > 0) simular_srtf(procesos, n);
    else printf("No se encontraron procesos.\n");

    free(procesos);
    return 0;
}
