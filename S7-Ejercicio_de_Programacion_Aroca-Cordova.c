#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTOS 5
#define TAM_NOMBRE 50

// Comparacion manual de cadenas caracter por caracter
int compararCadenas(char str1[], char str2[]) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    
    if (str1[i] == '\0' && str2[i] == '\0') {
        return 1;
    } else {
        return 0;
    }
}


int buscarProducto(char nombres[MAX_PRODUCTOS][TAM_NOMBRE], int total, char objetivo[]) {
    for (int i = 0; i < total; i++) {
        if (compararCadenas(nombres[i], objetivo) == 1) {
            return i; // Retorna la posicion donde se encontro
        }
    }
    return -1; // No existe
}

//Ingreso de productos
void ingresarProducto(char nombres[MAX_PRODUCTOS][TAM_NOMBRE], float tiempos[], float recursos[], int demandas[], int *total) {
    if (*total >= MAX_PRODUCTOS) {
        printf("\n[!] Limite alcanzado: maximo %d productos.\n", MAX_PRODUCTOS);
        return;
    }

    printf("\n--- REGISTRO DE PRODUCTO [%d] ---\n", *total + 1);
    printf("Nombre del producto: ");
    scanf(" %[^\n]", nombres[*total]);

    // Validar si ya existe con la funcion propia de busqueda
    if (buscarProducto(nombres, *total, nombres[*total]) != -1) {
        printf("[!] Error: Ya existe un producto con ese nombre.\n");
        return;
    }

    do {
        printf("Tiempo de fabricacion por unidad (horas): ");
        scanf("%f", &tiempos[*total]);
        if (tiempos[*total] <= 0) printf("[!] Ingrese un tiempo mayor a 0.\n");
    } while (tiempos[*total] <= 0);

    do {
        printf("Recursos requeridos por unidad: ");
        scanf("%f", &recursos[*total]);
        if (recursos[*total] <= 0) printf("[!] Ingrese una cantidad mayor a 0.\n");
    } while (recursos[*total] <= 0);

    do {
        printf("Demanda solicitada (unidades): ");
        scanf("%d", &demandas[*total]);
        if (demandas[*total] < 0) printf("[!] La demanda no puede ser negativa.\n");
    } while (demandas[*total] < 0);

    (*total)++;
    printf("[+] Producto registrado con exito.\n");
}

//Listar productos
void listarProductos(char nombres[MAX_PRODUCTOS][TAM_NOMBRE], float tiempos[], float recursos[], int demandas[], int total) {
    if (total == 0) {
        printf("\n[!] No hay productos registrados.\n");
        return;
    }

    printf("\n--- LISTA DE PRODUCTOS ---\n");
    printf("Nro\tNombre\t\tTiempo(h)\tRecursos\tDemanda\n");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%d\t%s\t\t%.2f\t\t%.2f\t\t%d\n", i + 1, nombres[i], tiempos[i], recursos[i], demandas[i]);
    }
    printf("------------------------------------------------------------------\n");
}

//Edicion de datos
void editarProducto(char nombres[MAX_PRODUCTOS][TAM_NOMBRE], float tiempos[], float recursos[], int demandas[], int total) {
    if (total == 0) {
        printf("\n[!] No hay productos para editar.\n");
        return;
    }

    char busqueda[TAM_NOMBRE];
    printf("\nIngrese el nombre del producto que desea editar: ");
    scanf(" %[^\n]", busqueda);

    int pos = buscarProducto(nombres, total, busqueda);
    if (pos == -1) {
        printf("[!] No se encontro el producto '%s'.\n", busqueda);
        return;
    }

    printf("\n--- Modificando datos de: %s ---\n", nombres[pos]);
    printf("Nuevo nombre: ");
    scanf(" %[^\n]", nombres[pos]);

    do {
        printf("Nuevo tiempo unitario (horas): ");
        scanf("%f", &tiempos[pos]);
    } while (tiempos[pos] <= 0);

    do {
        printf("Nuevos recursos por unidad: ");
        scanf("%f", &recursos[pos]);
    } while (recursos[pos] <= 0);

    do {
        printf("Nueva demanda: ");
        scanf("%d", &demandas[pos]);
    } while (demandas[pos] < 0);

    printf("[+] Datos actualizados correctamente.\n");
}

