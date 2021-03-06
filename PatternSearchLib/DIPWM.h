#pragma once

#include <string>
#include "LAM.h"
#include "LAT.h"
#include <vector>
#include <thread>
#include <mutex>

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
		const double WordScore(const char* word);

		double UsedSeuil();
		int WordCount();

		//Processors
		void Setup();

		void SetupCoeur();
		float DispersionEntre(int deb, int fin);
		 
	
		/// <summary>
		/// Performs the enumeration of all the words with a score greater or equal to the given threshold. Check if there's already a file containing these words.
		/// </summary>
		/// <param name="seuil"> : The given threshold, percentage of the max score</param>
		/// <param name="currentLocation"> : The location of the file containing the words</param>
		/// <returns> : True if the words were calculated, False if they were recovered in the file</returns>
		bool EnumerateFullWords(double seuil,string currentLocation, bool multithread, bool displayProgression);

		/// <summary>
		/// Performs the enumeration of all the core-words with a reachable score greater or equal to the given threshold. Check if there's already a file containing these words.
		/// </summary>
		/// <param name="seuil"> : The given threshold, percentage of the max score</param>
		/// <param name="currentLocation"> : The location of the file containing the words</param>
		/// <returns> : True if the words were calculated, False if they were recovered in the file</returns>
		bool EnumerateCoreWords(double seuil, string currentLocation);

		//Search
		/// <summary>
		/// Search for all occurences of the last calculated words (currently contained in the words-array) in the given sequence.
		/// </summary>
		/// <param name="sequence"> : The sequence (part) to analyze</param>
		/// <param name="sequenceDeb"> : The start index of this sequence part relative to the whole sequence</param>
		/// <returns> : A vector containing each occurrences, with the start-index, the end-index and the word itself</returns>
		vector<SearchResult> Search(string sequence,int sequenceDeb,bool isCore);

		//Prints
		void DisplayTable();

		void DisplayWords(int count, bool isCore); //-1 pour tout afficher

		//Constructors
		DIPWM();
		DIPWM(string file);

		//Static
		static char CharOf(char c);
		static char CharId(char c);

		static int RowOfPair(char a, char b);

		static double maxRowOf(double* arr, int nCol, int col);

		//Files
		bool ParsingFileData(string header, string data,bool isCore);
		void WriteWordsFile(double seuil, string currentLocation,bool isCore);
		bool ReadWordFile(string fileName, string currentLocation,bool isCore);
		void WritesFinalSequenceWordsFile(vector<SearchResult> results, string currentLocation, string sequenceFile);
		string FileName(double seuil,bool isCore);

	private:

		string id;
		string text;

		double* arr;
		int nCol;int nRow;
		
		int coeurDeb; int coeurFin; int coreLenght; float coeurDisp; //Index de d?but et de fin du coeur, et dispersion

		double maxValue;
		double minValue;

		int wordLength;
		char* words;
		float* scores;
		int wordCount;
		double usedSeuil;

		//trie searcher;

		mutex enumerationMutex;
		int currentThreadCount;

		static void FullWordRecursion(DIPWM* dipwm, vector<char>* vect, vector<float>* vectS,char* word, double seuil,int pos,double score, bool isMain);

		void CoreWordRecursion(vector<char>* vect, vector<float>* vectS, char* word, double seuil, int pos,  int end, double score);

		bool SearchFile(double seuil, string currentLocation,bool isCore);

		

		void FillEnumerationArray(vector<char> * vect, vector<float> * vectS,int wordL);

		LAM* lam;
		LAT* lat;

	};
}
