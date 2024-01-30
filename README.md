# Analizador Lexico D languaje

Se ha desarrollado un analizador léxico para la asignatura de Compiladores e Interpretes del grado de Ingenieria Informática de la USC.

La información relativa al lenguaje que es capaz de procesar este analizador lexico se encuentra en la siguiente pagina:
https://dlang.org/spec/lex.html


## Compilación.
Para compilar el analizador lexico se descargará este directorio y se guardarán en el todos los archivos, incluido el fichero con el lenguaje que se quiere analizar.
A continuación desde un terminal de Linux, se ejecutará el comando 'make'.
En el caso de querer deshacer la compilacion (borrar todos los fichero resultantes de estas) se ejecutará el comando 'make cleanall'.


## Ejecución
Para la ejecución, una vez compilado el analizador, se ejecutará en el terminal el siguiente comando './analizador {fichero-CodigoFuente-D}'

El resultado de la ejecución es el siguiente:
-   En primer lugar se mostrará la tabla de simbolos inicial, antes de que se realice el analisis.
-   A continuación se imprimiran uno a uno todos los componentes lexicos que se encuentren en el codigo fuente proporcionado.
-   Por último, se volvera a imprimir la tabla de simbolos una vez se hayan introducido en ella todos los identificadores encontrados.