#ifndef ESTRUCTURAALMACENAMIENTO_H
#define ESTRUCTURAALMACENAMIENTO_H


//Estructura de la información que tenemos, TIPOELEM
typedef struct {
    char *lexema;
    int valor;
}tipoelem;


//Establecemos cual es la clave de ordenacion
typedef char *tipoclave;

//Establecemos como tipo opaco la forma del arbol.
typedef struct celda *abb;

/*FUNCIONAS PUBLICAS*/

//Creamos el arbol binario
void crear(abb *A);

//Destruiimos el arbol binario
void destruir(abb *A);

//buscamos elementos en el arbol y los insertamos en el caso de no encontrarlos.
int buscar_Insertar_Arbol(abb *A, tipoelem E);

//Imprimimos el arbol binario
void imprimirABB(abb A);

#endif