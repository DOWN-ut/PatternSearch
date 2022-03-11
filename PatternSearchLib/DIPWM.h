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

		double ScoreOf(char c0, char c1,int pos);
		double WordScore(char* word);

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
		static char CharOf(char c);
		static char CharId(char c);

		static int RowOfPair(char a, char b);

		static double maxRowOf(double* arr, int nCol, int col);

	private:

		string id;
		string text;

		double* arr;
		int nCol;int nRow;
		double maxValue;

		int wordLength;
		char* words;
		int wordCount;

		void RecursiveWorder(vector<char>* vect,char* word, double seuil,int pos,double score);

		LAM* lam;
		LAT* lat;

	};
}