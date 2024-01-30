#include <stdio.h>
#include <stdlib.h>
#include "gestorErrores.h"

FILE *fErrores;

//Función que abre el fichero de errores
void inicializarGestorErrores(){
    fErrores= fopen("ficheroErrores.log", "a");
    if(fErrores==NULL){
        printf("ERRO al abrir/crear el fichero de errores\n");
        EXIT_FAILURE;
    }
}

//Función que escribe errores en el fichero de errores.
void nuevoError(int codigo, int linea){
    if(fErrores){
        //En función del codigo escribimos un mensaje de error
        switch(codigo){
            case 0: //Error-lexico-> Numero Mal formado.
                fprintf(fErrores, "Numero mal formado en la linea %d\n", linea);
                break;
            case 1: //Error al abrir el fichero de lectura
                fprintf(fErrores, "Error al abrir el fichero que se quiere analizar\n");
                break;
            case 2:
                fprintf(fErrores, "ERROR-sobreCarga en la linea. %d se continuan llenando los buffer y se perderá información\n", linea);
                break;
            case -1: //Cierre de ejecución
                fprintf(fErrores, "-------------FIN EJECUCION-------------\n\n");
        }
    }
}

//Función que cierra el fichero de errores
void finalizarGestorErrores(){
    fclose(fErrores);
    fErrores=NULL;
}
