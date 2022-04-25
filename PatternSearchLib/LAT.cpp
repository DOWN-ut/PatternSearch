#include "DIPWM.h"
#include "LAT.h"
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

namespace PatternSearch
{
    //Accessors


    //Processors
    void LAT::Setup(double* arr, int nCol, int nRow){
        double sumMax = 0;
        for (int i=nCol-1;i>=0;i--){ // on parcours le tableau de droite à gauche (voir def LAT)
            double m = arr[i]; // on initialise le max avec la première case de notre colonne
            for (int j=0;j<nRow;j++){ // on parcourt la colonne pour obtenir son max
            m = max(m,arr[i + (j * nCol)]);
            }
            sumMax += m;
            Tab[i] = sumMax;
        }
    }

    void LAT::DisplayTable(){
        for (int i=0; i<nCol; i++){
            cout << Tab[i] << "  |  ";
        }
        cout << "\n";
    }

    //Constructors
    LAT::LAT(double* arr, int nCol, int nRow){
        this->Tab = new double[nCol];
        Setup(arr, nCol, nRow);
        this->nCol = nCol;
    }
}