#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuraAlmacenamiento.h"


/*Estructura de los nodos del arbol*/
struct celda{
    tipoelem info;
    struct celda *izq, *der;
};
/*-------------------------------*/

//FUNCION COMPROBACION VACIO
unsigned es_vacio(abb A) {
    return A == NULL;
}

//FUNCION IMPRIME DATOS DE UN NODO
void leer(abb A) {
    printf("\t\t< %d  |  %s >\n", A->info.valor, A->info.lexema);
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

//FUNCION ORDEN LECTURA ARBOL
void inorden(abb A){
    if(!es_vacio(A)){
        inorden(izq(A));
        leer(A);
        inorden(der(A));
    }
}

/*FUNCIONES PUBLICAS*/

//FUNCION CREACION ARBOL
void crear(abb *A) {
    *A = NULL;
}

//FUNCION DESTRUCCION ARBOL
void destruir(abb *A) {
    if (*A != NULL) {
        destruir(&(*A)->izq);
        destruir(&(*A)->der);
        (*A)->izq = NULL;
        (*A)->der = NULL;
        free(((*A)->info.lexema));
        (*A)->info.lexema = NULL;
        free(*A);
        *A = NULL;
    }
}

//FUNCION de BUSQUEDA e INSERCION
int buscar_Insertar_Arbol(abb *A, tipoelem E){
    //si esta vacio
    if(es_vacio(*A)){
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info.lexema = (char*)malloc(strlen(E.lexema)*sizeof (char)+1);
        memset((*A)->info.lexema, '\0',strlen(E.lexema)+1); 
        strcpy((*A)->info.lexema, E.lexema);
        (*A)->info.valor=E.valor;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return (*A)->info.valor;
    }
        //Si lo encuentro
        
    else if(strcmp((*A)->info.lexema, E.lexema)==0){
        return (*A)->info.valor;
        //Si esta a la izquierda
    }else if(strcmp((*A)->info.lexema, E.lexema) < 0){
        return buscar_Insertar_Arbol(&(*A)->izq, E);
        //Si estaá a la derecha.
    }else{
        return buscar_Insertar_Arbol(&(*A)->der, E);
    }
}

//FUNCION PUBLICA IMPRIMIR ARBOL
void imprimirABB(abb A){
    printf("\t\t--TABLA DE SIMBOLOS--\n");
    inorden(A);
    printf("\t\t---------------------\n");
}

