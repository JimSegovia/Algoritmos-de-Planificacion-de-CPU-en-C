#include <stdio.h>
#include <stdlib.h>

// Estructura de un proceso - EXPROPIATIVO (Round Robin)
typedef struct {
    int id;
    int tiempo_llegada;       // AT (Arrival Time)
    int rafaga_ejecucion;     // BT (Burst Time)
    int tiempo_restante;      // BT restante
    int tiempo_finalizacion;  // CT (Completion Time)
    int tiempo_retorno;       // TAT = CT - AT
    int tiempo_espera;        // WT = TAT - BT
    int tiempo_respuesta;     // RT = primera ejecución - AT
    int iniciado;             // bandera para RT (0=no, 1=sí)
    int completado;           // bandera (0=no, 1=sí)
} Proceso;

// Ordenar por tiempo de llegada (para inicializar)
int comparar_procesos(const void *a, const void *b) {
    Proceso *p1 = (Proceso *)a;
    Proceso *p2 = (Proceso *)b;
    if (p1->tiempo_llegada != p2->tiempo_llegada)
        return p1->tiempo_llegada - p2->tiempo_llegada;
    return p1->id - p2->id;
}

void simular_rr(Proceso *procesos, int n, int quantum) {
    int tiempo_actual = 0;
    int completados = 0;
    float total_espera = 0, total_retorno = 0, total_respuesta = 0;

    // Orden inicial por tiempo de llegada
    qsort(procesos, n, sizeof(Proceso), comparar_procesos);

    // Cola de ejecución
    int *cola = malloc(n * sizeof(int));
    int frente = 0, fin = 0;

    printf("\n--- Diagrama de Gantt y Cálculos (Round Robin, Quantum = %d) ---\nTiempo:\n", quantum);

    // Agregar el primer proceso disponible
    cola[fin++] = 0;

    while (completados < n) {
        if (frente == fin) {
            // CPU idle hasta que llegue un proceso
            int proximo_tiempo = 1e9;
            for (int i = 0; i < n; i++)
                if (!procesos[i].completado && procesos[i].tiempo_llegada < proximo_tiempo)
                    proximo_tiempo = procesos[i].tiempo_llegada;

            if (proximo_tiempo > tiempo_actual) {
                printf("%d - [IDLE: %d] - ", tiempo_actual, proximo_tiempo);
                tiempo_actual = proximo_tiempo;
            }

            // Añadir proceso disponible a la cola
            for (int i = 0; i < n; i++)
                if (!procesos[i].completado && procesos[i].tiempo_llegada <= tiempo_actual) {
                    int ya_en_cola = 0;
                    for (int j = frente; j < fin; j++)
                        if (cola[j] == i) ya_en_cola = 1;
                    if (!ya_en_cola)
                        cola[fin++] = i;
                }
            continue;
        }

        int idx = cola[frente++];
        Proceso *p = &procesos[idx];

        // Registrar tiempo de respuesta si aún no ha iniciado
        if (!p->iniciado) {
            p->tiempo_respuesta = tiempo_actual - p->tiempo_llegada;
            p->iniciado = 1;
        }

        // Determinar cuánto ejecuta
        int ejecucion = (p->tiempo_restante > quantum) ? quantum : p->tiempo_restante;

        printf("%d - [P%d: %d] - ", tiempo_actual, p->id, tiempo_actual + ejecucion);
        tiempo_actual += ejecucion;
        p->tiempo_restante -= ejecucion;

        // Añadir nuevos procesos que llegaron durante este intervalo
        for (int i = 0; i < n; i++)
            if (!procesos[i].completado && procesos[i].tiempo_llegada <= tiempo_actual) {
                int ya_en_cola = 0;
                for (int j = frente; j < fin; j++)
                    if (cola[j] == i) ya_en_cola = 1;
                if (!ya_en_cola)
                    cola[fin++] = i;
            }

        // Si el proceso terminó
        if (p->tiempo_restante == 0) {
            p->completado = 1;
            completados++;
            p->tiempo_finalizacion = tiempo_actual;
            p->tiempo_retorno = p->tiempo_finalizacion - p->tiempo_llegada;
            p->tiempo_espera = p->tiempo_retorno - p->rafaga_ejecucion;
        } else {
            // Reenviar a la cola
            cola[fin++] = idx;
        }
    }

    printf("%d\n", tiempo_actual);

    printf("\n------------------ TABLA DE RESULTADOS RR ------------------\n");
    printf("| ID | AT | BT | CT | TAT | WT | RT |\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("| %2d | %2d | %2d | %2d | %3d | %2d | %2d |\n",
               procesos[i].id,
               procesos[i].tiempo_llegada,
               procesos[i].rafaga_ejecucion,
               procesos[i].tiempo_finalizacion,
               procesos[i].tiempo_retorno,
               procesos[i].tiempo_espera,
               procesos[i].tiempo_respuesta);

        total_retorno += procesos[i].tiempo_retorno;
        total_espera += procesos[i].tiempo_espera;
        total_respuesta += procesos[i].tiempo_respuesta;
    }

    printf("------------------------------------------------------------\n");
    printf("Promedio TAT: %.2f\n", total_retorno / n);
    printf("Promedio WT : %.2f\n", total_espera / n);
    printf("Promedio RT : %.2f\n", total_respuesta / n);

    free(cola);
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
        procesos[n - 1].tiempo_restante = bt;
        procesos[n - 1].iniciado = 0;
        procesos[n - 1].completado = 0;
    }
    fclose(archivo);

    int quantum = 2; // 🔁 Puedes cambiar este valor para probar otros quantums
    if (n > 0) simular_rr(procesos, n, quantum);
    else printf("No se encontraron procesos.\n");

    free(procesos);
    return 0;
}
