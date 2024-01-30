#include <stdio.h>
#include "tablaSimbolos.h"
#include "analizadorSintactico.h"
#include "sistemaEntrada.h"
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
    iniciarSistemaEntrada(nombreFichero);
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
    finalizarSistemaEntrada();
    //-------------------------

    //FINALIZAMOS EL GESTOR DE ERRORES
    finalizarGestorErrores();

    return 0;
}
