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

	double DIPWM::ScoreOf(char c0, char c1,int pos)
	{
		return Get(pos,(c0*4) + c1);
	}

	double DIPWM::WordScore(char* word)
	{
		double s = 0;
		for (int i = 1; i < wordLength; i++)
		{
			s += ScoreOf(CharId(word[i - 1]), CharId(word[i]), i - 1);
		}
		return s;
	}

	//Prints
	void DIPWM::DisplayTable() 
	{
		cout << id << "  |  " << nCol << "x" << nRow << "  |  max_score = " << maxValue << endl;
		for (int y = 0; y < nRow; y++)	
		{
			for (int x = 0; x < nCol; x++)
			{
				cout << DIPWM::Get(x, y) << "  |  ";
			}
			cout << "\n";
		}
	}

	void DIPWM::DisplayWords()
	{
		cout << endl << wordCount << " words \n";
		for (int i = 0; i < wordCount; i++)
		{
			cout << endl << " | "; double s = WordScore(&words[i * wordLength]);
			for (int o = 0; o < wordLength; o++)
			{
				cout << words[(i * wordLength) + o] << " | ";
			}
			cout << "= " << s;
		}
		cout<<endl;
	}

	//Process
	void DIPWM::Setup()
	{
		id = "";

		vector<double> vect = vector<double>();
		nCol = 0; nRow = 0;

		string buffer = ""; int row = 0;
		for (int i = 1; i < text.length(); i++)
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

	void DIPWM::RecursiveWorder(vector<char>* vect,char* buffer,double seuil,int pos,double score)
	{
		//Si on est arrivé à la dernière lettre,
		if (pos >= wordLength)
		{
			//On ajoute ce mot à la liste, en copiant le buffer dans le vect
			for (int i = 0; i < wordLength; i++)
			{
				vect->push_back(buffer[i]);
			}
			return;
		}
		else
		{
			for (char c = 0; c < 4; c++)
			{
				double nscore = score + ScoreOf(buffer[pos - 1], c, pos - 1);	//Défine le nouveau score pour cet ajout de lettre

				if ( (pos == wordLength - 1) && nscore < seuil)			//Si on est en train de placer la dernière lettre, on vérifie que le score final soit suffisant
				{
					continue;
				}
				else if (nscore + lam->MaxOf(c, pos) < seuil)	//Sinon, on vérigie que le nouveau score maximal atteignable soit suffisant
				{
					continue;
				}
				else										//Si les conditions sont remplies, on continue
				{
					buffer[pos] = c;
					RecursiveWorder(vect, buffer, seuil, pos + 1, nscore);
				}
			}
		}
	}

	void DIPWM::CalculateWords(double seuil)
	{
		double seuilVal = seuil * maxValue;

		cout << "Calcul des mots avec un seuil de " << seuilVal << endl;

		vector<char> vect = vector<char>();

		for (char c = 0; c < 4; c++) //On traite séparément les mots qui commencent pas A T C ou G
		{
			char* buffer = new char[wordLength];
			buffer[0] = c;					//On initialise la première lettre du buffer
			RecursiveWorder(&vect, buffer, seuilVal, 1, 0);
		}

		//Copie dans le tableau principal
		wordCount = vect.size() / wordLength;
		words = new char[vect.size()];
		for (int i = 0; i < vect.size(); i++)
		{
			words[i] = CharOf(vect.at(i));
		}
	}

	//Files
	void DIPWM::WriteWordsFile(double seuil, string currentLocation) 
	{
		string header = id + "	" + to_string(wordLength) + "	" + to_string(wordCount) + "	" + to_string(seuil) + '\n'; //Header : id  tailles de mots nombre de mots
		string fileName = id + to_string(seuil) + "Words.txt";
		string path = currentLocation + "\\" + fileName;

		cout << path << endl;
		ofstream fichier(path);
		
		if (fichier.bad())
			printf("probleme creation fichier \n"); 

		fichier << header << endl;
		fichier << words;
		
		printf("fichier creer");
		fichier.close();
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

		maxValue = lam->GetMaxValue();
		wordLength = nCol + 1;
	}

	//Static
	char DIPWM::CharOf(char c) {
		return
			c == 0 ? 'A' :
			c == 1 ? 'T' :
			c == 2 ? 'C' :
			c == 3 ? 'G' : '?';
	}

	char DIPWM::CharId(char c)
	{
		return
			c == 'A' ? 0 :
			c == 'T' ? 1 :
			c == 'C' ? 2 :
			c == 'G' ? 3 : 0;
	}

	int DIPWM::RowOfPair(char a, char b)
	{
		int ai =
			a == 'A' ? 0 :
			a == 'T' ? 1 :
			a == 'C' ? 2 :
			a == 'G' ? 3 : 0;

		int bi = 
			b == 'A' ? 0 :
			b == 'T' ? 1 :
			b == 'C' ? 2 :
			b == 'G' ? 3 : 0;

		return (ai * 4) + bi;

	}

}