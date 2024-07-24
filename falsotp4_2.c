#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 1- Cargar tareas pendientes
// 2- Preguntar si desea mover tareas de pendientes a completadas
// 3- Transferir, si corresponde, tareas de pendientes a completadas
// 4- Mostrar listas pendientes y realizadas completas
// 5- Preguntar si se quiere consultar una tarea específica
// 6- Si es pertinente, mostrar los datos de la tarea (desc., dur. y estado)
// 7- Liberar memoria

// Definicion de estructuras
struct tarea
{
    int TareaId; // numero autoincremental comenzando en 1000
    char *Descripcion;
    int duracion; // entero en horas
} typedef Tarea;

struct Nodo
{
    Tarea deber;
    struct Nodo *Siguiente;
} typedef Nodo;

// Declaracion de funciones

Nodo *crearNodo(Tarea tarea);
Nodo *crearLista();
void mostrarTarea(const Tarea *tarea);
void cargarAlFinal(Nodo **lista, Nodo *nodo);
void liberarTarea(Tarea *tarea);
void cargarTareasPendientes(Nodo **lista);
void mostarLista(Nodo *lista);
void moverTareas(Nodo **lista, Nodo **lista2);
void buscarPorId(Nodo *lista, Nodo *lista2);
void buscarPorPalabra(Nodo *lista, Nodo *lista2);
void buscarTareas(Nodo *lista, Nodo *lista2);
void eleccionFinal(Nodo *lista, Nodo *lista2);
void liberarLista(Nodo *lista);

// Main
int main()
{
    srand(time(NULL));

    // Creo las listas de pendientes y completas
    Nodo *listaPendientes = crearLista();
    Nodo *listaCompletas = crearLista();

    // El usuario carga los datos
    cargarTareasPendientes(&listaPendientes);

    // Procedo con la reasignación de tareas si es pertinente
    moverTareas(&listaPendientes, &listaCompletas);

    // Pido al usuario que elija si buscar, mostrar, o salir
    eleccionFinal(listaPendientes, listaCompletas);

    // Libero la memoria de las listas
    liberarLista(listaPendientes);
    liberarLista(listaCompletas);
    return 0;
};

Nodo *crearNodo(Tarea tarea)
{
    // Creacion de un nodo auxiliar
    Nodo *Aux;
    Aux = (Nodo *)malloc(sizeof(Nodo));

    // Realizo la carga de datos
    Aux->deber.Descripcion = (char *)malloc(sizeof(char) * (strlen(tarea.Descripcion) + 1)); // Reserva para desc.
    strcpy(Aux->deber.Descripcion, tarea.Descripcion);                                       // copia de strings
    Aux->deber.TareaId = tarea.TareaId;
    Aux->deber.duracion = tarea.duracion;

    // El nodo creado apunta a NULL
    Aux->Siguiente = NULL;
    return Aux;
}

Nodo *crearLista()
{
    // Retorno un nodo Vacío
    return NULL;
}

void mostrarTarea(const Tarea *tarea)
{
    if (tarea != NULL) // Compruebo que la tarea sea valida
    {
        // Imprimo los datos
        printf("\tTarea id: %d\n", tarea->TareaId);
        printf("\tDescripcion: %s\n", tarea->Descripcion);
        printf("\tDuracion: %d horas\n\n", tarea->duracion);
    }
    else
    {
        printf("Tarea no valida.\n");
    }
}

void cargarAlFinal(Nodo **lista, Nodo *nodo)
{
    // Checkear lista vacia
    if (*lista == NULL)
    {
        *lista = nodo;
    }
    else
    {
        // Declaro un nodo auxiliar para no perder la cabecera
        Nodo *Aux = *lista;

        // itero hasta que el siguiente no exista
        while (Aux->Siguiente)
        {
            Aux = Aux->Siguiente;
        }

        // Encontrado el final de la lista, modifico el nodo
        Aux->Siguiente = nodo; // como Aux y lista comparten la misma direccion de memoria, modificar Aux modifica lista
    }
}

void liberarTarea(Tarea *tarea)
{
    // Se deben liberar los campos de frases y la estructura pero en forma de puntero, otherwise no se puede
    if (tarea != NULL)
    {
        free(tarea->Descripcion);
        free(tarea);
    }
}

