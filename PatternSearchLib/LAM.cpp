#include "DIPWM.h"
#include "LAM.h"
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

namespace PatternSearch
{

  //Processors

    /*
  double LAM::score(int pos, int lettre, double* arr, int nCol){
    int c1 = lettre * 4;
    int c2 = lettre * 4 + 1;
    int c3 = lettre * 4 + 2;
    int c4 = lettre * 4 + 3;
    if(pos == nCol -1){
        //return max({ arr[pos * nCol + c1], arr[pos * nCol + c2], arr[pos * nCol + c3], arr[pos * nCol + c4] });
        return max(max(max(arr[pos * nCol + c1], arr[pos * nCol + c2]), arr[pos * nCol + c3]),arr[pos * nCol + c4]);
    }
    else{
        return max(max(max(arr[pos * c1] + score(pos + 1, 0, arr, nCol),
            arr[pos * c2] + score(pos + 1, 1, arr, nCol)),
            arr[pos * c3] + score(pos + 1, 2, arr, nCol)),
            arr[pos * c4] + score(pos + 1, 3, arr, nCol));
        /*return max({
          (arr[pos * c1] + score(pos + 1, 0, arr, nCol)),
          (arr[pos * c2] + score(pos + 1, 1, arr, nCol)),
          (arr[pos * c3] + score(pos + 1, 2, arr, nCol)),
          (arr[pos * c4] + score(pos + 1, 3, arr, nCol)) });
    }
  }

  void LAM::Setup(double* arr, int nCol){
    for(int i = 0; i < nCol - 1; i++){
      for(int j = 0; j < nRow; j++){
        Tab[i*nCol + j] = score(j,i,arr,nCol);
      }
    }
  }*/

    void LAM::Set(double v, int col, int row) {
        Tab[col + (row * nCol)] = v;
    }

    double LAM::Get(int col, int row)
    {
        return Tab[col + (row * nCol)];
    }

    double max4(double a, double b, double c, double d)
    {
        return max(a, max(b, max(c, d)));
    }

    void LAM::Setup(double* arr, int nCol) {
        
        //On remplit la dernière colonne avec les maximums de la dernière colonne de la matrice
        for (int i = 0; i < nRow; i++)
        {
            double v = max4(
                arr[nCol - 1 + (((4 * i)) * nCol)],
                arr[nCol - 1 + (((4 * i) + 1) * nCol)],
                arr[nCol - 1 + (((4 * i) + 2) * nCol)],
                arr[nCol - 1 + (((4 * i) + 3) * nCol)]);

            Set(v, nCol - 1, i);
        }

        //Pour les colonnes suivantes :
        for (int c = nCol - 2; c >= 0; c--)
        {
            for (int i = 0; i < nRow; i++)
            {
                double v = max4(
                    arr[c + (((4 * i)) * nCol)] + Get(c+1,0),
                    arr[c + (((4 * i) + 1) * nCol)] + Get(c + 1, 1),
                    arr[c + (((4 * i) + 2) * nCol)] + Get(c + 1, 2),
                    arr[c + (((4 * i) + 3) * nCol)] + Get(c + 1, 3));

                Set(v, c, i);
            }
        }
    }

  void LAM::DisplayTable(){

      cout << nCol << " x " << nRow << endl;
    for(int j = 0; j < nRow; j++){
      cout << "\n";
      for (int i=0; i<nCol; i++){
          cout << Get(i,j) << "  |  ";
      }
    }
      cout << "\n";
  }

  //Constructors
  LAM::LAM(double* arr, int nCol){
    this->nRow = 4;
    this->nCol = nCol;
    this->Tab = new double[nRow * nCol];
    Setup(arr, nCol);
  }
}