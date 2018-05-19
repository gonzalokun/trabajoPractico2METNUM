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

//template <class T>
//Devuelve el vector mu con la media de los vectores del conjunto
std::vector<double> obtenerMedia(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes);

//Calcula y Devuelve la matriz M (calcula los vectores que la forman)
std::vector<std::vector<double>> obtenerMatrizM(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes, std::vector<double> vectorMedia);

//Calcula los autovalores y autovectores de la matriz ingresada usando el metodo de la potencia
//Devuelve un vector de pares que los contiene en el siguiente formato <autovalor, autovector asociado>
std::vector<std::pair<double, std::vector<double>>> calcularAutovalores(std::vector<std::vector<double>> matrizM);

//Calcula la transformacion caracteristica de la imagen (ya vectorizada) con los autovectores de la matriz
std::vector<double> transformacionCaracteristica(std::vector<std::vector<double>> autovectores, std::vector<double> imagenVectorizada);

#endif //TRABAJOPRACTICO2METNUM_MODULOPCA_H
