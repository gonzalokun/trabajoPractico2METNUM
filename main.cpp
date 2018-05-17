#include <iostream>
#include "cargadorDeImagenes.h"

int main(int cantidadDeArgumentos, char** argumentos) {

    //argumentos: ./tp2 -m <METHOD> -i <TRAINSET> -q <TESTSET> -o <CLASSIF>
    //Hacer un ciclodespues

    //std::cout << argumentos[4] << std::endl;

    //Probamos el cargador
    cargadorDeImagenes c(argumentos[4]);

    return 0;
}