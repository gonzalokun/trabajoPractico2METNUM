//
// Created by Gonzalo on 16/05/2018.
//

#ifndef TRABAJOPRACTICO2METNUM_MODULOPCA_H
#define TRABAJOPRACTICO2METNUM_MODULOPCA_H

#include <vector>

//class moduloPCA{
//    public:
//        moduloPCA(std::vector<std::pair<std::vector<unsigned char>, int>> conjuntoDeImagenes);
//        //
//
//    private:
//        std::vector<std::pair<std::vector<double>, int>> _conjutoImagenes;
//        std::vector<std::vector<double >> _matriz
//        std::vector< std::pair<std::vector<double>, double>> _obtenerAutovalores(); //PASARLE LA MATRIZ M
//};

//namespace PCA{
class PCA{
public:
    //template <class T>
    //PCA(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes);

    //Devuelve el vector mu con la media de los vectores del conjunto
    std::vector<double> obtenerMedia(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes);

    //Calcula y Devuelve la matriz M (calcula los vectores que la forman)
    std::vector<std::vector<double>> obtenerMatrizM(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes, std::vector<double> vectorMedia);

    //Devuelve la lista de autovectores con los autovalores relacionados como
    //<autovector, autovalor>
    std::vector<std::pair<std::vector<double>, double >> calcularAutovalYAutoVec(std::vector<std::vector<double>> matrizM);

    //Calcula la transformacion caracteristica de la imagen (ya vectorizada) con los autovectores de la matriz
    std::vector<double> transformacionCaracteristica(std::vector<std::vector<double>> autovectores, std::vector<double> imagenVectorizada, int alfa);

private:

    std::vector<double> multiplicarMatrizVector(std::vector<std::vector<double>> &matriz, std::vector<double> &vec) const;

    std::vector<std::vector<double>> transponerMatriz(std::vector<std::vector<double>> matriz);

    //Calcula los autovalores de una matriz con el metodo de la potencia
    double calcularAutovalor(std::vector<std::vector<double>> matrizM, std::vector<double> autovector);

    //Calcula los autovectores de una matriz en base a sus autovalores
    std::vector<std::vector<double>> calcularAutovectores(std::vector<double> autovalores, std::vector<std::vector<double>> matrizM);
};

#endif //TRABAJOPRACTICO2METNUM_MODULOPCA_H
