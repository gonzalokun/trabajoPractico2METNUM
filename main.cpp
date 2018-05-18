#include <iostream>
#include <fstream>
#include "cargadorDeImagenes.h"

int main(int cantidadDeArgumentos, char** argumentos) {

    //argumentos: ./tp2 -m <METHOD> -i <TRAINSET> -q <TESTSET> -o <CLASSIF>
    //Hacer un ciclodespues

    //std::cout << argumentos[4] << std::endl;

    //Probamos el cargador
    cargadorDeImagenes c(argumentos[4]);


    //PARTE DONDE PRUEBO COSAS

//    FILE* fid = fopen("full/s1/1.pgm", "rb");
//
//    std::cout << "SE APUNTO" << std::endl;
//
//    if(fid == NULL){
//        std::cout << "FALLÓ LA APERTURA" << std::endl;
//    }
//    else{
//        std::cout << "Se pudo abrir!" << std::endl;
//    }
//
//    fclose(fid);

    return 0;
}