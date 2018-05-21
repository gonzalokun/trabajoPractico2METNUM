#include <math.h>
#include <stdexcept>
#include "moduloPCA.h"

//PCA::PCA(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes) {
//    //
//}

std::vector<double> PCA::obtenerMedia(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes) {
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
        std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes, std::vector<double> vectorMedia) {
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
    std::vector<std::vector<double>> matrizXt(cantidadDeColumnas, std::vector<double>(cantidadDeVectores));

    for(int fila = 0; fila < cantidadDeVectores; fila++){
        //
        for(int columna = 0; columna < cantidadDeColumnas; columna++){
            matrizXt[columna][fila] = matrizX[fila][columna];
        }
    }

    //Ahora puedo calcular la matriz M haciendo Xt*X
    //M va a ser de tamaño cantidadDeColumnas * cantidadDeColumnas
    std::vector<std::vector<double>> matrizM(cantidadDeColumnas, std::vector<double>(cantidadDeColumnas, 0));

    //Multiplico X * Xt
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

//std::vector<std::vector<double>> PCA::calcularAutovectores(std::vector<double> autovalores,
//                                                           std::vector<std::vector<double>> matrizM) {
//    //
//}

std::vector<std::pair<std::vector<double>, double >> PCA::calcularAutovalYAutoVec(
        std::vector<std::vector<double>> matrizM) {
    //Acá guardo los autovec y los autoval, va a haber n autovec (y autoval)
    int tamMatriz = matrizM.size();
//    std::vector<std::pair<std::vector<double>, double >> paresAutoVecYAutoVal(tamMatriz, std::make_pair(std::vector<double>(tamMatriz), 0));
    std::vector<std::pair<std::vector<double>, double >> paresAutoVecYAutoVal;
    double tolerancia = 0.01;

    //Para cada autovalor
    for(int i = 0; i < tamMatriz; i++){
        //Por cada iteracion de este ciclo tengo que hallar un par <autovector, autovalor>

        //Hago un vector al azar normalizado (POR AHORA AGARRO EL DE SOLO UNOS)
        std::vector<double> vectorX(tamMatriz, (1.0 / tamMatriz));

        //Vector sobre el cual voy a iterar
        std::vector<double> vectorIteracion(tamMatriz, 0);

        double autovalX = calcularAutovalor(matrizM, vectorX);
        double autovalIter;
        bool encontrado = false;

        //Multiplico a la matriz por derecha por el vector hasta que no cambie mucho (se acerca cada vez mas)
        do{
            //Igualo el vectorIteracion al vector resultante de multiplicar la matriz por vectorX
            vectorIteracion = multiplicarMatrizVector(matrizM, vectorX);

            //Calculo el Rayleigh Quotient del autovec
            autovalIter = calcularAutovalor(matrizM, vectorIteracion);

            //Si los autovalores calculados estan muy cerca, entonces el vector no cambio mucho y nos estamos
            //acercando al autovalor real
            if(fabs(autovalX - autovalIter) < tolerancia){
                //Estan cerca
                encontrado = true;
            }
            else{
                //No estan cerca, actualizo el vector y el autovalor para seguir el ciclo
                autovalX = autovalIter;
                vectorX = vectorIteracion;
            }

        }while(!encontrado);

        //Ahora vectorIteracion y autovalIter son el par autovec, autoval correspondiente
        //paresAutoVecYAutoVal.push_back(std::make_pair(vectorIteracion, autovalIter));
        paresAutoVecYAutoVal.emplace_back(vectorIteracion, autovalIter);

        //std::vector<std::vector<double>> matrizResta(vectorIteracion.size(), std::vector<double>(vectorIteracion.size()));
        //Le saco el autovector y el autovalor a la matriz ingresada
        //Para eso creo la matriz autovalIter * vectorIteracion^t * vectorIteracion
        //Puedo restarle los valores a matrizM directamente
        for(int fila = 0; fila < vectorIteracion.size(); fila++){
            for(int columna = 0; columna < vectorIteracion.size() ; columna++){
                //matrizResta[fila][columna] = autovalIter * vectorIteracion[fila] * vectorIteracion[columna];
                matrizM[fila][columna] -= autovalIter * vectorIteracion[fila] * vectorIteracion[columna];
            }
        }

        //Deberia estar modificada la matrizM en los lugares correctos para seguir calculando los autovec y autoval

    }

    //Se calculó todo

    return paresAutoVecYAutoVal;
}

std::vector<std::vector<double>> PCA::transponerMatriz(std::vector<std::vector<double>> matriz) {
    //Calculo la transpuesta
    //Si matriz es matriz.size * matriz[i].size (para cualquier i) -> matrizT es matriz[i].size * matriz.size
    std::vector<std::vector<double>> matrizT(matriz[0].size(), std::vector<double>(matriz.size()));

    for(int fila = 0; fila < matriz[0].size(); fila++){
        //
        for(int columna = 0; columna < matriz.size(); columna++){
            matrizT[columna][fila] = matriz[fila][columna];
        }
    }

    return matrizT;
}

//Hace la multiplicacion A*x donde A es matriz y x vector, el resultado es normalizado
std::vector<double> PCA::multiplicarMatrizVector(std::vector<std::vector<double>> &matriz, std::vector<double> &vec) const{
    if(matriz[0].size() != vec.size()){
        throw std::runtime_error("EL TAMAÑO DE LA MATRIZ Y EL VECTOR NO COINCIDEN");
    }

    //Se puede multiplicar
    std::vector<double> resultado(vec.size(), 0);
    double suma = 0; //con esto calculo la norma cuadrada de resultado
    for(int fila = 0; fila < matriz.size(); fila++){
        for(int k = 0; k < vec.size(); k++){
            resultado[k] += matriz[fila][k] * vec[k];
            suma += pow(matriz[fila][k] * vec[k], 2); //No hace falta el modulo
        }
    }

    suma = sqrt(suma); //Ahora esto es la norma

    //Normalizo resultado
    for(int i = 0; i < resultado.size() ; i++){
        resultado[i] = resultado[i] / suma;
    }

    return resultado;
}

//Calcula estimado del autovalor con el Rayleigh Quotient
//si autovector es autovector de matrizM entonces esta funcion devuelve el autovalor exacto
double PCA::calcularAutovalor(std::vector<std::vector<double>> matrizM, std::vector<double> autovector) {
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
    for(int i = 0; i < autovector.size(); i++){
        Avtv += mulAv[i] * autovector[i];
    }

    // ((matrizM * autovector)^t * autovector) / (autovector^t * autvector)
    return (Avtv / vtv);
}

std::vector<double> PCA::transformacionCaracteristica(std::vector<std::vector<double>> autovectores,
                                                      std::vector<double> imagenVectorizada, int alfa) {
    //
}
