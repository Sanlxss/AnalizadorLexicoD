#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "analizadorLexico.h"
#include "sistemaEntrada.h"
#include "tablaSimbolos.h"
#include "gestorErrores.h"

#define tamBloque 32

FILE *fp; //Varaible identificación fichero



//Estructura de buffer.
typedef struct {
    char contenido[tamBloque+1];
    int inicio;
    int delantero;
}bloque;

bloque centinelas[2]; //definimos dos bloques
int numeroCentinela;    //varibale para seleccionar un bloque
long numCaracteresLeidos=0; //variable para contar el numero de caracteres leidos
int yaCargado=0;    //variable para saber si hemos cargado un nuevo bloque y al retroceder no es necesario cargar
int sobrecarga=0;   //varibale para indicar que el lexema ocupa mas de un bloque
int errorSobreCarga=0; //Variable para limitar las veces que aparece el error por parsarnos de tamaño
int line=0; //varibale para indicar el numero de linea


//Función para incializar los centinelas
void _inicializarBloques(){
    //inicalizamos un bloque
    centinelas[0].inicio=0;
    centinelas[0].delantero=0;
    memset(centinelas[0].contenido, '\0', tamBloque+1);
    centinelas[0].contenido[tamBloque]=EOF;

    //inicializamos el otro bloque
    centinelas[1].inicio=0;
    centinelas[1].delantero=0;
    memset(centinelas[1].contenido, '\0', tamBloque+1);
    centinelas[1].contenido[tamBloque]=EOF;

    //empezamos con uno de los bloques
    numeroCentinela=0;
}

//Función que llena un centinela
void _llenarCentinela(){
    fseek(fp, numCaracteresLeidos,SEEK_SET);//Colocamos el puntero de lectura
    memset(centinelas[numeroCentinela].contenido,'\0',tamBloque);  //Vaciamos el contenido del bloque
    numCaracteresLeidos += fread(centinelas[numeroCentinela].contenido, sizeof (char), tamBloque, fp); //leemos los caracteres y los guardamos en el bloque y devolvemos le numero de caracteres leidos correctamente
    centinelas[numeroCentinela].inicio=0;   //establecemos el inicio en la primera casilla del bloque
    centinelas[numeroCentinela].delantero=0; //establecemos el delentero en la primera casilla del bloque
}

//Función que cambia el centinela al que nos referimos
void _cambiarCentinela(){
    numeroCentinela=(numeroCentinela+1)%2; //Cambiamos el numero de seleción de centinela [0,1]
}

//Función para abrir el fichero que leemos
void _abrirFichero(char *nombreFichero){
    fp= fopen(nombreFichero, "rt"); //abrimos fichero
    if(fp==NULL){
        nuevoError(1, -1); //avisamos de error
        EXIT_FAILURE;
    }
}

//Función para cerrar el fichero que estamos leyendo
void _cerrarFichero(){
    fclose(fp); //Cerramos fichero
    fp=NULL; //igualamos a null
}


//Función para inicalizar el sistema de entrada
void iniciarSistemaEntrada(char *nombreFichero){
    //Inicializamos bloques
    _inicializarBloques();
    //Abrimos el fichero
    _abrirFichero(nombreFichero);
    //llenamos los bloques
    _llenarCentinela();
}

//Función para finalizar el sistema de entrada
void finalizarSistemaEntrada(){
    _cerrarFichero(); //llamamos a la funcion de cerrar fichero
}

//Función que devuelve un nuevo caracter
char solicitarCaracter(){
    char caracter;
    int posicion= centinelas[numeroCentinela].delantero; //guardamos la posicion en la que estamos leyendo
    caracter=centinelas[numeroCentinela].contenido[posicion]; //obtenemos el caracter
    if(caracter == '\n'){
        line++; //cambiamos de linea
    }
    if(caracter== EOF){ //si es EOF vemos que tipo es
        if(!feof(fp)){  //Si no es de fin de fichero =>
            _cambiarCentinela();    //cambiamosCentinela
            if(yaCargado==0){ //si el bloque ya estaba carcado unicamente no entramos
                if(sobrecarga==0){  //si solo ocupabamos un bloque ahora ocupariamos 2
                    _llenarCentinela(); //llenamos el centinela
                    sobrecarga=1; //marcamos la sobrecarga
                }
                else{
                    if(errorSobreCarga==0){ //si aun no di error y recubimos sobrecarga intentando volver a cargar => el lexema es muy grande
                        nuevoError(2, line);    //Avisamos de error
                        errorSobreCarga=1;
                    }
                    _llenarCentinela(); //el lexema tendra errores pq seguimos cargando pero los siguientes se formaran correctamente
                }
            }
            else{
                yaCargado=0; //cambiamos la bandera
            }
            caracter=solicitarCaracter(); //obtenemos el caracter
        }
    }
    else{
        centinelas[numeroCentinela].delantero++; //nos movemos en la lectura
    }
    return caracter;
}

//Función que retrasa el caracter que leemos
void retrasarPuntero(){
    if(centinelas[numeroCentinela].delantero!=0) { //si al retrasar seguimos en el mismo bloque
        if(centinelas[numeroCentinela].delantero>0) {
            centinelas[numeroCentinela].delantero--;    //retrsamos
        }
    }
    else{
        _cambiarCentinela(); //cambiamos de centinela
        centinelas[numeroCentinela].delantero=tamBloque-1; //nos ponemos al final
        yaCargado=1; //marcamos que ya tenemos el otro bloque cargado
    }
}

//Función que acepta (devuelve) el lexema
void aceptarLexema(componenteLexico *comp){
    int tamA, tamB ;
    comp->lexema=NULL; //establecemos a null

    if (sobrecarga==1){ //si vamos a devolver el contenido de dos bloques
        //Comprobamos los tamaños que ocupamos
        tamA= centinelas[(numeroCentinela+1)%2].delantero - centinelas[(numeroCentinela+1)%2].inicio;
        tamB= centinelas[numeroCentinela].delantero - centinelas[numeroCentinela].inicio;

        comp->lexema= (char *) malloc((tamA+tamB)*sizeof (char)+1); //reservamos memoria
        
        memset(comp->lexema, '\0', (tamA+tamB)+1); //inicializamos la variable

        strncpy(comp->lexema, centinelas[(numeroCentinela+1)%2].contenido + centinelas[(numeroCentinela+1)%2].inicio, tamA);
        strncpy(comp->lexema+tamA, centinelas[numeroCentinela].contenido + centinelas[numeroCentinela].inicio, tamB);
    }
    else{
        //Comprobamos los tamaños que ocupamos
        tamA=centinelas[numeroCentinela].delantero-centinelas[numeroCentinela].inicio;

        comp->lexema= (char *) malloc(tamBloque*sizeof (char)+1); //reservamos memoria

        memset(comp->lexema, '\0', tamA+1); //inicializamos la variable

        strncpy(comp->lexema, centinelas[numeroCentinela].contenido + centinelas[numeroCentinela].inicio, tamA);
    }
    sobrecarga=0; //establecemos los parametros por defecto para los siguientes lexemas
    errorSobreCarga=0;
    centinelas[numeroCentinela].inicio=centinelas[numeroCentinela].delantero;
}
