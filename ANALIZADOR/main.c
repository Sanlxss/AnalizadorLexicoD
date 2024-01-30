#include <stdio.h>
#include "tablaSimbolos.h"
#include "analizadorSintactico.h"
#include "lex.yy.h"
#include "gestorErrores.h"




int main(int argc, char **argv) {
    char *nombreFichero;


    //Comprobamos que se pasa el nombre del fichero a analizar como parametro.
    if(argc!=2){
        printf("Error, parametros introducidos incorrectos");
        return 1;
    }

    //Iniciamos sistema de Entrada y gestorErrores
    nombreFichero=argv[1];
    abrirFichero(nombreFichero);
    inicializarGestorErrores();
    //------------------------------------

    //Inicializamos la Tabla de Simbolos.
    inicializarTabla();
    //-----------------------------------

    //Imprimimos la tabla inicial.
    imprimirContenidoTabla();
    //---------------------------


    //Analizamos el codigo
    analizar();
    //---------------------


    //Imprimirmos la tabla resultante
    printf("-------------------------------------\n");
    imprimirContenidoTabla();
    //--------------------------------------------------

    //LIBERAMOS MEMORIA
    borrarTabla();
    cerrarFichero();
    //-------------------------

    //FINALIZAMOS EL GESTOR DE ERRORES
    finalizarGestorErrores();

    return 0;
}
