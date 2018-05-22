#include <math.h>
#include <random>
#include <stdexcept>
#include <iostream>
#include "moduloPCA.h"

//PCA::PCA(std::vector<std::pair<std::vector<double>, int>> conjuntoDeImagenes) {
//    //
//}

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

    //std::cout << "Antes de crear la matriz X" << std::endl;
    //Lleno la matriz X con los datos que corresponden
    for(int fila = 0; fila < cantidadDeVectores; fila++){
        std::vector<double> filaActual(cantidadDeColumnas, 0);

        for(int columna = 0; columna < cantidadDeColumnas; columna++){
            filaActual[columna] = (conjuntoDeImagenes[fila].first[columna] - vectorMedia[columna]) / sqrt((double) cantidadDeVectores - 1);
        }

        matrizX.push_back(filaActual);
    }

    //std::cout << "Despues de crear la matriz X" << std::endl;

    //Deberia estar formada la matrizX

    //std::cout << "Antes de crear la matriz X transpuesta" << std::endl;
    //Ahora calculo la transpuesta
    //Si matrizX es cantidadDeVectores * cantidadDeColumnas -> matrizXt es cantidadDeColumnas * cantidadDeVectores
    //std::vector<std::vector<double>> matrizXt(cantidadDeColumnas, std::vector<double>(cantidadDeVectores));
    std::vector<std::vector<double>> matrizXt(transponerMatriz(matrizX));

    //std::cout << "Despues de crear la matriz X transpuesta" << std::endl;
//    for(int fila = 0; fila < cantidadDeVectores; fila++){
//        //
//        for(int columna = 0; columna < cantidadDeColumnas; columna++){
//            matrizXt[columna][fila] = matrizX[fila][columna];
//        }
//    }

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

/*
std::vector<std::pair<std::vector<double>, double >> PCA::calcularAutovalYAutoVec(
        std::vector<std::vector<double>> matrizM) {
    //Acá guardo los autovec y los autoval, va a haber n autovec (y autoval)
    unsigned int tamMatriz = matrizM.size();
    //std::vector<std::pair<std::vector<double>, double >> paresAutoVecYAutoVal(tamMatriz, std::make_pair(std::vector<double>(tamMatriz), 0));
    std::vector<std::pair<std::vector<double>, double >> paresAutoVecYAutoVal;
    double tolerancia = 0.01;

    std::cout << "TAMAÑO MATRIZ: " << tamMatriz << std::endl;

    //Para cada autovalor
    for(int i = 0; i < tamMatriz; i++){
        //Por cada iteracion de este ciclo tengo que hallar un par <autovector, autovalor>
        std::cout << "CICLO i: " << i << std::endl;

        //Hago un vector al azar normalizado (POR AHORA AGARRO EL DE SOLO UNOS)
        std::vector<double> vectorX(tamMatriz, (1.0));

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

            std::cout << "AUTOVALX: " << autovalX  << std::endl;
            std::cout << "AUTOVALITER: " << autovalIter  << std::endl;

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

    //Se calculó todo (Si dios quiere)
    return paresAutoVecYAutoVal;
}
*/

std::vector<std::pair<std::vector<double>, double >> PCA::calcularAutovalYAutoVec(
        const std::vector<std::vector<double>> &matrizM, int alfa, double tolerancia){
    //Solo calculo los necesarios
    std::vector<std::pair<std::vector<double>, double >> autoVecYAutoval(alfa);

    //Copio la matriz pasada para poder aplicar el metodo de las potencias
    std::vector<std::vector<double>> matrizAux(matrizM);

    //Para cada autovalor
    for(int i = 0; i < alfa; i++){

        //Guardo la cantidad de filas
        int filas = matrizM.size();

        //Creo un vector al azar
        std::vector<double> vectorX = generarVectorAlAzar(filas);

        //Creo un vector para poder comparar las distintas iteraciones
        std::vector<double> vectorAnterior = vectorX;

        //La distancia inicial de los dos vectores
        double distanciaVectores = tolerancia;

        //Cuando se acerquen los vectores se habra llegado al autovector
        while(distanciaVectores >= tolerancia){
            vectorAnterior = vectorX;

            vectorX = multiplicarMatrizVector(matrizM, vectorX);

            double normaVector = 1 / (calcularNorma(vectorX));

            for(int j = 0; j < vectorX.size() ; j++){
                vectorX[j] = vectorX[j] * normaVector;
            }

            std::vector<double> resta(vectorX.size());

            for(int j = 0; j < vectorX.size(); j++){
                resta[j] = vectorX[j] - vectorAnterior[j];
            }

            distanciaVectores = calcularNorma(resta);
        }

        //Tengo el autovector, ahora calculo el autovalor
        double autovalor = calcularAutovalor(matrizM, vectorX);

        //Los guardo
        //autoVecYAutoval.emplace_back(vectorX, autovalor);
        autoVecYAutoval[i].first = vectorX;
        autoVecYAutoval[i].second = autovalor;

        //Uso deflacion para poder buscar los proximos autovec y autoval
        for(int fila = 0; fila < vectorX.size(); fila++){
            for(int columna = 0; columna < vectorX.size() ; columna++){
                matrizAux[fila][columna] -= autovalor * vectorX[fila] * vectorX[columna];
            }
        }

        //Deveria funcionar
    }

    return autoVecYAutoval;
}

std::vector<double> PCA::generarVectorAlAzar(int &dimension) {
    std::vector<double> vecAlAzar(dimension, 0);

    //De 0 a 255
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

    std::cout << "Cantidad de filas de matriz ingresada: " << matriz.size()  << std::endl;
    std::cout << "Cantidad de columnas de matriz ingresada: " << matriz[0].size()  << std::endl;

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

    //std::cout << "ANTES DE MULTIPLICAR LA MATRIZ"  << std::endl;

    //Se puede multiplicar
    std::vector<double> resultado(vec.size(), 0);
    double suma = 0; //con esto calculo la norma cuadrada de resultado

    //
//    for(int fila = 0; fila < matriz.size(); fila++){
//        for(int k = 0; k < vec.size(); k++){
//            resultado[k] += matriz[fila][k] * vec[k];
//            suma += pow(matriz[fila][k] * vec[k], 2); //No hace falta el modulo
//        }
//    }

    for(int elem = 0; elem < resultado.size(); elem++){

        double sum = 0;

        for(int k = 0; k < resultado.size(); k++){
            sum += matriz[elem][k] * vec[k];
        }

        //std::cout << "El sum es: " << sum  << std::endl;

        suma += pow(sum, 2);

        //std::cout << "La suma de los sum al cuadrado es: " << suma << std::endl;

        resultado[elem] = sum;
    }

    //std::cout << "DESPUES DE MULTIPLICAR LA MATRIZ"  << std::endl;

    suma = sqrt(suma); //Ahora esto es la norma

    //suma = 1;

//    for(int i = 0; i < resultado.size() ; i++){
//        resultado[i] = resultado[i] / suma;
//    }

    //Normalizo resultado
//    for(int i = 0; i < resultado.size() ; i++){
//        resultado[i] = resultado[i] / suma;
//    }

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
    for(int i = 0; i < autovector.size(); i++){
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
