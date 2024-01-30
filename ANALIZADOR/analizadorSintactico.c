#include <stdio.h>
#include <stdlib.h>

#include "analizadorSintactico.h"
#include "lex.yy.h"
#include "tablaSimbolos.h"
#include "gestorErrores.h"


void analizar(){
    componenteLexico comp;
    comp.lexema=NULL;

    printf("\n\t---INICIO ANALISIS---\n");

    siguienteComponenteLexico(&comp); //Solicitamos el primer Componente Lexico

    while(comp.valor!=EOF){ //Mientras no terminemos de leer el fichero
        if(comp.lexema != NULL){    //Comprobamos que el lexema tiene contenido y podemos escribir algo
            printf("\t\t< %d  |  %s >\n", comp.valor, comp.lexema); //Mostramos por pantalla el componente lexico
        }
        siguienteComponenteLexico(&comp); //Solicitamos un nuevo componente lexico

    }
    printf("\n\t---FIN ANALISIS---\n");
    nuevoError(-1,-1); //Establecemos el final de los errores de la ultima ejecucion en el fichero de errores

    //Liberamos Memoria.
    free(comp.lexema);
    comp.lexema=NULL;
}
