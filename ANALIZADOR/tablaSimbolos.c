#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definiciones.h"
#include "estructuraAlmacenamiento.h"

#define MAX_LENGTH 100
#define NUM_STRINGS 9


abb tablaSimbolos;

    /*
     Función para inicializar la tabla de simbolos.
     * */
void inicializarTabla(){
    tipoelem info;
    
    info.lexema=NULL; //inizializamos el lexema

    //Creamos unas variables que utilizaremos para inicializar la tabla de simbolos
    char lex [NUM_STRINGS][MAX_LENGTH]= {"double", "cast", "foreach", "import", "return",  "void", "while",  "id", "string"};
    int valores [] = {DOUBLE,CAST, FOREACH, IMPORT, RETURN, VOID, WHILE,  ID, STRING};


    crear(&tablaSimbolos); //creamos la tabla de simbolos.

    //Para cada elemento de los que pusimos arriba los introducimos en la tabla de simbolos.
    
    for(int i=0; i< NUM_STRINGS;i++){
        info.lexema = (char*)malloc(strlen(lex[i])*sizeof (char)+1);
        memset(info.lexema,'\0',strlen(lex[i])+1);
        strcpy(info.lexema,lex[i]);
        info.valor = valores[i];
        buscar_Insertar_Arbol(&tablaSimbolos, info);
        free(info.lexema); //liberamos memoria
        info.lexema=NULL;
    }
    

}


/*
 FUncion para buscar elementos en la tabla de simbolos (devuelve el tipo de elemento
 En el caso de no encontrar el elemento lo inserta en la posición que le corresponde.
 * */
int buscar_insertar_Tabla(char *lex, int valor){
    tipoelem info;
    info.lexema=lex;
    info.valor=valor;
    return buscar_Insertar_Arbol(&tablaSimbolos, info);
}

/*
 Función para imprimrir todo el contenido de la tabla de simbolos
 * */
void imprimirContenidoTabla(){
    imprimirABB(tablaSimbolos);
}


/*
 Función para borrar la tabla de simbolos
 * */
void borrarTabla(){
    destruir(&tablaSimbolos);
    printf("\t\t---TABLA BORRADA--\n");
}
