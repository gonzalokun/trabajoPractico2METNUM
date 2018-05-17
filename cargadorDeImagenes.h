
#ifndef TRABAJOPRACTICO2METNUM_CARGADORDEIMAGENES_H
#define TRABAJOPRACTICO2METNUM_CARGADORDEIMAGENES_H

#include <vector>

class cargadorDeImagenes{
    public:
        //Toma el archivo con la lista de las imagenes de la base
        cargadorDeImagenes(const char *archivo);

        //Devuelve el conjunto de las imagenes de la base con el siguiente formato:
        //  <(vector que representa la imagen),(ID de la persona)>
        std::vector< std::pair<std::vector<int>, int> > conjuntoDeImagenes();

    private:
        //Carga la imagen pasada en el vector
        void cargarImagen(const char *archivo, int IDPersona);

        //Vector con todas las imagenes
        std::vector< std::pair<std::vector<int>, int> > _imagenes;
};

#endif //TRABAJOPRACTICO2METNUM_CARGADORDEIMAGENES_H
