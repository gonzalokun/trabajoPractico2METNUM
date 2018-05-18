//
// Created by Gonzalo on 16/05/2018.
//

#ifndef TRABAJOPRACTICO2METNUM_MODULOPCA_H
#define TRABAJOPRACTICO2METNUM_MODULOPCA_H

#include <vector>

class moduloPCA{
    public:
        moduloPCA(std::vector<std::pair<std::vector<unsigned char>, int>> conjuntoDeImagenes);
        //

    private:
        std::vector<std::pair<std::vector<unsigned char>, int>> _conjutoImagenes;
        std::vector< std::pair<std::vector<unsigned char>, double>> autovalores(); //PASARLE LA MATRIZ M
};

#endif //TRABAJOPRACTICO2METNUM_MODULOPCA_H
