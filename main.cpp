#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstring>
#include "cargadorDeImagenes.h"
#include "knn.hpp"
#include "moduloPCA.h"

void operacionkNN(std::string trainset, std::string testSet, std::string classif);
void operacionkNNPCA(std::string trainset, std::string testSet, std::string classif);

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
    std::cout << "METODO: " << metodo << std::endl;
    std::cout << "TRAINSET: " << trainSet << std::endl;
    std::cout << "TESTSET: " << testSet << std::endl;
    std::cout << "CLASSIF: " << classif << std::endl;

    //Ahora opero según lo ingresado
    if(metodo == "0"){
        //Solo se hace kNN
        operacionkNN(trainSet, testSet, classif);
    }else if(metodo == "1"){
        //Se hace kNN + PCA
        operacionkNNPCA(trainSet, testSet, classif);
    } else{
        //No se hace nada por ahora
        std::cout << "NO SE HIZO NADA!!!!!" << std::endl;
    }

    //**********PARTE DONDE PRUEBO COSAS**********//

    //Tengo que interpretar el input

    //Probamos el cargador
//    cargadorDeImagenes c(argumentos[4]);
//    cargadorDeImagenes testeo("imagenTesteo.in");

    //Creo la variable que maneje el PCA
//    PCA modPCA;
//
//    std::vector<double> mediaBase = modPCA.obtenerMedia(c.conjuntoDeImagenes());
//
//    std::vector<std::vector<double>> matrizM = modPCA.obtenerMatrizM(c.conjuntoDeImagenes(), mediaBase);
//
//    int alfa = 15;
//
//    std::vector<std::pair<std::vector<double>, double >> autovecYAutoval = modPCA.calcularAutovalYAutoVec(matrizM, alfa, 0.01);
//
//    std::vector<std::vector<double>> autovectores(autovecYAutoval.size());
//
//    for(int i = 0; i < autovecYAutoval.size(); i++){
//        autovectores[i] = autovecYAutoval[i].first;
//    }

//    std::vector<double> autovalores(autovecYAutoval.size());
//
//    for(int i = 0; i < autovecYAutoval.size(); i++){
//        autovalores[i] = autovecYAutoval[i].second;
//    }
//
//    //Ordeno los autoval acá
//    std::sort(autovalores.rbegin(), autovalores.rend());
//    //
//
//    //muestro las raices
//    for(int i = 0; i < autovecYAutoval.size(); i++){
//        std::cout << "AUTOVALOR " << i << ": " << sqrt(autovalores[i]) << std::endl;
//    }

    //std::vector<double> imagenSinTC = testeo.conjuntoDeImagenes()[0].first;

    //std::vector<double> imagenTC = modPCA.transformacionCaracteristica(autovectores, imagenSinTC);

    //std::vector<std::vector<double>> baseConTC(c.conjuntoDeImagenes().size());

//    for(int i = 0; i < baseConTC.size(); i++){
//        baseConTC[i] = modPCA.transformacionCaracteristica(autovectores, c.conjuntoDeImagenes()[i].first);
//    }

    std::cout << "FIN DE PROGRAMA" << std::endl;

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

void operacionkNN(std::string trainset, std::string testSet, std::string classif){
    std::cout << "ENTRE EN operacionkNN!" << std::endl;

    ofstream archivoOUT(classif, std::ofstream::out);
    std::stringstream conversor;
    conversor.str("");
    conversor.clear();

    cargadorDeImagenes baseDeEntrenamiento(trainset.c_str());
    cargadorDeImagenes baseDePrueba(testSet.c_str());

    std::vector<std::vector<double>> vecImagenesBase = baseDeEntrenamiento.vectoresDeImagenes();
    std::vector<int> clases = baseDeEntrenamiento.clases();

    //std::cout << "ANTES DE ALGUN KNN" << std::endl;

    std::cout << "TAMAÑO DE LA BASE DE ENTRENAMIENTO: " << baseDeEntrenamiento.conjuntoDeImagenes().size() << std::endl;
    std::cout << "TAMAÑO DE LA BASE A PROBAR: " << baseDePrueba.conjuntoDeImagenes().size() << std::endl;

    //Realizo el reconocimiento mediante kNN
    //Para cada imagen a reconocer su ID
    for(int imagen = 0; imagen < baseDePrueba.conjuntoDeImagenes().size(); imagen++){

        //Le averiguo el knn a la imagen
        int IDpropuesta = knn(vecImagenesBase, clases, baseDePrueba.vectoresDeImagenes()[imagen], 7);

        //std::cout << "SOBREVIVI EL KNN DE LA ITERACION: " << imagen << std::endl;

        //Aca puedo chequear el accuracy antes de seguir!

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
}

void operacionkNNPCA(std::string trainset, std::string testSet, std::string classif){
    std::cout << "ENTRE EN operacionkNNPCA!" << std::endl;

    //Es igual a la anterior pero le aplico PCA a los vectores antes de usarla
    cargadorDeImagenes baseDeEntrenamiento(trainset.c_str());
    cargadorDeImagenes baseDePrueba(testSet.c_str());

    //Acá le aplicas PCA a todo

    //Acá haces lo de la anterior
}