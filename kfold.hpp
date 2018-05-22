#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;


template<class In>
class Kfold {
public:
    Kfold(int k, In _beg, In _end);
    template<class Out>
    void getFold(int foldNo, Out training, Out testing);

private:
    In beg;
    In end;
    int folds;
    vector<int> whichFoldToGo;

};

template<class In>
Kfold<In>::Kfold(int _folds, In _beg, In _end) :
        beg(_beg), end(_end), folds(_folds) {
    if (folds <= 0)
    {
        cout<<"No se pueden crear "<< folds<<" folds";
        throw;
    }

    int foldNo = 0;
    for (In i = beg; i != end; i++) {
        whichFoldToGo.push_back(++foldNo);
        if (foldNo == folds)
            foldNo = 0;
    }
    if (foldNo)
    {
         cout<<"Con este numero de folds (folds="<< folds <<") no se puede dividir en parter iguales"<<endl;
         throw;
    }
    random_shuffle(whichFoldToGo.begin(), whichFoldToGo.end());
}

template<class In>
template<class Out>
void Kfold<In>::getFold(int foldNo, Out training, Out testing) {

    int k = 0;
    In i = beg;
    while (i != end) {
        if (whichFoldToGo[k++] == foldNo) {
            *testing++ = *i++;
        } else
            *training++ = *i++;
    }
}