void cargarTareasPendientes(Nodo **lista)
{

    char bandera = 'n';                                // creo la bandera para iterar
    int id = 999;                                      // para asignar ids desde mil
    char *buffer = (char *)malloc(sizeof(char) * 250); // buffer para cargar descripciones
    char respuesta;                                    // para cargar si el usuario quiere o no seguir
    // Comienza la carga
    puts("\n--- Carga de Pendientes ---\n");

    // Cargo tareas hasta que el usuario pida basta
    do
    {
        // Creo Aux dentro del Bucle por liberacion de memoria
        Tarea *Aux = (Tarea *)malloc(sizeof(Tarea)); // estructura tarea auxiliar

        // Asigno el id y actualizo la variable
        Aux->TareaId = id + 1;
        id++;

        // pido y pongo descripcion en el buffer, para luego copiarla en Aux
        puts("\nIntroduzca la descripcion de la tarea:\n");
        gets(buffer);
        Aux->Descripcion = (char *)malloc(strlen(buffer) + 1); // reservo memoria para la desc
        strcpy(Aux->Descripcion, buffer);                      // copio el buffer en Aux

        // Cargo la duracion
        puts("\nIntroduzca la duracion en horas\n");
        scanf("%d", &Aux->duracion);
        fflush(stdin);

        // Creo un nodo con Aux
        Nodo *nodoAuxiliar = crearNodo(*Aux);

        // Cargo el nodo a la lista de pendientes
        cargarAlFinal(lista, nodoAuxiliar); // LISTA SIN * !!!

        // Pregunto si el usuario desea cargar otra tarea
        puts("\nQuiere cargar otra tarea? (s/n)\n");
        scanf(" %c", &respuesta);
        fflush(stdin);

        if (respuesta == 's')
        {
            bandera = 's';
        }
        else
        {
            bandera = 'n'; // el caracter es irrelevante, solo debe ser != 's'
        }

        // Libero la memoria del Aux
        liberarTarea(Aux);

    } while (bandera == 's');

    free(buffer); // libero al buffer
}

void mostarLista(Nodo *lista)
{
    // Creo un nodo Aux
    Nodo *Aux = lista;

    // compruebo que la lista no este vacia
    if (Aux == NULL)
    {
        puts("\n--- La lista esta vacia ---\n");
        return;
    }

    puts("'\n---- Tareas de la lista ----\n");

    // Imrpimo los elementos de la tareas hasta que llego al final
    do
    {
        mostrarTarea(&Aux->deber);

        Aux = Aux->Siguiente; // actualizo Aux
    } while (Aux != NULL);
}

void moverTareas(Nodo **lista, Nodo **lista2)
{
    char sino;           // para preguntar si quiere mover tareas
    char iterable = 's'; // para iterar el movimiento de tareas
    int id;              // Para guardar el id de la tarea

    // Consulto si desea mover tareas
    puts("\nDesea mover tareas de pendientes a realizadas? (s/n): ");
    scanf(" %c", &sino);
    fflush(stdin);

    // Si no quiere mover tareas, sale de la funcion
    if (sino == 'n')
    {
        return;
    }

    // Muestro la lista de tareas
    puts("\n>>> Pendientes\n");
    mostarLista(*lista); // En este caso, usar el * desreferencia al puntero

    // Iteracion para mover n tareas
    do
    {
        Nodo *Previo = NULL;   // Nodo auxiliar previo
        Nodo *Actual = *lista; // Nodo Auxiliar actual

        // Solicito el id
        puts("\nIngrese el id de la tarea: ");
        scanf("%d", &id);
        fflush(stdin);

        // Realizo un control para comprobar que la lista no esta vacia
        if (Actual != NULL)
        {
            // Iteracion para buscar y mover la tarea
            while (Actual != NULL)
            {
                // Encuentro el nodo y modifico las listas
                if (Actual->deber.TareaId == id)
                {
                    // Controlo si es el primer elemento de la lista, si es asi, muevo la cabecera
                    if (Previo == NULL)
                    {
                        *lista = Actual->Siguiente;
                    }
                    else
                    {
                        Previo->Siguiente = Actual->Siguiente;
                    }

                    // Muevo el nodo a la lista2
                    Actual->Siguiente = NULL; // Importante para evitar ciclos
                    cargarAlFinal(lista2, Actual);

                    // Salgo del bucle una vez que se mueve la tarea
                    break;
                }
                else // Si no lo encuentro, actualizo los nodos
                {
                    Previo = Actual;
                    Actual = Actual->Siguiente;
                }
            }

            // Si la tarea no está, aviso
            if (Actual == NULL)
            {
                puts("\nLa lista no contiene una tarea con ese id <<<<<");
            }
        }
        else
        {
            puts("\nLa lista está vacía.");
        }

        // Pregunto si desea buscar otra tarea
        puts("\n>>> Desea mover otra tarea? (s/n):");
        scanf(" %c", &iterable);

    } while (iterable == 's');
}

