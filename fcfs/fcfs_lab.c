#include <stdio.h>
#include <stdlib.h>

// Estructura de un proceso - NO EXPROPIATIVO
typedef struct {
    int id;
    int tiempo_llegada; // AT (Arrival Time)
    int rafaga_ejecucion; // BT (Burst Time)
    int tiempo_finalizacion; // CT (Completion Time)
    int tiempo_retorno; // TAT (Turnaround Time): CT - AT
    int tiempo_espera; // WT (Waiting Time): TAT - BT
    int tiempo_respuesta; // RT (Response Time): Inicio - AT
} Proceso;

// Ordenar por tiempo de llegada
int comparar_procesos(const void *a, const void *b) {
    Proceso *p1 = (Proceso *)a;
    Proceso *p2 = (Proceso *)b;
    if (p1->tiempo_llegada != p2->tiempo_llegada)
        return p1->tiempo_llegada - p2->tiempo_llegada;
    return p1->id - p2->id;
}

// Simulación FCFS
void simular_fcfs(Proceso *procesos, int n) {
    int tiempo_actual = 0;
    float total_espera = 0, total_retorno = 0, total_respuesta = 0;

    qsort(procesos, n, sizeof(Proceso), comparar_procesos);

    printf("\n--- Diagrama de Gantt y Cálculos ---\nTiempo:\n");

    for (int i = 0; i < n; i++) {
        int tiempo_inicio_ejecucion;

        if (tiempo_actual < procesos[i].tiempo_llegada) {
            printf("%d - [IDLE: %d] - ", tiempo_actual, procesos[i].tiempo_llegada);
            tiempo_inicio_ejecucion = procesos[i].tiempo_llegada;
        } else {
            tiempo_inicio_ejecucion = tiempo_actual;
        }

        procesos[i].tiempo_finalizacion = tiempo_inicio_ejecucion + procesos[i].rafaga_ejecucion;
        procesos[i].tiempo_retorno = procesos[i].tiempo_finalizacion - procesos[i].tiempo_llegada;
        procesos[i].tiempo_espera = procesos[i].tiempo_retorno - procesos[i].rafaga_ejecucion;
        procesos[i].tiempo_respuesta = tiempo_inicio_ejecucion - procesos[i].tiempo_llegada;

        printf("%d - [P%d: %d] - ", tiempo_inicio_ejecucion, procesos[i].id, procesos[i].tiempo_finalizacion);
        tiempo_actual = procesos[i].tiempo_finalizacion;

        total_retorno += procesos[i].tiempo_retorno;
        total_espera += procesos[i].tiempo_espera;
        total_respuesta += procesos[i].tiempo_respuesta;
    }
    printf("%d\n", tiempo_actual);

    printf("\n------------------ TABLA DE RESULTADOS FCFS ------------------\n");
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
    printf("Promedio TAT: %.2f\n", total_retorno / n);
    printf("Promedio WT : %.2f\n", total_espera / n);
    printf("Promedio RT : %.2f\n", total_respuesta / n);
}

int main() {
    FILE *archivo = fopen("../procesos.txt", "r"); // "../" porque procesos.txt está en la raíz
    if (archivo == NULL) {
        perror("Error al abrir procesos.txt");
        return 1;
    }

    Proceso *procesos = NULL;
    int n = 0;
    int id, at, bt;

    while (fscanf(archivo, "%d %d %d", &id, &at, &bt) == 3) {
        n++;
        procesos = realloc(procesos, n * sizeof(Proceso));
        procesos[n - 1].id = id;
        procesos[n - 1].tiempo_llegada = at;
        procesos[n - 1].rafaga_ejecucion = bt;
    }

    fclose(archivo);

    if (n > 0) simular_fcfs(procesos, n);
    else printf("No se encontraron procesos.\n");

    free(procesos);
    return 0;
}
