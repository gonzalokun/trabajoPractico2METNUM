#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstring>
#include "cargadorDeImagenes.h"
#include "knn.hpp"
#include "moduloPCA.h"

void operacionkNN(std::string trainset, std::string testSet, std::string classif, int valorK);
void operacionkNNPCA(std::string trainset, std::string testSet, std::string classif, int valorAlfa, double valorTolerancia, int valorK, int limiteDeCiclos);
void pruebaPCA(std::string trainset, std::string testSet, std::string classif, int valorAlfa, double valorTolerancia, int limiteDeCiclos);
void pruebaKFold(std::string trainset, std::string classif, int KDeFold, int valorK);

int main(int cantidadDeArgumentos, char** argumentos) {

    //argumentos: ./tp2 -m <METHOD> -i <TRAINSET> -q <TESTSET> -o <CLASSIF>
    //Hacer un ciclodespues

    //std::cout << argumentos[4] << std::endl;

    //********************************************//

    std::cout << "ARGUMENTOS: " << " ";

    for(int i=0;i<cantidadDeArgumentos;i++){
        std::cout << argumentos[i] << " ";
    }

    std::cout << std::endl;

    //std::cout << "CANTIDAD DE ARGUMENTOS: " << cantidadDeArgumentos <<std::endl;

    std::string metodo;
    std::string trainSet;
    std::string testSet;
    std::string classif;

    if(cantidadDeArgumentos < 9){
        return 1;
    }

    //Procesamos los argumentos del programa
    //Los argumentos van a estar siempre en las posiciones impares
    //ya que el primer arg es el nombre del archivo
    for(int i = 1; i < cantidadDeArgumentos; i = i+2){

        //std::cout << "VALOR DE i: " << i << std::endl;
        //std::cout << "ARGUMENTO[i]: " << argumentos[i] << std::endl;


        if((i + 1 < cantidadDeArgumentos)){
            //std::cout << "ARGUMENTO[i + 1]: " << argumentos[i + 1] << std::endl;

            if(strcmp(argumentos[i], "-m") == 0){
                metodo = argumentos[i+1];
            }
            else if(strcmp(argumentos[i], "-i") == 0){
                trainSet = argumentos[i+1];
            }
            else if(strcmp(argumentos[i], "-q") == 0){
                testSet = argumentos[i+1];
            }
            else if(strcmp(argumentos[i], "-o") == 0){
                classif = argumentos[i+1];
            }
        }
    }

    //Muestro los strings para comprobar
    //std::cout << "METODO: " << metodo << std::endl;
    //std::cout << "TRAINSET: " << trainSet << std::endl;
    //std::cout << "TESTSET: " << testSet << std::endl;
    //std::cout << "CLASSIF: " << classif << std::endl;

    int valorK = 7;
    int valorAlfa = 15;
    double tolerancia = 0.01;
    int valorKFold = 4;
    int limiteDeCiclos = 100;

    //Ahora opero según lo ingresado
    if(metodo == "0"){
        //Solo se hace kNN
        operacionkNN(trainSet, testSet, classif, valorK);
    }else if(metodo == "1"){
        //Se hace kNN + PCA
        operacionkNNPCA(trainSet, testSet, classif, valorAlfa, tolerancia, valorK, limiteDeCiclos);
    }else if(metodo == "2"){
        //Se hace kNN + PCA
        pruebaPCA(trainSet, testSet, classif, valorAlfa, tolerancia, limiteDeCiclos);

        //Para probar al reves
        //pruebaPCA(testSet, trainSet, classif, valorAlfa, tolerancia, limiteDeCiclos);
    }else if(metodo == "3"){
        //Se hace kNN + PCA
        pruebaKFold(trainSet, classif, valorKFold, valorK);
    }else{
        //No se hace nada por ahora
        std::cout << "Los metodos válidos son:" << std::endl;
        std::cout << "\t0 - Identificar imagenes usando KNN" << std::endl;
        std::cout << "\t1 - Identificar imagenes usando KNN y PCA" << std::endl;
        std::cout << "\t2 - Test para verificar si los autovalores encontrados son correctos" << std::endl;
        std::cout << "\t3 - Test de KFold sobre el conjunto de entrenamiento" << std::endl;
    }

    return 0;
}

