#include <iostream>
#include <fstream>
#include "cargadorDeImagenes.h"
#include "moduloPCA.h"

int main(int cantidadDeArgumentos, char** argumentos) {

    //argumentos: ./tp2 -m <METHOD> -i <TRAINSET> -q <TESTSET> -o <CLASSIF>
    //Hacer un ciclodespues

    //std::cout << argumentos[4] << std::endl;

    //Probamos el cargador
    cargadorDeImagenes c(argumentos[4]);
    cargadorDeImagenes testeo("imagenesATestear.in");

    //Creo la variable que maneje el PCA
    PCA modPCA;

    std::vector<double> mediaBase = modPCA.obtenerMedia(c.conjuntoDeImagenes());

    std::vector<std::vector<double>> matrizM = modPCA.obtenerMatrizM(c.conjuntoDeImagenes(), mediaBase);

    int alfa = 10;

    std::vector<std::pair<std::vector<double>, double >> autovecYAutoval = modPCA.calcularAutovalYAutoVec(matrizM, alfa, 0.01);

//    for(int i = 0; i < autovecYAutoval.size(); i++){
//        std::cout << "Autovalor " << i <<":" << autovecYAutoval[i].second << std::endl;
//    }

    std::vector<std::vector<double>> autovectores(autovecYAutoval.size());

    for(int i = 0; i < autovecYAutoval.size(); i++){
        autovectores[i] = autovecYAutoval[i].first;
    }

    std::vector<double> imagenSinTC = testeo.conjuntoDeImagenes()[0].first;

    std::vector<double> imagenTC = modPCA.transformacionCaracteristica(autovectores, imagenSinTC);

    std::cout << "FIN DE PROGRAMA" << std::endl;

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