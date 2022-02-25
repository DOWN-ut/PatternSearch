#pragma once

#include <string>
#include "LAM.h"
#include "LAT.h"
#include <vector>

using namespace std;

namespace PatternSearch
{
	class DIPWM
	{
	public:

		//Accessors
		string Id();
		string GetText();
		LAM* Lam();
		LAT* Lat();
		double Get(int x, int y);

		//Processors
		void Setup();

		void CalculateWords(double seuil);

		//Prints
		void DisplayTable();

		void DisplayWords();

		//Constructors
		DIPWM();
		DIPWM(string file);

		//Static
		static int rowOfPair(char a, char b);

		static double maxRowOf(double* arr, int nCol, int col);

	private:

		string id;
		string text;

		double* arr;
		int nCol;int nRow;

		char* words;
		int wordCount;

		void RecursiveWorder(vector<char>* vect,char* word, double seuil,int pos);

		LAM* lam;
		LAT* lat;

	};
}