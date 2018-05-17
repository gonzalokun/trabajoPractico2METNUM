//
// Created by Gonzalo on 17/05/2018.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "cargadorDeImagenes.h"
#include "lib/ppmloader.h"

cargadorDeImagenes::cargadorDeImagenes(const char *archivo) {
    std::ifstream listaDeNombres(archivo);

    char *nombreArchivoDeImagen;
    int IDPersona;

    //HAY QUE VER SI ESTO FUNCA

    std::cout << "CONSTRUCTOR ANTES DE CICLO" << std::endl;

    std::ostringstream conversor;

    std::string delimiter = ",";

    for(std::string linea; std::getline(listaDeNombres, linea); ){
        std::cout << linea << std::endl;

        //Separamos la linea

    }

    listaDeNombres.close();
}

void cargadorDeImagenes::cargarImagen(const char *archivo, int IDPersona) {
    //NADA POR AHORA
}

std::vector< std::pair<std::vector<int>, int> > cargadorDeImagenes::conjuntoDeImagenes() {
    return _imagenes;
}