void buscarPorId(Nodo *lista, Nodo *lista2)
{
    Nodo *Aux;    // Declaro nodo auxiliar
    char repetir; // Para seguir iterando busqueda de tareas

    do // aquí itero cuantas veces quiera buscar tareas
    {
        int encontrada = 0; // 1 si se encuentra tarea, 0 si no
        int id;

        // Consulto la tarea
        puts("\nIngrese el id de la tarea a buscar: ");
        scanf("%d", &id);
        fflush(stdin);

        // ubico el auxiliar en la primera lista
        Aux = lista;
        // Si el Auxiliar no es nulo, busco la tarea
        while (Aux != NULL)
        {
            // Si se encuentra la tarea se imprime todo
            if (Aux->deber.TareaId == id)
            {
                encontrada = 1; // actualizo el valor de encontrada
                puts("\n\t--- Tarea encontrada ---\n");
                mostrarTarea(&Aux->deber);
                printf("\tEstado de la tarea: >> Pendiente <<\n");
            }

            if (encontrada == 1)
            {
                break;
            }
            Aux = Aux->Siguiente;
        }

        if (encontrada != 1)
        {
            Aux = lista2;
            while (Aux != NULL)
            {
                // Si se encuentra la tarea se imprime todo
                if (Aux->deber.TareaId == id)
                {
                    encontrada = 1; // actualizo el valor de encontrada
                    puts("\n\t--- Tarea encontrada ---\n");
                    mostrarTarea(&Aux->deber);
                    printf("\tEstado de la tarea: >> Completada <<\n");
                }

                if (encontrada == 1)
                {
                    break;
                }
                Aux = Aux->Siguiente;
            }
        }

        if (encontrada == 0)
        {
            puts("\n>>>>Tarea no encontrada\n");
        }

        // Pregunto si quiere repetir
        puts("\nDesea buscar otra tarea?(s/n): \n");
        scanf(" %c", &repetir);
        fflush(stdin);
    } while (repetir == 's');
}

void buscarPorPalabra(Nodo *lista, Nodo *lista2)
{
    Nodo *Aux;              // Declaro nodo auxiliar
    char repetir;           // Para seguir iterando busqueda de tareas
    char palabraClave[100]; // Para almacenar la palabra clave ingresada

    do
    {
        int encontrada = 0; // 1 si se encuentra tarea, 0 si no
        // Consulto la palabra clave
        puts("\nIngrese la palabra clave para buscar la tarea: ");
        gets(palabraClave);

        // Buscar en la lista de tareas pendientes
        Aux = lista;
        while (Aux != NULL)
        {
            if (strcmp(Aux->deber.Descripcion, palabraClave) == 0)
            {
                encontrada = 1; // actualizo el valor de encontrada
                puts("\n\t--- Tarea encontrada ---\n");
                mostrarTarea(&Aux->deber);
                printf("\tEstado de la tarea: >> Pendiente <<\n");
                break;
            }
            Aux = Aux->Siguiente;
        }

        // Si no se encuentra en la primera lista, buscar en la segunda lista
        if (!encontrada) // !encontrada = encontrada == 0
        {
            Aux = lista2;
            while (Aux != NULL)
            {
                if (strcmp(Aux->deber.Descripcion, palabraClave) == 0)
                {
                    encontrada = 1; // actualizo el valor de encontrada
                    puts("\n\t--- Tarea encontrada ---\n");
                    mostrarTarea(&Aux->deber);
                    printf("\tEstado de la tarea: >> Completada <<\n");
                    break;
                }
                Aux = Aux->Siguiente;
            }
        }

        if (!encontrada)
        {
            puts("\n>>>>Tarea no encontrada\n");
        }

        // Pregunto si quiere repetir
        puts("\nDesea buscar otra tarea? (s/n): ");
        scanf(" %c", &repetir);
        fflush(stdin);
    } while (repetir == 's');
}

void buscarTareas(Nodo *lista, Nodo *lista2)
{
    int criterio; // determina el tipo de busqueda

    // Pregunto si quiere buscar por id o palabra clave
    puts("\nElija el metodo que prefiera para buscar la tarea:\n1-Id\t2-Palabra clave");
    scanf("%d", &criterio);
    fflush(stdin);

    // Si busca por id
    if (criterio == 1)
    {
        buscarPorId(lista, lista2);
    }
    else
    { // si busca por palabra
        buscarPorPalabra(lista, lista2);
    }
}

void eleccionFinal(Nodo *lista, Nodo *lista2) {
    int eleccion;
    // Pregunto que quiere hacer el usuario
    puts(">> Seleccione que desea hacer a continuacion:\n\t1-Buscar una tarea especifica\n\t2-Mostrar ambas listas de tareas\n\t3-Salir");
    scanf(" %d", &eleccion);
    fflush(stdin);

    // Switch según el caso
    switch (eleccion)
    {
    case 1:
        buscarTareas(lista, lista2);
        break;
    
    case 2:
        puts("\n>> Tareas Pendientes <<\n");
        mostarLista(lista);

        puts("\n>> Tareas Completadas <<\n");
        mostarLista(lista2);
        break;
    
    case 3:
        puts("\n\t--- Cerrando Programa ---\n");
        break;
    }
}

void liberarLista(Nodo *lista) {
    Nodo *Aux; // Designo nodo Aux

    while (lista != NULL) { // si la lista no está vacía
        // pongo el nodo en el lugar de la lista para liberarlo, y voy moviendo la lista
        Aux = lista;
        lista = lista->Siguiente; 
        free(Aux->deber.Descripcion); // solo debo liberar este campo ya que es el unico con memo dinamica asignada
        free(Aux);
    }
}