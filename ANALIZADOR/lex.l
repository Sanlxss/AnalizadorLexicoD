/*DEFINICIONES*/
%{
#include "definiciones.h"
#include "tablaSimbolos.h"
#include "gestorErrores.h"

typedef struct {
    char *lexema;
    int valor;
}componenteLexico;


void abrirFichero(char *nombreFichero);
void siguienteComponenteLexico(componenteLexico *comp);
void cerrarFichero();

int nComentariosAnidados=0;

%}
%option yylineno
%option noyywrap
%option nounput
%option noinput

%x COMMENT

/*TIPOS DE SEPARADORES*/
SEPARADORES (\t|\n|\r|[ ])
/*----------------------------------------------------------------------*/

/*CADENAS DE CARACTERES*/
IDENTIFICADOR [a-zA-Z_][a-zA-Z0-9_]*
/*----------------------------------------------------------------------*/

/*NUMEROS*/
DECIMAL [0-9]
BINARIO [0-1]

/*integers*/
DECIMAL_INT {DECIMAL}(([_])?{DECIMAL})*
BINARIO_INT (0[bB]({BINARIO})*)
INT {DECIMAL_INT}|{BINARIO_INT}

/*doubles*/
EXPONENTE_DECIMAL [eE][+-]?({DECIMAL_INT})
DECIMAL_DOUBLE_T1 {DECIMAL_INT}\.{DECIMAL_INT}?{EXPONENTE_DECIMAL}?
DECIMAL_DOUBLE_T2 {DECIMAL_INT}{EXPONENTE_DECIMAL}
DECIMAL_DOUBLE_T3 \.{DECIMAL_INT}{EXPONENTE_DECIMAL}?
DECIMAL_DOUBLE {DECIMAL_DOUBLE_T1}|{DECIMAL_DOUBLE_T2}|{DECIMAL_DOUBLE_T3}
/*----------------------------------------------------------------------*/

/*OPERADORES*/
OPERADOR_SIMPLE [/&|+=<>!*(){}[\];,.:$-]

AND &&
OR \|\|
MENOS_IGUAL -=
MENOS_MENOS --
MAS_IGUAL \+=
MAS_MAS \+\+
MENOR_IGUAL <=
MAYOR_IGUAL >=
DISTINTO !=
IGUAL_IGUAL ==
/*----------------------------------------------------------------------*/

/*COMENTARIOS*/
COMENTARIO_LINEA \/\/.*
COMENTARIO_BLOQUE \/\*([^*]|[\n\r]|(\*+([^*/]|[\n\r])))*\*+\/

COMENTARIO_ANIDADO_INICIO \/\+
COMENTARIO_ANIDADO_FIN \+\/
/*----------------------------------------------------------------------*/

/*STRINGS*/
STRING_FORMA \"(\\.|[^\\\"])*\"
/*----------------------------------------------------------------------*/

%%
{SEPARADORES}
{IDENTIFICADOR} {return(ID);}
{INT} {return(INT);}
{DECIMAL_DOUBLE} {return(DOUBLE);}
{COMENTARIO_LINEA}
{COMENTARIO_BLOQUE}
{OPERADOR_SIMPLE} {return((int)*yytext);}
{AND} {return(AND);}
{OR} {return(OR);}
{MENOS_IGUAL} {return(MENOS_IGUAL);}
{MENOS_MENOS} {return(MENOS_MENOS);}
{MAS_IGUAL} {return(MAS_IGUAL);}
{MAS_MAS} {return(MAS_MAS);}
{MENOR_IGUAL} {return(MENOR_IGUAL);}
{MAYOR_IGUAL} {return(MAYOR_IGUAL);}
{DISTINTO} {return(DISTINTO);}
{IGUAL_IGUAL} {return(IGUAL_IGUAL);}
{STRING_FORMA} {return(STRING);}


{COMENTARIO_ANIDADO_INICIO} {nComentariosAnidados++;BEGIN(COMMENT);}
<COMMENT>{COMENTARIO_ANIDADO_INICIO} {nComentariosAnidados++;}
<COMMENT>{COMENTARIO_ANIDADO_FIN} {nComentariosAnidados--;if(nComentariosAnidados==0) BEGIN(INITIAL);}
<COMMENT>.
<COMMENT>{SEPARADORES}

<<EOF>> {return(-1);}

. {return(-3);}

%%



/*CODIGO USUARIO*/
//Función para abrir el fichero que leemos
void abrirFichero(char *nombreFichero){
    yyin= fopen(nombreFichero, "rt"); //abrimos fichero
    if(yyin==NULL){
        nuevoError(1, -1); //avisamos de error
        EXIT_FAILURE;
    }
}
//Funcion para solicitar y devolver el siguiente componente lexico
void siguienteComponenteLexico(componenteLexico *comp){
    
    comp->valor = yylex();

    if(comp->lexema!=NULL){
        //LIBERAMOS MEMORIA
        free(comp->lexema);
        comp->lexema=NULL;
    }

    comp->lexema= (char *) malloc(strlen(yytext)*sizeof (char)+1); //reservamos memoria   
    memset(comp->lexema, '\0', strlen(yytext)+1); //inicializamos la variable
    strncpy(comp->lexema, yytext, strlen(yytext)); //Copiamos el lexema

    if(comp->valor == ID){
        comp->valor = buscar_insertar_Tabla(comp->lexema, ID);
    }
    if(comp->valor == -3){
        nuevoError(0, yylineno);
    }

}


//Función para cerrar el fichero que estamos leyendo
void cerrarFichero(){
    fclose(yyin); //Cerramos fichero
    yyin=NULL; //igualamos a null
    yylex_destroy();
}