#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "definiciones.h"
#include "analizadorLexico.h"
#include "sistemaEntrada.h"
#include "tablaSimbolos.h"
#include "gestorErrores.h"


#define sinTIPO 0
#define HEXADECIMAL 1
#define BINARIO 2
#define normalDoble 3
#define ELEVADO 4

int tamBase = 10;
int aumento = 10;
int linea=0;

void _funcionError(int codigo, int lin) {
    nuevoError(codigo, lin);
}

void automataAlfaNumerico(char caracter, componenteLexico *comp){
    while (isalnum(caracter)||caracter=='_'){
        caracter=solicitarCaracter(); //Pido uno nuevo caracter
    }
    retrasarPuntero(); //volvemos atras una posicion el puntero
    aceptarLexema(comp); //Aceptamos el lexema
    comp->valor = buscar_insertar_Tabla(comp->lexema, ID); //buscamos en la tabla de simbolos
}

void automataNumerico(char caracter, componenteLexico *comp) {
    int tipoNumero=sinTIPO;
    int valor=INT;
    int error=0;
    int primeraVez=0;
    int yaEsDouble=0;
    int encontreElevado=0;

    while (isalnum(caracter) || caracter == '.' || caracter == '_' || caracter=='+'  || caracter=='-') {

        if(tipoNumero==sinTIPO){
            //Si aun no conocemos el tipo comprobamos si con el nuevo caracter nos decantamos por alguno
            if (caracter == 'b' || caracter == 'B') {
                tipoNumero = BINARIO;
                valor=INT;
            }
            else if(caracter == 'x' || caracter =='X'){
                tipoNumero=HEXADECIMAL;
            }
            else if(caracter=='.' ){
                tipoNumero=normalDoble;
                valor=DOUBLE;
            }
            else if(caracter == 'e' || caracter == 'E' ){
                tipoNumero=ELEVADO;
                valor=DOUBLE;
            }
            else if(isdigit(caracter) || caracter=='_'){
                tipoNumero=sinTIPO;
            }
            else{
                error=1;
                comp->valor = valor;
            }
        }

        //Si es binario sabemos que es INT -> comprobamos que este bien formado
        if(tipoNumero == BINARIO) {
            if (primeraVez > 0) {
                if (caracter != '1' && caracter != '0') {
                    error = 1;
                }
            }
            else{
                //En el caso de que sea la primera vez que vemos la b comprobamos el valor del caracter que hay antes pues tiene que ser 0
                primeraVez++;
                retrasarPuntero();
                retrasarPuntero();
                if(solicitarCaracter()!='0'){
                    error = 1;
                }
                solicitarCaracter();
            }
        }

        //Si es hexadecima puede ser INT o DOUBLE -> comprobamos el tipo y si está bien formado
        else if(tipoNumero==HEXADECIMAL){
            if(primeraVez>0){
                if (!isdigit(caracter) && caracter!='A'&& caracter!='B'&& caracter!='C'&& caracter!='D'&& caracter!='E'&& caracter!='F' && caracter!='p' && caracter!='P' && caracter!='+' && caracter!='-') {
                    error = 1;
                }
            }
            else{
                //En el caso de que sea la primera vez que vemos la X comprobamos el valor del caracter que hay antes pues tiene que ser 0
                primeraVez++;
                retrasarPuntero();
                retrasarPuntero();
                if(solicitarCaracter()!='0'){
                    error = 1;
                }
                solicitarCaracter();
            }

            if(error==0) {  //Si no hay error
                if (yaEsDouble == 0) { //Vemos si ya esta en double
                    if (caracter != 'p' && caracter != 'P') { //comprobamos que con este caracter no sea double
                        valor = INT;
                    } else {   // o Cambiamos a double
                        valor = DOUBLE;
                        yaEsDouble = 1;
                    }
                }

                if (yaEsDouble == 1) { //Si ya es double
                    if(encontreElevado==0){ //Si no tengo el elevado veo si lo tengo
                        if (caracter == 'p' || caracter == 'P') {
                            encontreElevado ++;
                        }
                        else if (!isdigit(caracter) && caracter!='A'&& caracter!='B'&& caracter!='C'&& caracter!='D'&& caracter!='E'&& caracter!='F' && caracter!='p' && caracter!='P') { //Si no estña elevado solo podemos tener esto caracteres
                            error = 1;
                        }
                    }
                }
            }
        }

        //En el caso de ser un elevado con 'e' sabemos que es DOUBLE -> comprobamos que este bien formado
        else if(tipoNumero==ELEVADO){
            if(encontreElevado==0){
                if (caracter == 'e' || caracter == 'E') {
                    encontreElevado ++;
                }
            }
            else if( encontreElevado>0){
                if (!isdigit(caracter) && caracter!='+' && caracter!='-') { //Solo si encontre el elevado puedo poner +-
                    error = 1;
                }
            }
        }

        //En el caso de ser decimal Doble -> comprobamos que esté bien formado.
        else if(tipoNumero==normalDoble){
            if (primeraVez > 0) {
                if(caracter == 'e' || caracter == 'E'){ //si encontramos elevado cambiamos a elevado para comprobar que este bien formado
                    tipoNumero=ELEVADO;
                    encontreElevado=1;
                }
                else if (!isdigit(caracter)) {
                    error = 1;
                }
            }
            else{
                primeraVez++; //guardo el indicador de double '.'
            }

        }

        //Solicitamos un nuevo caracter
        caracter = solicitarCaracter();
    }

    comp->valor = valor;    //incluimos el tipo de nuemero
    retrasarPuntero();      //retrasamos el puntero de lectura de caracter
    aceptarLexema(comp);    //aceptamos el lexema

    //En el caso de tener algún error lo reflejamos en el .log
    if(error==1) {
        _funcionError(0, linea);
    }
}

