//
// Created by Gonzalo on 17/05/2018.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "cargadorDeImagenes.h"

//REVISAR ESTAS FUNCIONES
//Copiadas del ppmloader
unsigned int obtenerMediaDePixel(uchar* datos, int i, int j, int alto, int ancho){
    if(i > alto)
        throw std::runtime_error("El direccionamiento vertical no puede ser mayor a la altura.");
    if(j > ancho)
        throw std::runtime_error("El direccionamiento horizontal no puede ser mayor al ancho.");
    unsigned int red = (unsigned int)(datos[i*ancho*3 + j*3 + 0]);
    unsigned int green = (unsigned int)(datos[i*ancho*3 + j*3 + 1]);
    unsigned int blue = (unsigned int)(datos[i*ancho*3 + j*3 + 2]);
    return (unsigned int)((red+green+blue) / 3);
}

void leerImagen(std::string filename, uchar** datos, int* ancho, int* alto){
    *datos = NULL;
    *ancho = 0;
    *alto = 0;
    PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;

    bool ret = LoadPPMFile(datos, ancho, alto, &pt, filename.c_str());
    if (!ret || ancho == 0|| alto == 0|| pt!=PPM_LOADER_PIXEL_TYPE_RGB_8B){
        throw std::runtime_error("Fallo al leer la imagen.");
    }
}

cargadorDeImagenes::cargadorDeImagenes(const char *archivo) {
    std::ifstream listaDeNombres(archivo);

    if(listaDeNombres.eof()){
        std::cout << "EL ARCHIVO ESTA VACIO O NO EXISTE" << std::endl;
        return;
    }

    std::string nombreArchivoDeImagen;
    int IDPersona;

    //HAY QUE VER SI ESTO FUNCA

    //std::cout << "CONSTRUCTOR ANTES DE CICLO" << std::endl;

    std::stringstream conversor;

    std::string delimiter = ",";

    for(std::string linea; std::getline(listaDeNombres, linea); ){
        std::cout << linea << std::endl;

        //Vacio el conversor
        conversor.clear();
        conversor.str("");

        //Separamos la linea y copio la direccion del archivo
        conversor << linea.substr(0, linea.find(delimiter));
        conversor >> nombreArchivoDeImagen;

        //Borro el nombre del archivo de la line actual
        linea.erase(0, linea.find(delimiter) + delimiter.length());

        //Vacio el conversor
        conversor.clear();
        conversor.str("");

        //Pongo la ID de la persona en el conversor
        conversor << linea.substr(0, linea.find(delimiter));

        //Convierto el ID a INT
        conversor >> IDPersona;

        //std::cout << "ARCHIVO: " << nombreArchivoDeImagen << std::endl;
        //std::cout << "ID DE PERSONA: " << IDPersona << std::endl;

        //Cargo la Imagen
        cargarImagen(nombreArchivoDeImagen.c_str(), IDPersona);
    }

    listaDeNombres.close();
}

void cargadorDeImagenes::cargarImagen(std::string rutaArchivo, int IDPersona) {
    std::cout << "CARGANDO IMAGEN: " << rutaArchivo << "; ID: " << IDPersona  << std::endl;

    //Uso el ppmloader para cargar el archivo dado
    uchar *datos;
    int ancho = 0, alto = 0;

    rutaArchivo = "../" + rutaArchivo;

    leerImagen(rutaArchivo, &datos, &ancho, &alto);

    std::vector<int> vectorPixeles;

    //Obtengo la info de cada pixel
    for(int h = 0; h < alto; ++h){
        for(int w = 0; w < ancho; ++w){
            vectorPixeles.push_back(obtenerMediaDePixel(datos, h, w, alto, ancho));
        }
    }

    std::cout << "MUESTRO EL VECTOR PARA LA PRUEBA CHICA: " << std::endl;
    std::cout << "[";

    for(int i = 0; i < vectorPixeles.size(); i++){
        std::cout << i << ", ";
    }

    std::cout << "]" << std::endl;

    //Ahora el vector esta cargado con los pixeles de la imagen (en escala de grises)
    //entonces lo pongo en el vector de imagenes
    _imagenes.push_back(std::make_pair(vectorPixeles, IDPersona));

    //Borro lo contenido en datos
    delete [] datos;

}

std::vector< std::pair<std::vector<int>, int> > cargadorDeImagenes::conjuntoDeImagenes() {
    return _imagenes;
}


