#pragma once

#include <string>
#include "LAM.h"
#include "LAT.h"
#include <vector>

using namespace std;

namespace PatternSearch
{
	struct SearchResult {
		int start;
		int end;
		string str;
	};

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

		double UsedSeuil();

		//Processors
		void Setup();

		bool CalculateWords(double seuil,string currentLocation); //Retourne TRUE si un calcul a du etre fait, FALSE si on a recup les donnees dans un fichier

		//Search
		vector<SearchResult> Search(string sequence);

		//Prints
		void DisplayTable();

		void DisplayWords(int count); //-1 pour tout afficher

		//Constructors
		DIPWM();
		DIPWM(string file);

		//Static
		static char CharOf(char c);
		static char CharId(char c);

		static int RowOfPair(char a, char b);

		static double maxRowOf(double* arr, int nCol, int col);

		//Files
		bool ParsingFileData(string header, string data);
		void WriteWordsFile(double seuil, string currentLocation);
		bool ReadWordFile(double seuill, string currentLocation);
		string FileName(double seuil);

	private:

		string id;
		string text;

		double* arr;
		int nCol;int nRow;
		double maxValue;

		int wordLength;
		char* words;
		int wordCount;
		double usedSeuil;

		//trie searcher;

		void RecursiveWorder(vector<char>* vect,char* word, double seuil,int pos,double score);

		LAM* lam;
		LAT* lat;

	};
}