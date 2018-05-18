
#ifndef TRABAJOPRACTICO2METNUM_CARGADORDEIMAGENES_H
#define TRABAJOPRACTICO2METNUM_CARGADORDEIMAGENES_H

#include <vector>
#include <stdio.h>
#include "lib/ppmloader.h"

typedef unsigned char uchar;

////REVISAR ESTAS FUNCIONES
////Copiadas del ppmloader
//unsigned int obtenerMediaDePixel(uchar* datos, int i, int j, int alto, int ancho){
//    if(i > alto)
//        throw std::runtime_error("El direccionamiento vertical no puede ser mayor a la altura.");
//    if(j > ancho)
//        throw std::runtime_error("El direccionamiento horizontal no puede ser mayor al ancho.");
//    unsigned int red = (unsigned int)(datos[i*ancho*3 + j*3 + 0]);
//    unsigned int green = (unsigned int)(datos[i*ancho*3 + j*3 + 1]);
//    unsigned int blue = (unsigned int)(datos[i*ancho*3 + j*3 + 2]);
//    return (unsigned int)((red+green+blue) / 3);
//}
//
//void leerImagen(std::string filename, uchar** datos, int* ancho, int* alto){
//    *datos = NULL;
//    *ancho = 0;
//    *alto = 0;
//    PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
//
//    bool ret = LoadPPMFile(datos, ancho, alto, &pt, filename.c_str());
//    if (!ret || ancho == 0|| alto == 0|| pt!=PPM_LOADER_PIXEL_TYPE_RGB_8B){
//        throw std::runtime_error("Fallo al leer la imagen.");
//    }
//}

//REVISAR ESTAS FUNCIONES
//Copiadas del ppmloader
unsigned int obtenerMediaDePixel(uchar* datos, int i, int j, int alto, int ancho);

void leerImagen(std::string filename, uchar** datos, int* ancho, int* alto);

class cargadorDeImagenes{
    public:
        //Toma el archivo con la lista de las imagenes de la base
        cargadorDeImagenes(const char *archivo);

        //Devuelve el conjunto de las imagenes de la base con el siguiente formato:
        //  <(vector que representa la imagen),(ID de la persona)>
        std::vector< std::pair<std::vector<int>, int> > conjuntoDeImagenes();

    private:
        //Carga la imagen pasada en el vector
        void cargarImagen(std::string rutaArchivo, int IDPersona);

        //Vector con todas las imagenes
        std::vector< std::pair<std::vector<int>, int> > _imagenes;
};

#endif //TRABAJOPRACTICO2METNUM_CARGADORDEIMAGENES_H
