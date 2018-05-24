#include <math.h>
#include <random>
#include <stdexcept>
#include <iostream>
#include "moduloPCA.h"

std::vector<double> PCA::obtenerMedia(const std::vector<std::pair<std::vector<double>, int>> &conjuntoDeImagenes) const{
    int cantidadDeVectores = conjuntoDeImagenes.size();
    std::vector<double> promedioVectores(conjuntoDeImagenes[0].first.size(), 0);

    //Hago la suma de los vectores sobre la cantidad total de vectores
    for(int i = 0; i < cantidadDeVectores; i++){
        //
        for(int j = 0; j < conjuntoDeImagenes[i].first.size(); j++){
            promedioVectores[j] += (conjuntoDeImagenes[i].first[j] / cantidadDeVectores);
        }
    }

    return promedioVectores;
}

std::vector<std::vector<double>> PCA::obtenerMatrizM(
        const std::vector<std::pair<std::vector<double>, int>> &conjuntoDeImagenes, const std::vector<double> &vectorMedia) {
    unsigned int cantidadDeVectores = conjuntoDeImagenes.size();

    //Cant elementos de cada vector
    unsigned int cantidadDeColumnas = conjuntoDeImagenes[0].first.size();
    std::vector<std::vector<double>> matrizX;

    //Lleno la matriz X con los datos que corresponden
    for(int fila = 0; fila < cantidadDeVectores; fila++){
        std::vector<double> filaActual(cantidadDeColumnas, 0);

        for(int columna = 0; columna < cantidadDeColumnas; columna++){
            filaActual[columna] = (conjuntoDeImagenes[fila].first[columna] - vectorMedia[columna]) / sqrt((double) cantidadDeVectores - 1);
        }

        matrizX.push_back(filaActual);
    }

    //Deberia estar formada la matrizX

    //Ahora calculo la transpuesta
    //Si matrizX es cantidadDeVectores * cantidadDeColumnas -> matrizXt es cantidadDeColumnas * cantidadDeVectores
    std::vector<std::vector<double>> matrizXt(transponerMatriz(matrizX));

    //Ahora puedo calcular la matriz M haciendo Xt * X
    //M va a ser de tamaño cantidadDeColumnas * cantidadDeColumnas
    std::vector<std::vector<double>> matrizM(cantidadDeColumnas, std::vector<double>(cantidadDeColumnas, 0));

    //Multiplico Xt * X
    for(int fila = 0; fila < cantidadDeColumnas; fila++){
        for(int columna = 0; columna < cantidadDeColumnas; columna++){
            for(int k = 0; k < cantidadDeVectores; k++){
                matrizM[fila][columna] += matrizXt[fila][k] * matrizX[k][columna];
            }
        }
    }

    //Ya esta formada la matriz M
    return matrizM;
}

std::vector<std::pair<std::vector<double>, double >> PCA::calcularAutovalYAutoVec(
        const std::vector<std::vector<double>> &matrizM, int alfa, double tolerancia, int limiteCiclos){
    //Solo calculo los necesarios
    std::vector<std::pair<std::vector<double>, double >> autoVecYAutoval(alfa);

    //Copio la matriz pasada para poder aplicar el metodo de las potencias
    std::vector<std::vector<double>> matrizAux(matrizM);

    std::cout << "TAM DE LA MATRIZ: " << matrizM.size() << "x" << matrizM[0].size()  << std::endl;

    //Para cada autovalor
    for(int i = 0; i < alfa; i++){

        std::cout << "Iteracion i: " << i << std::endl;

        //Guardo la cantidad de filas
        int filas = matrizM.size();

        //Creo un vector al azar
        std::vector<double> vectorX = generarVectorAlAzar(filas);

        //Creo un vector para poder comparar las distintas iteraciones
        std::vector<double> vectorAnterior = vectorX;

        //La distancia inicial de los dos vectores
        double distanciaVectores = tolerancia;

        int ciclosRealizados = 0;

        //Cuando se acerquen los vectores se habra llegado al autovector
        while(distanciaVectores >= tolerancia && ciclosRealizados < limiteCiclos){

            vectorX = multiplicarMatrizVector(matrizAux, vectorX);

            double normaVector = 1 / (calcularNorma(vectorX));

            for(int j = 0; j < vectorX.size() ; j++){
                vectorX[j] = vectorX[j] * normaVector;
            }

            std::vector<double> resta(vectorX.size());

            for(int j = 0; j < vectorX.size(); j++){
                resta[j] = vectorX[j] - vectorAnterior[j];
            }

            distanciaVectores = calcularNorma(resta);

            std::cout << "DISTANCIAVECTORES: " << distanciaVectores  << std::endl;

            vectorAnterior = vectorX;

            ciclosRealizados++;
        }

        //Tengo el autovector, ahora calculo el autovalor
        double autovalor = calcularAutovalor(matrizM, vectorX);

        //Los guardo
        autoVecYAutoval[i].first = vectorX;
        autoVecYAutoval[i].second = autovalor;

        //Uso deflacion para poder buscar los proximos autovec y autoval
        //A tengo que hacer matrizAux = matrizAux - autovalor * autovector * autovector^t

        //Construyo autovalor autovector * autovector^t
        std::vector<std::vector<double>> matrizResta(vectorX.size(), std::vector<double>(vectorX.size()));

        for(int fila = 0; fila < matrizResta.size() ; fila++){
            for(int columna = 0; columna < matrizResta[fila].size(); columna++){
                matrizResta[fila][columna] = autovalor * vectorX[fila] * vectorX[columna];

            }
        }

        //Se lo resto a matrizAux
        for(int fila = 0; fila < matrizAux.size() ; fila++){
            for(int columna = 0; columna < matrizAux[fila].size(); columna++){
                matrizAux[fila][columna] = matrizAux[fila][columna] - matrizResta[fila][columna];
            }
        }

        //Deberia funcionar
    }

    return autoVecYAutoval;
}

