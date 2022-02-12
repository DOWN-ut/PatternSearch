#include "DIPWM.h"
#include "LAM.h"
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

namespace PatternSearch
{

  //Processors
  double LAM::max4(double a, double b, double c, double d){
    double max = std::max({a, b, c, d});
    return max;
  }

  double LAM::score(int pos, int lettre, double* arr, int nCol){
    int c1 = lettre * 4;
    int c2 = lettre * 4 + 1;
    int c3 = lettre * 4 + 2;
    int c4 = lettre * 4 + 3;
    if(pos == nCol -1){
      return max4(arr[pos * nCol + c1], arr[pos * nCol +c2], arr[pos * nCol + c3], arr[pos * nCol + c4]);
    }
    else{
      return max4(
        (arr[pos * c1] + score(pos + 1, 0, arr, nCol)),
        (arr[pos * c2] + score(pos + 1, 1, arr, nCol)),
        (arr[pos * c3] + score(pos + 1, 2, arr, nCol)),
        (arr[pos * c4] + score(pos + 1, 3, arr, nCol)));
    }
  }

  void LAM::Setup(double* arr, int nCol){
    for(int i = 0; i < nCol - 1; i++){
      for(int j = 0; j < nRow; j++){
        Tab[i*nCol + j] = score(j,i,arr,nCol);
      }
    }
  }

  void LAM::DisplayTable(){
    for(int j = 0; j < nRow; j++){
      cout << "\n";
      for (int i=0; i<nCol; i++){
          cout << Tab[i*nCol + j] << "  |  ";
      }
    }
      cout << "\n";
  }

  //Constructors
  LAM::LAM(double* arr, int nCol){
    this->nRow = 4;
    this->Tab = new double[nRow * nCol];
    Setup(arr, nCol);
  }
}