void automataComentariosAnidados(){
    int nAnidados=1;
    int fin=0;
    char caracter;

    while(fin==0){
        caracter=solicitarCaracter(); //Pido uno nuevo

        //POSIBLE FIN
        if(caracter=='+'){
            caracter=solicitarCaracter();
            if(caracter=='/'){
                nAnidados-=1;
                caracter=solicitarCaracter();
            }
        }

        //POSIBLE ANIDADO
        if(caracter=='/'){
            caracter=solicitarCaracter();
            if(caracter=='+'){
                nAnidados+=1;
            }
        }

        if(nAnidados==0){
            fin=1;
        }
    }
    retrasarPuntero();
}

void automataComentariosNormales(){
    int fin=0;
    char caracter;

    while(fin==0){
        caracter=solicitarCaracter(); //Pido uno nuevo

        //POSIBLE FIN
        if(caracter=='*'){
            caracter=solicitarCaracter();
            if(caracter=='/'){
                fin=1;
            }
        }
    }
}

void automataComentariosLinea(){
    int fin=0;;
    char caracter;
    while(fin==0){
        caracter=solicitarCaracter(); //Pido uno nuevo
        //FiN
        if(caracter=='\n'){
            fin=1;
        }
    }
}

void operadoresUnitarios(char caracter, componenteLexico *comp){
    aceptarLexema(comp); //Aceptamos el lexema
    comp->valor= caracter; //Le damos un valor
}

void cadenaCaracteres(char caracter, componenteLexico *comp){
    int fin=0;
    int escapado=0;

    while(fin==0){
        caracter=solicitarCaracter(); //Pido uno nuevo

        //En el caso de tener un caracter de escape pido otro y pongo que ya no hay caracter de escape
        if(escapado==1){
            caracter=solicitarCaracter(); //Pido uno nuevo;
            escapado=0;
        }
        //POSIBLE ESCAPE
        if(escapado==0 && caracter=='\\') {
            escapado = 1;
        }
        //FINAL del STRING
        if(escapado==0){
            if(caracter=='"'){
                fin=1;
            }
        }
    }

    aceptarLexema(comp); //aceptamos el lexema
    comp->valor=STRING; //ponemos el valor de STRING
}