void operacionkNN(std::string trainset, std::string testSet, std::string classif, int valorK){
    //std::cout << "ENTRE EN operacionkNN!" << std::endl;

    ofstream archivoOUT(classif, std::ofstream::out);
    std::stringstream conversor;
    conversor.str("");
    conversor.clear();

    cargadorDeImagenes baseDeEntrenamiento(trainset.c_str());
    cargadorDeImagenes baseDePrueba(testSet.c_str());

    std::vector<std::vector<double>> vecImagenesBase = baseDeEntrenamiento.vectoresDeImagenes();
    std::vector<int> clases = baseDeEntrenamiento.clases();

    //std::cout << "TAMAÑO DE LA BASE DE ENTRENAMIENTO: " << baseDeEntrenamiento.conjuntoDeImagenes().size() << std::endl;
    //std::cout << "TAMAÑO DE LA BASE A PROBAR: " << baseDePrueba.conjuntoDeImagenes().size() << std::endl;

    int imagenesAcertadas = 0;

    //Vector que tiene las ID reales de las pruebas
    //IDrealesDeBaseDePrueba[i] es la ID del vector guardado en baseDePrueba.vectoresDeImagenes()[i]
    std::vector<int> IDrealesDeBaseDePrueba = baseDePrueba.clases();

    //Realizo el reconocimiento mediante kNN
    //Para cada imagen a reconocer su ID
    for(int imagen = 0; imagen < baseDePrueba.conjuntoDeImagenes().size(); imagen++){

        //Le averiguo el knn a la imagen
        int IDpropuesta = knn(vecImagenesBase, clases, baseDePrueba.vectoresDeImagenes()[imagen], valorK);

        //Aca puedo chequear el accuracy antes de seguir!

        if(IDpropuesta == IDrealesDeBaseDePrueba[imagen]){
            //La imagen se encontro bien!
            imagenesAcertadas++;
        }

        //Ahora tengo que escribir en el archivo

        std::string lineaSalida;

        conversor << baseDePrueba.rutas()[imagen] + ", ";
        conversor << IDpropuesta;
        conversor << ",\n";

        lineaSalida = conversor.str();

        archivoOUT << lineaSalida;

        //Limpio el conversor
        conversor.str("");
        conversor.clear();
    }

    archivoOUT.close();

    //Ahora muestro lo demas
    double accuracy = (double) (imagenesAcertadas/baseDePrueba.conjuntoDeImagenes().size());

    std::cout << "-------------------------RESULTADOS-------------------------" << std::endl;
    std::cout << "Metodo elegido: 0 - Identificar imagenes usando KNN" << std::endl;
    std::cout << "Archivo usado para el trainset: " << trainset << std::endl;
    std::cout << "Archivo usado para el testset: " << testSet << std::endl;
    std::cout << "Nombre del archivo de salida: " << classif << std::endl;
    std::cout << "Valor de k utilizado en kNN: " << valorK << std::endl;
    std::cout << "ACCURACY: " << (accuracy * 100) << "% de imagenes acertadas" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
}

