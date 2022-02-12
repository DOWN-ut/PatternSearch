#include "DIPWM.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "LAM.h"
#include "LAT.h"

using namespace std;

namespace PatternSearch
{
	//Accessors
	string DIPWM::Id()
	{
		return this->id;
	}
	string DIPWM::GetText()
	{
		return this->text;
	}
	LAM* DIPWM::Lam()
	{
		return this->lam;
	}
	LAT* DIPWM::Lat()
	{
		return this->lat;
	}

	double DIPWM::Get(int x, int y)
	{
		return arr[x + (y * nCol)];
	}

	//Prints
	void DIPWM::DisplayTable() 
	{
		cout << id << "  |  " << nCol << "x" << nRow << endl;
		for (int y = 0; y < nRow; y++)	
		{
			for (int x = 0; x < nCol; x++)
			{
				cout << DIPWM::Get(x, y) << "  |  ";
			}
			cout << "\n";
		}
	}

	//Process
	void DIPWM::Setup()
	{
		id = "";

		vector<double> vect = vector<double>();
		nCol = 0; nRow = 0;

		string buffer = ""; int row = 0;
		for (int i = 0; i < text.length(); i++)
		{
			if (text[i] == '\n')	//Quand on arrive en bout de ligne, on passe � la ligne suivante
			{
				if (id == "") //Si l'id est encore vide, on est en train de lire de header
				{ 
					id = buffer; 
				}			
				else //Sinon, on ligne une des lignes du tableau
				{
					vect.push_back(stod(buffer));
					if (nRow <= 0) { nRow = row+1; }		//On est arriv� au bout de la ligne, donc on connait le nombre de ligne
					nCol++;								//Et on incr�mente le nombre de colonnes
				}
				buffer = "";
			}
			else if (text[i] == 32 || text[i] == 9 || text[i] == ',') //Si on lit un espace ou une tabulation, on passe � la colonne suivante
			{
				row++;
				vect.push_back(stod(buffer));
				buffer = "";
			}
			else 
			{
				buffer += text[i];
			}
		}

		arr = new double[nCol * nRow]; 
		row = 0; int col = 0;
		for (int i = 0; i < vect.size(); i++)
		{
			cout << vect.at(i) << " | ";
			arr[col + (row * nCol)] = vect.at(i);
			row++;
			if (row >= (nRow) )
			{
				row = 0;
				col++;
			}
		}
	}

	//Constructors
	DIPWM::DIPWM(string filePath)
	{
		text = "";

		ifstream file;
		file.open(filePath);

		string line;
		while (getline(file, line)) { text += line + "\n"; }

		file.close();

		Setup();
		this->lat = new LAT(this->arr, this->nCol, this->nRow);
		this->lam = new LAM(this->arr, this->nCol);
	}

	//Static
	int DIPWM::rowOfPair(char a, char b)
	{
		return 0;
	}

	double maxRowOf(double* arr, int nCol, int nRow, int col)
	{
		double m = 0;
		for (int c = 0; c < nRow; c++)
		{
			m = max(m, arr[col + (c * nCol)]);
		}
		return m;
	}
}