int automataOeradores(char caracter, componenteLexico *comp){
    int estado=0;
    switch (caracter) {
        case '/':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='+'){
                automataComentariosAnidados(); //tendiramos un comentario anidado
                estado=0; //Pasamos al estado final
            }
            else if(caracter=='*'){
                automataComentariosNormales(); //tendríamos un comentario normal
                estado=0; //Pasamos al estado final
            }
            else if(caracter=='/'){
                automataComentariosLinea(); //tendríamos un comentario de linea
                estado=0;
            }
            else if(caracter=='='){
                comp->valor=DIVISION_IGUAL;
                estado=-1;
            }
            else{
                comp->valor='/';
                retrasarPuntero();
                estado=-1;
            }
            break;
        case'&':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=AMPERSAN_IGUAL;
            }
            else if(caracter=='&'){
                comp->valor=AND;
            }
            else{
                retrasarPuntero();
                comp->valor='&';
            }
            estado=-1;
            break;
        case'|':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=BARRA_IGUAL;
            }
            else if(caracter=='|'){
                comp->valor=OR;
            }
            else{
                retrasarPuntero();
                comp->valor='|';
            }
            estado=-1;
            break;
        case'-':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=MENOR_IGUAL;
            }
            else if(caracter=='-'){
                comp->valor=MENOS_MENOS;
            }
            else{
                retrasarPuntero();
                comp->valor='-';
            }
            estado=-1;
            break;
        case'+':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=MAS_IGUAL;
            }
            else if(caracter=='+'){
                comp->valor=MAS_MAS;
            }
            else{
                retrasarPuntero();
                comp->valor='+';
            }
            estado=-1;
            break;
        case'<':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=MENOR_IGUAL;
            }
            else{
                retrasarPuntero();
                comp->valor='<';
            }
            estado=-1;
            break;
        case'>':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=MAYOR_IGUAL;
            }
            else{
                retrasarPuntero();
                comp->valor='>';
            }
            estado=-1;
            break;
        case'!':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=DISTINTO;
            }
            else{
                retrasarPuntero();
                comp->valor='!';
            }
            estado=-1;
            break;
        case'=':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=IGUAL_IGUAL;
            }
            else{
                retrasarPuntero();
                comp->valor='=';
            }
            estado=-1;
            break;
        case'*':
            caracter= solicitarCaracter(); //Pedimos el caracter
            if(caracter=='='){
                comp->valor=POR_IGUAL;
            }
            else{
                retrasarPuntero();
                comp->valor='*';
            }
            estado=-1;
            break;
    }
    aceptarLexema(comp);
    return estado;



}


void siguienteComponenteLexico(componenteLexico *comp){
    int fin=0;
    int estado=0;
    char caracter = '\0';

    while(!fin){
        switch (estado) {
            //ESTADO INICIAL AFD
            case 0:

                if(comp->lexema!=NULL){
                    //LIBERAMOS MEMORIA
                    free(comp->lexema);
                    comp->lexema=NULL;
                }            

                caracter= solicitarCaracter(); //Pedimos un caracter

                //Comprobamos si es alguno de estos caracteres que ignoramos
                if(caracter==' ' || caracter == '\t' || caracter == '\n'){
                    if(caracter == '\n'){   //En el caso de saltar de linea debemos tenerlo en cuenta para lelvar la cuenta en el ficheroErrores.
                        linea++;
                    }
                    aceptarLexema(comp); //Aceptamos el el lexema
                    estado=0; //Seguimos en este estado porque no tenemos que hacer nada con estos caracteres.
                }

                //Comprobamos si es una cadena Alfanumeria
                else if(isalpha(caracter)||caracter=='_'){
                    automataAlfaNumerico(caracter, comp); //Llamamos al automata alfanumerico
                    estado=-1; //Pasamos al estado final
                }

                //Comprobamos si es una cadena de Numeros
                else if(isdigit(caracter)){
                    automataNumerico(caracter, comp);
                    estado=-1;
                }

                //Punto-->Si los numeros NO pueden empezar por punto elimino este IF y veo que es un punto
                else if(caracter=='.'){
                    caracter= solicitarCaracter(); //Pedimos el caracter
                    if(isdigit(caracter)){
                        retrasarPuntero();
                        retrasarPuntero();
                        caracter=solicitarCaracter();
                        automataNumerico(caracter, comp);
                        estado=-1;
                    }
                    else{
                        retrasarPuntero();
                        retrasarPuntero();
                        caracter=solicitarCaracter();
                        operadoresUnitarios(caracter, comp);
                        estado=-1;
                    }
                }

                //Operadores unitarios
                else if(caracter=='('||caracter==')'||caracter=='{'||caracter=='}'||caracter=='['||caracter==']'||caracter==';'||caracter==','||caracter=='?'||caracter==':'||caracter=='$'||caracter=='@' ||caracter=='#'){
                    operadoresUnitarios(caracter, comp);
                    estado=-1;
                }

                //Cadenas STRINGS
                else if(caracter=='"'){
                    cadenaCaracteres(caracter, comp);
                    estado=-1;
                }
                //Cambiamos pq hay diferentes posibilidades.
                else if(caracter=='/'){
                    estado= automataOeradores(caracter, comp);
                }

                //Caracter final
                else if( caracter == EOF){
                    comp->valor=EOF;
                    estado=-1;
                }

                //Cualquier otro caracter que no sea numero ni letra y no encaje con los anteriores.
                else if(!isdigit(caracter) && !isalpha(caracter)){
                    estado= automataOeradores(caracter, comp);
                }
                break;

            default:
                fin=1;
                break;
        }
    }
}