std::vector<double> PCA::generarVectorAlAzar(int &dimension) {
    std::vector<double> vecAlAzar(dimension, 0);

    //De 0 a 255 (no deberia importar mucho este rango)
    for(int i = 0; i < dimension; i++){
        vecAlAzar[i] = rand() % 256;
    }

    return vecAlAzar;
}

double PCA::calcularNorma(std::vector<double> vector) {
    double suma = 0;

    for(int i = 0; i < vector.size(); i++){
        suma += vector[i] * vector[i];
    }

    suma = sqrt(suma);

    return suma;
}

std::vector<std::vector<double>> PCA::transponerMatriz(const std::vector<std::vector<double>> &matriz) const{
    //Calculo la transpuesta

    //Si matriz es matriz.size * matriz[i].size (para cualquier i) -> matrizT es matriz[i].size * matriz.size
    std::vector<std::vector<double>> matrizT(matriz[0].size(), std::vector<double>(matriz.size()));

    for(int fila = 0; fila < matriz.size(); fila++){
        //
        for(int columna = 0; columna < matriz[0].size(); columna++){
            matrizT[columna][fila] = matriz[fila][columna];
        }
    }

    return matrizT;
}

//Hace la multiplicacion A*x donde A es matriz y x vector, el resultado es normalizado
std::vector<double> PCA::multiplicarMatrizVector(const std::vector<std::vector<double>> &matriz, const std::vector<double> &vec) const{
    if(matriz[0].size() != vec.size()){
        throw std::runtime_error("EL TAMAÑO DE LA MATRIZ Y EL VECTOR NO COINCIDEN");
    }

    //Se puede multiplicar
    std::vector<double> resultado(vec.size(), 0);

    for(int elem = 0; elem < resultado.size(); elem++){

        double sum = 0;

        for(int k = 0; k < resultado.size(); k++){
            sum += matriz[elem][k] * vec[k];
        }

        resultado[elem] = sum;
    }

    return resultado;
}

//Calcula estimado del autovalor con el Rayleigh Quotient
//si autovector es autovector de matrizM entonces esta funcion devuelve el autovalor exacto
double PCA::calcularAutovalor(const std::vector<std::vector<double>> &matrizM, const std::vector<double> &autovector) const{
    //Tengo que calcular:
    // (matrizM * autovector)^t * autovector
    // autovector^t * autvector

    //(matrizM * autovector)
    std::vector<double> mulAv = multiplicarMatrizVector(matrizM, autovector);

    double vtv = 0;
    // autovector^t * autvector
    for(int i = 0; i < autovector.size(); i++){
        vtv += autovector[i] * autovector[i];
    }

    //(matrizM * autovector)^t * autovector

    double Avtv = 0;
    for(int i = 0; i < mulAv.size(); i++){
        Avtv += mulAv[i] * autovector[i];
    }

    // ((matrizM * autovector)^t * autovector) / (autovector^t * autvector)
    return (Avtv / vtv);
}

std::vector<double> PCA::transformacionCaracteristica(std::vector<std::vector<double>> &autovectores,
                                                      std::vector<double> &imagenVectorizada) {
    std::vector<double> transformacionCaracteristica(autovectores.size(), 0);

    for(int i = 0; i < autovectores.size(); i++){
        for(int k = 0; k < imagenVectorizada.size(); k++){
            transformacionCaracteristica[i] += autovectores[i][k] * imagenVectorizada[k];
        }
    }

    return transformacionCaracteristica;
}