//Eliminacion con desplazamiento manual
void eliminarProducto(char nombres[MAX_PRODUCTOS][TAM_NOMBRE], float tiempos[], float recursos[], int demandas[], int *total) {
    if (*total == 0) {
        printf("\n[!] No hay productos para eliminar.\n");
        return;
    }

    char busqueda[TAM_NOMBRE];
    printf("\nIngrese el nombre del producto a eliminar: ");
    scanf(" %[^\n]", busqueda);

    int pos = buscarProducto(nombres, *total, busqueda);
    if (pos == -1) {
        printf("[!] No se encontro el producto '%s'.\n", busqueda);
        return;
    }

    // Desplazamos todos los elementos una posicion a la izquierda
    for (int i = pos; i < (*total) - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        tiempos[i] = tiempos[i + 1];
        recursos[i] = recursos[i + 1];
        demandas[i] = demandas[i + 1];
    }

    (*total)--;
    printf("[+] Producto eliminado con exito.\n");
}

//Calculos de tiempo, recursos y viabilidad de la fabrica
void evaluarProduccion(float tiempos[], float recursos[], int demandas[], int total, float tiempoLimite, float recursosLimite) {
    if (total == 0) {
        printf("\n[!] Ingrese productos antes de evaluar la produccion.\n");
        return;
    }

    float totalTiempo = 0;
    float totalRecursos = 0;

    for (int i = 0; i < total; i++) {
        totalTiempo = totalTiempo + (tiempos[i] * demandas[i]);
        totalRecursos = totalRecursos + (recursos[i] * demandas[i]);
    }

    printf("\n=========================================\n");
    printf("   EVALUACION DE LA LINEA DE PRODUCCION    \n");
    printf("===========================================\n");
    printf("Tiempo total necesario   : %.2f horas\n", totalTiempo);
    printf("Tiempo disponible        : %.2f horas\n", tiempoLimite);
    printf("Recursos necesarios      : %.2f unidades\n", totalRecursos);
    printf("Recursos disponibles     : %.2f unidades\n", recursosLimite);
    printf("-----------------------------------------\n");

    if (totalTiempo <= tiempoLimite && totalRecursos <= recursosLimite) {
        printf("[VIABLE] La fabrica SI puede cumplir con la demanda.\n");
        printf("Sobran %.2f horas y %.2f unidades de recursos.\n", tiempoLimite - totalTiempo, recursosLimite - totalRecursos);
    } else {
        printf("[NO VIABLE] La fabrica NO puede cumplir con la demanda:\n");
        if (totalTiempo > tiempoLimite) {
            printf(" -> Faltan %.2f horas de produccion.\n", totalTiempo - tiempoLimite);
        }
        if (totalRecursos > recursosLimite) {
            printf(" -> Faltan %.2f unidades de recursos.\n", totalRecursos - recursosLimite);
        }
    }
    printf("=========================================\n");
}

int main() {
    char nombres[MAX_PRODUCTOS][TAM_NOMBRE];
    float tiempos[MAX_PRODUCTOS];
    float recursos[MAX_PRODUCTOS];
    int demandas[MAX_PRODUCTOS];
    int total = 0;

    float tiempoDisponible = 0;
    float recursosDisponibles = 0;
    int opcion = 0;

    printf("=== CONFIGURACION DE FABRICA ===\n");
    do {
        printf("Ingrese las horas disponibles en fabrica: ");
        scanf("%f", &tiempoDisponible);
    } while (tiempoDisponible <= 0);

    do {
        printf("Ingrese la cantidad total de recursos disponibles: ");
        scanf("%f", &recursosDisponibles);
    } while (recursosDisponibles <= 0);

    do {
        printf("\n--- MENU DE PRODUCCION ---\n");
        printf("1. Ingresar producto\n");
        printf("2. Mostrar productos registrados\n");
        printf("3. Editar producto\n");
        printf("4. Eliminar producto\n");
        printf("5. Evaluar produccion de la demanda\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            opcion = 0;
        }

        switch (opcion) {
            case 1:
                ingresarProducto(nombres, tiempos, recursos, demandas, &total);
                break;
            case 2:
                listarProductos(nombres, tiempos, recursos, demandas, total);
                break;
            case 3:
                editarProducto(nombres, tiempos, recursos, demandas, total);
                break;
            case 4:
                eliminarProducto(nombres, tiempos, recursos, demandas, &total);
                break;
            case 5:
                evaluarProduccion(tiempos, recursos, demandas, total, tiempoDisponible, recursosDisponibles);
                break;
            case 6:
                printf("\nSaliendo del programa...\n");
                break;
            default:
                printf("\nOpcion incorrecta, intente de nuevo.\n");
                break;
        }
    } while (opcion != 6);

    return 0;
}