/*
 * Estructura de almacenamiento de componente lexico
 * */
typedef struct {
    char *lexema;
    int valor;
}componenteLexico;




/*
 * Función que devuelve el siguiente componente lexico.
 * */
void siguienteComponenteLexico(componenteLexico *comp);