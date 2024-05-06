#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Tarea
{
    int TareaID;       // Numérico autoincremental comenzando en 1000
    char *Descripcion; //
    int Duracion;      // entre 10 – 100
} typedef Tarea;

struct Nodo
{
    struct Tarea T;
    struct Nodo *Siguiente;
} typedef Nodo;

// Definicion de Funciones
Nodo *crearNodo(Tarea tarea);
Nodo *crearLista();
void agregarPrincipio(Nodo **lista, Nodo *nodo);
void agregarFinal(Nodo **lista, Nodo *nodo);
int seguirCargando(char seguir);
void preguntarTarea(Nodo **lista);
void mostrarTareas(Nodo *lista);

int main()
{
    srand(time(NULL));

    // Creo una lista y la apunto a NULL con crearLista
    Nodo *listaPendientes;
    listaPendientes = crearLista();

    // Pregunto e implemento mediante funcion
    preguntarTarea(&listaPendientes);

    // Muestro
    mostrarTareas(listaPendientes);

    // Creo una lista vacia
    Nodo *listaCompletos;
    listaCompletos = crearLista();

    int sino;
    printf("Desea marcar tareas como completadas? (0=si,1=no)");
    scanf("%d", &sino);

    if (sino == 0)
    {
        int id;
        printf("Introduzca el id de la tarea completada");
        scanf("%d", &id);
        id = id + 999;

        Nodo *Auxiliar = listaPendientes;
        Nodo *Anterior = NULL;
        while (Auxiliar && Auxiliar->T.TareaID != id)
        {
            Anterior = Auxiliar;
            Auxiliar = Auxiliar->Siguiente;
        }

        if(!Auxiliar) {
            printf("No existe tarea con ese id");
        } else {
            if (Anterior == NULL) {
                listaPendientes = listaPendientes->Siguiente;
                Auxiliar->Siguiente = NULL;
                agregarFinal(&listaCompletos, Auxiliar);
            } else {
                Anterior->Siguiente = Auxiliar->Siguiente;
                Auxiliar->Siguiente = NULL;
                agregarFinal(&listaCompletos, Auxiliar);
            }
        }
    }
    printf("Lista de tareas completas:\n");
    mostrarTareas(listaCompletos);
}


Nodo *crearNodo(Tarea tarea)
{
    // Creación de Nodo Auxiliar
    Nodo *Aux;
    // Reserva de memoria para Nodo auxiliar
    Aux = (Nodo *)malloc(sizeof(Nodo));

    // Carga de datos
    Aux->T.Descripcion = (char *)malloc(sizeof(char) * (strlen(tarea.Descripcion) + 1)); // Reserva de memoria solo para descripción porque se comporta como arrelo
    strcpy(Aux->T.Descripcion, tarea.Descripcion);                                       // Copio porque son dos punteros
    Aux->T.TareaID = tarea.TareaID;
    Aux->T.Duracion = tarea.Duracion;

    // En la creación, los nodos apuntan a NULL
    Aux->Siguiente = NULL;
    return Aux;
}

Nodo *crearLista()
{
    return NULL;
}

void agregarPrincipio(Nodo **lista, Nodo *nodo)
{
    nodo->Siguiente = *lista;
    *lista = nodo;
}

void agregarFinal(Nodo **lista, Nodo *nodo)
{
    // Control Lista Vacia
    if (*lista == NULL)
    {
        *lista = nodo;
    }
    else
    {
        // Declaro un nodo Aux para no perder la cabecera
        Nodo *Aux = *lista;
        // Mientras el aux no sea nulo mueve la cabecera
        while (Aux->Siguiente)
        {
            Aux = Aux->Siguiente;
        }
        // Al compartir direccion de memoria, conectando con Aux conecto con lista.
        Aux->Siguiente = nodo;
    }
}

int seguirCargando(char seguir)
{
    int respuesta = 1;

    if (seguir == 's' || seguir == 'S')
    {
        respuesta = 0;
    }
    return respuesta;
}

void preguntarTarea(Nodo **lista)
{
    // Creo Bandera e Id tracker
    int bandera = 0;
    char seguir; // para la pregunta de la bandera
    int ID = 999;
    // Creo un Buffer y le asigno memoria
    char *buffer;
    buffer = (char *)malloc(sizeof(char) * 270);
    // Creo Tarea auxiliar
    Tarea Aux;

    printf("Ingrese los datos de las tareas:\n");

    while (bandera == 0)
    {
        // Auxiliar con ID
        Aux.TareaID = ID + 1;
        ID++;
        printf("Introduzca los datos de la tarea \n----------------\n");
        printf("Ingrese la descripcion de la tarea:\n");
        // cargo la descripcion al Buffer
        gets(buffer);

        // Reservo memoria para la descripción del Aux de la iteracion actual
        Aux.Descripcion = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));

        // Copio el buffer en la descripción del Aux
        strcpy(Aux.Descripcion, buffer);

        // Duracion
        printf("Ingrese la duracion en horas:\n");
        scanf("%d", &Aux.Duracion);
        fflush(stdin);

        // Utilizo la tarea Auxiliar para crear un nodo
        Nodo *nuevoNodo = crearNodo(Aux);

        // Cargo el nodo al final de la lista
        agregarFinal(lista, nuevoNodo);

        printf("¿Desea agregar otra tarea? (s/n)");
        scanf("%c", &seguir);
        fflush(stdin);

        bandera = seguirCargando(seguir);
    }
}

void mostrarTareas(Nodo *lista)
{
    // Compruebo que la lista no este vacia
    if (lista == NULL)
    {
        printf("No hay tareas en esta categoria");
    }
    else
    { // muestro los elementos hasta terminar la lista
        while (lista != NULL)
        {
            printf("Tarea:\n");
            printf("\tDescripcion: %s\n", lista->T.Descripcion);
            printf("\tDuracion: %d\n", lista->T.Duracion);

            lista = lista->Siguiente;
        }
    }
}