void operacionkNNPCA(std::string trainset, std::string testSet, std::string classif, int valorAlfa,
                     double valorTolerancia, int valorK, int limiteDeCiclos){
    //std::cout << "ENTRE EN operacionkNNPCA!" << std::endl;

    int alfa = valorAlfa;
    double tolerancia = valorTolerancia;

    ofstream archivoOUT(classif, std::ofstream::out);
    std::stringstream conversor;
    conversor.str("");
    conversor.clear();

    cargadorDeImagenes baseDeEntrenamiento(trainset.c_str());
    cargadorDeImagenes baseDePrueba(testSet.c_str());

    std::vector<std::vector<double>> vecImagenesBase = baseDeEntrenamiento.vectoresDeImagenes();
    std::vector<int> clases = baseDeEntrenamiento.clases();

    //std::cout << "TAMAÑO DE LA BASE DE ENTRENAMIENTO: " << baseDeEntrenamiento.conjuntoDeImagenes().size() << std::endl;
    //std::cout << "TAMAÑO DE LA BASE A PROBAR: " << baseDePrueba.conjuntoDeImagenes().size() << std::endl;

    //Preproceso con PCA las imagenes de la base
    PCA modPCA;

    std::vector<double> vectorMedia = modPCA.obtenerMedia(baseDeEntrenamiento.conjuntoDeImagenes());
    std::vector<std::vector<double>> matrizM = modPCA.obtenerMatrizM(baseDeEntrenamiento.conjuntoDeImagenes(), vectorMedia);
    std::vector<std::pair<std::vector<double>, double >> autoVecYAutoVal = modPCA.calcularAutovalYAutoVec(matrizM, alfa, tolerancia, limiteDeCiclos);

    std::vector<std::vector<double>> autovectoresBasePrueba(autoVecYAutoVal.size());
    for(int j = 0; j < autoVecYAutoVal.size(); j++){
        autovectoresBasePrueba[j] = autoVecYAutoVal[j].first;
    }

    //Aplico la transformacion a los vectores
    for(int i = 0; i < vecImagenesBase.size(); i++){
        vecImagenesBase[i] = modPCA.transformacionCaracteristica(autovectoresBasePrueba, vecImagenesBase[i]);
    }
    //

    //Preproceso con PCA las imagenes de test
    std::vector<double> vectorMediaTest = modPCA.obtenerMedia(baseDePrueba.conjuntoDeImagenes());

    std::vector<std::vector<double>> matrizMTest = modPCA.obtenerMatrizM(baseDePrueba.conjuntoDeImagenes(), vectorMediaTest);

    std::vector<std::pair<std::vector<double>, double >> autoVecYAutoValTest = modPCA.calcularAutovalYAutoVec(matrizMTest, alfa, tolerancia, limiteDeCiclos);

    std::cout << "HAYYYYYYYYY" << std::endl;

    std::vector<std::vector<double>> autovectoresBaseTest(autoVecYAutoVal.size());

    for(int j = 0; j < autoVecYAutoValTest.size(); j++){
        autovectoresBaseTest[j] = autoVecYAutoValTest[j].first;
    }

    std::vector<std::vector<double>> vecImagenesTest(baseDePrueba.conjuntoDeImagenes().size());

    //Aplico la transformacion a los vectores
    for(int i = 0; i < vecImagenesTest.size(); i++){
        vecImagenesTest[i] = modPCA.transformacionCaracteristica(autovectoresBaseTest, baseDePrueba.vectoresDeImagenes()[i]);
    }
    //

    int imagenesAcertadas = 0;
    std::vector<int> IDrealesDeBaseDePrueba = baseDePrueba.clases();

    //Realizo el reconocimiento mediante kNN
    //Para cada imagen a reconocer su ID
    for(int imagen = 0; imagen < baseDePrueba.conjuntoDeImagenes().size(); imagen++){

        //Le averiguo el knn a la imagen
        int IDpropuesta = knn(vecImagenesBase, clases, vecImagenesTest[imagen], valorK);

        if(IDpropuesta == IDrealesDeBaseDePrueba[imagen]){
            //La imagen se encontro bien!
            imagenesAcertadas++;
        }

        //Ahora tengo que escribir en el archivo

        std::string lineaSalida;

        conversor << baseDePrueba.rutas()[imagen] + ", ";
        conversor << IDpropuesta;
        conversor << ",\n";

        lineaSalida = conversor.str();

        archivoOUT << lineaSalida;

        //Limpio el conversor
        conversor.str("");
        conversor.clear();
    }

    archivoOUT.close();

    //Ahora muestro lo demas
    double accuracy = (double) (imagenesAcertadas/baseDePrueba.conjuntoDeImagenes().size());

    std::cout << "-------------------------RESULTADOS-------------------------" << std::endl;
    std::cout << "Metodo elegido: 1 - Identificar imagenes usando KNN y PCA" << std::endl;
    std::cout << "Archivo usado para el trainset: " << trainset << std::endl;
    std::cout << "Archivo usado para el testset: " << testSet << std::endl;
    std::cout << "Nombre del archivo de salida: " << classif << std::endl;
    std::cout << "Valor de k utilizado en kNN: " << valorK << std::endl;
    std::cout << "Valor de alfa utilizado en PCA: " << alfa << std::endl;
    std::cout << "Tolerancia usada para calcular autovalores y autovectores en PCA: " << tolerancia << std::endl;
    std::cout << "ACCURACY: " << (accuracy * 100) << "% de imagenes acertadas" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
}

void pruebaPCA(std::string trainset, std::string testSet, std::string classif, int valorAlfa, double valorTolerancia, int limiteDeCiclos){

    //Acá le aplicas PCA a todo
    PCA modPCA;

    cargadorDeImagenes baseDeEntrenamiento(trainset.c_str());

    std::vector<double> mediaBase = modPCA.obtenerMedia(baseDeEntrenamiento.conjuntoDeImagenes());

    std::vector<std::vector<double>> matrizM = modPCA.obtenerMatrizM(baseDeEntrenamiento.conjuntoDeImagenes(), mediaBase);

    int alfa = valorAlfa;

    std::vector<std::pair<std::vector<double>, double >> autovecYAutoval = modPCA.calcularAutovalYAutoVec(matrizM, alfa, valorTolerancia, limiteDeCiclos);

    std::vector<std::vector<double>> autovectores(autovecYAutoval.size());

    for(int i = 0; i < autovecYAutoval.size(); i++){
        autovectores[i] = autovecYAutoval[i].first;
    }

    std::vector<double> autovalores(autovecYAutoval.size());

    for(int i = 0; i < autovecYAutoval.size(); i++){
        autovalores[i] = autovecYAutoval[i].second;
    }

    //Ordeno los autoval acá
    std::sort(autovalores.rbegin(), autovalores.rend());
    //

    //muestro las raices
    for(int i = 0; i < autovecYAutoval.size(); i++){
        std::cout << "AUTOVALOR " << i << ": " << sqrt(autovalores[i]) << std::endl;
    }
}

void pruebaKFold(std::string trainset, std::string classif, int KDeFold, int valorK){
    //
}