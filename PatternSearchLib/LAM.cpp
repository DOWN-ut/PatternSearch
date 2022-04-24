#include "DIPWM.h"
#include "LAM.h"
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

namespace PatternSearch
{

  //Processors

    void LAM::Set(double* toFill,double v, int col, int row) {
        toFill[col + (row * nCol)] = v;
    }

    double LAM::Get(double * arr ,int col, int row)
    {
        return arr[col + (row * nCol)];
    }

    double LAM::GetLeft(int col, int row)
    {
        return Get(tabLeft, col, row);
    }

    double LAM::GetRight(int col, int row)
    {
        return  Get(tabRight, col, row);
    }

    double LAM::GetMaxValue() { return maxValue; }

    double LAM::GetMinValue() { return minValue; }

    double LAM::MaxLeftOf(char c, int pos)
    {
        return GetLeft(pos, (int)c);
    }

    double LAM::MaxRightOf(char c, int pos)
    {
        return GetRight(pos, (int)c);
    }

    double max4(double a, double b, double c, double d)
    {
        return max(a, max(b, max(c, d)));
    }

    double min4(double a, double b, double c, double d)
    {
        return min(a, min(b, min(c, d)));
    }

    void LAM::Setup(double* toFill,double* arr, int nCol, bool maxOrMin) { // maxOrMin = true si on souhaite calculer la LAM basique et obtenir maxValue, maxOrMin = false pour la LAM "négative" et obtenir minValue

      if(maxOrMin){
        //On remplit la derni�re colonne avec les maximums de la derni�re colonne de la matrice
        for (int i = 0; i < nRow; i++)
        {
            double v = max4(
                arr[nCol - 1 + (((4 * i)) * nCol)],
                arr[nCol - 1 + (((4 * i) + 1) * nCol)],
                arr[nCol - 1 + (((4 * i) + 2) * nCol)],
                arr[nCol - 1 + (((4 * i) + 3) * nCol)]);

            Set(toFill, v, nCol - 1, i);
        }

        //Pour les colonnes suivantes :
        for (int c = nCol - 2; c >= 0; c--)
        {
            for (int i = 0; i < nRow; i++)
            {
                double v = max4(
                    arr[c + (((4 * i)) * nCol)] + Get(toFill,c+1,0),
                    arr[c + (((4 * i) + 1) * nCol)] + Get(toFill, c + 1, 1),
                    arr[c + (((4 * i) + 2) * nCol)] + Get(toFill, c + 1, 2),
                    arr[c + (((4 * i) + 3) * nCol)] + Get(toFill, c + 1, 3));

                Set(toFill,v, c, i);
            }
        }

        maxValue = max4(Get(toFill, 0, 0), Get(toFill, 0, 1), Get(toFill, 0, 2), Get(toFill, 0, 3));
      }
      else{
        //On remplit la dernière colonne avec les minimums de la dernière colonne de la matrice
        for(int i = 0; i < nRow; i++)
        {
          double v = min4(
            arr[nCol - 1 + (((4 * i)) * nCol)],
            arr[nCol - 1 + (((4 * i) + 1) * nCol)],
            arr[nCol - 1 + (((4 * i) + 2) * nCol)],
            arr[nCol - 1 + (((4 * i) + 3) * nCol)]);

          Set(toFill,v, nCol-1, i);
        }

        // colonnes suivantes :
        for(int c = nCol - 2; c >= 0; c--)
        {
          for (int i = 0; i < nRow; i++)
          {
            double v = min4(
              arr[c + (((4 * i)) * nCol)] + Get(toFill, c+1, 0),
              arr[c + (((4 * i) + 1) * nCol)] + Get(toFill, c+1, 1),
              arr[c + (((4 * i) + 2) * nCol)] + Get(toFill, c+1, 2),
              arr[c + (((4 * i) + 3) * nCol)] + Get(toFill, c+1, 3));

            Set(toFill,v, c, i);
          }
        }

        minValue = min4(Get(toFill, 0,0), Get(toFill, 0,1), Get(toFill, 0,2), Get(toFill, 0,3));
      }
    }

  void LAM::FlipTable(double* arrFrom,double* arrTo, int nCol){

    // pour les A?
    int add = 0;
    for (int i = 0; i < nCol; i++){
      for (int j = 0; j < nCol; j++){
        if (i % 5 == 0){
            arrTo[(i*nCol) + j] = arrFrom[(i*nCol) + j];
        }
        else{
            arrTo[(i * nCol) + j] = arrFrom[(i*nCol) + (add*nCol) + j];
        }
      }
      add+=3;
    }

    // pour les C?
    add = -3;
    for (int i = 4; i < 2*nCol; i++){
      for (int j = 0; j < nCol; j++){
        if (i % 5 == 0){
            arrTo[(i*nCol) + j] = arrFrom[(i*nCol) + j];
        }
        else{
            arrTo[(i * nCol) + j] = arrFrom[(i*nCol) + (add*nCol) + j];
        }
      }
      add+=3;
    }

    //pour les G?
    add = -6;
    for (int i = 8; i < 3*nCol; i++){
      for (int j = 0; j < nCol; j++){
        if (i % 5 == 0){
            arrTo[(i*nCol) + j] = arrFrom[(i*nCol) + j];
        }
        else{
            arrTo[(i * nCol) + j] = arrFrom[(i*nCol) + (add*nCol) + j];
        }
      }
      add+=3;
    }

    //pour les T?
    add = -9;
    for (int i = 12; i < 4*nCol; i++){
      for (int j = 0; j < nCol; j++){
        if (i % 5 == 0){
            arrTo[(i*nCol) + j] = arrFrom[(i*nCol) + j];
        }
        else{
            arrTo[(i * nCol) + j] = arrFrom[(i*nCol) + (add*nCol) + j];
        }
      }
      add+=3;
    }
  }

  void LAM::FlipLAM(double* arrFrom, double* arrTo, int nCol){ // inversion horizontale de la LAM
    for (int i = 0; i < nCol; i++){
      for(int j = 0; j < nCol; j++){
        arrTo[(i * nCol) + j] = arrFrom[(i * nCol) + nCol - 1 - j];
      }
    }
  }

  void LAM::DisplayLeftTable() { DisplayTable(tabLeft); }
  void LAM::DisplayRightTable() { DisplayTable(tabRight); }
  void LAM::DisplayTable(double* arr){

      cout << nCol << " x " << nRow << endl;
    for(int j = 0; j < nRow; j++){
      cout << "\n";
      for (int i=0; i<nCol; i++){
          cout << Get(arr,i,j) << "  |  ";
      }
    }
      cout << "\n";
  }

  //Constructors
  LAM::LAM(double* arr, int nCol, int nr){
    this->nRow = 4;
    this->nCol = nCol;

    this->tabLeft = new double[nRow * nCol];
    this->tabRight = new double[nRow * nCol];

    this->flippedTab = new double[nRow * nCol];

    Setup(tabLeft, arr, nCol, false); // pour récupérer minValue en premier car la LAM négative est inutile
    Setup(tabLeft, arr, nCol, true); // on récupère ensuite la vraie LAM pour l'afficher + maxValue

    double * flippedArr = new double[nCol * nr];
    
    return;
    FlipTable(arr,flippedArr, nCol);

    Setup(tabRight, flippedArr, nCol, true);
  }
}