#include "DIPWM.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "LAM.h"
#include "LAT.h"
#include "aho_corasick.hpp"
#if defined(_WIN64_) || defined(_WIN32_) || defined(WIN64) || defined(WIN32)  || defined(_WIN64) || defined(_WIN32)
#else
#include <cmath>
#endif

using namespace std;
using namespace aho_corasick;

#define RESETCOLOR "\033[0m"
#define REDCOLOR     "\033[31m" 

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

	const double DIPWM::WordScore(const char* word)
	{
		double s = 0;
		for (int i = 1; i < wordLength; i++)
		{
			s += ScoreOf(CharId(word[i - 1]), CharId(word[i]), i - 1);
		}
		return s;
	}

	double DIPWM::UsedSeuil() { return usedSeuil; }
	int DIPWM::WordCount() { return wordCount; }

	//Prints
	void DIPWM::DisplayTable() 
	{
		cout << id << "  |  " << nCol << "x" << nRow << "  |  max_score = " << maxValue << " | coeur [" << coeurDeb << "-" << coeurFin << "] [" << coreLenght << "] disp = " << coeurDisp << endl;
		for (int y = 0; y < nRow; y++)	
		{
			for (int x = 0; x < nCol; x++)
			{
				int val = (int)((DIPWM::Get(x, y) *100) / 100.0);

				if (x >= coeurDeb && x <= coeurFin) { cout << REDCOLOR; }			

				cout << (val >= 0 ? " " : "") << val << " | ";

				if (x >= coeurDeb && x <= coeurFin) { cout << RESETCOLOR; }
				
			}
			cout << "\n";
		}
	}

	void DIPWM::DisplayWords(int count, bool isCore)
	{
		int wl = isCore ? coreLenght : wordLength;
		cout << endl << wordCount << " words \n";
		for (int i = 0; i < wordCount; i++)
		{
			cout << endl << " | "; double s = scores[i];//WordScore(&words[i * wordLength]);
			for (int o = 0; o < wl; o++)
			{
				cout << words[(i * wl) + o] << " | ";
			}
			cout << "= " << s;
			count--;
			if (count == 0) { break; }
		}
		cout << endl << endl;
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
			arr[col + (row * nCol)] = vect.at(i);
			row++;
			if (row >= (nRow) )
			{
				row = 0;
				col++;
			}
		}
	}

	void DIPWM::SetupCoeur()
	{
		coeurDeb = 0; coeurFin = 1; coeurDisp = 0; //Mémorisation des valeurs optimales

		for (int deb = 0; deb < nCol - 1; deb++)  //L'index de début commence à 0 et termine à nCol-2
		{
			for (int fin = deb + 1; fin < nCol; fin++) //L'index de fin commence à 1 et termine à nCol-1
			{
				float v = DispersionEntre(deb, fin);
				if (v > coeurDisp) { coeurDisp = v; coeurDeb = deb; coeurFin = fin; }
			}
		}

		coreLenght = coeurFin - coeurDeb + 1;
	}

	float DIPWM::DispersionEntre(int deb, int fin)
	{ 
		double moyenne = 0;
		for (int i = deb; i < fin; i++)
		{
			moyenne += arr[i];		
		}
		moyenne /= (fin - deb);
	
		double carres = 0;
		for (int i = deb; i < fin; i++)
		{
			carres += abs(arr[i]-moyenne);
		}
		carres /= (fin - deb);

		return sqrt(carres);
	}

	//Enumeration
	void DIPWM::FullWordRecursion(DIPWM* dipwm, vector<char>* vectW, vector<float>* vectS, char* buffer, double seuil, int pos, double score, bool isMain)
	{
		//Si on est arrivé à la dernière lettre,
		if (pos >= dipwm->wordLength)
		{
			dipwm->enumerationMutex.lock();
			//On ajoute ce mot à la liste, en copiant le buffer dans le vect
			for (int i = 0; i < dipwm->wordLength; i++)
			{
				vectW->push_back(buffer[i]);
			}
			vectS->push_back(score);
			dipwm->enumerationMutex.unlock();
			return;
		}
		else
		{
			for (char c = 0; c < 4; c++)
			{
				double nscore = score + dipwm->ScoreOf(buffer[pos - 1], c, pos - 1);	//Défine le nouveau score pour cet ajout de lettre

				if ((pos == dipwm->wordLength - 1) && nscore < seuil)			//Si on est en train de placer la dernière lettre, on vérifie que le score final soit suffisant
				{
					continue;
				}
				else if (nscore + dipwm->lam->MaxLeftOf(c, pos) < seuil)	//Sinon, on vérigie que le nouveau score maximal atteignable soit suffisant
				{
					continue;
				}
				else										//Si les conditions sont remplies, on continue
				{
					buffer[pos] = c;
					FullWordRecursion(dipwm, vectW, vectS, buffer, seuil, pos + 1, nscore,false);
				}
			}
		}

		if (isMain) {
			dipwm->enumerationMutex.lock();
			dipwm->currentThreadCount--;
			dipwm->enumerationMutex.unlock();
		}
	}

	void DIPWM::CoreWordRecursion(vector<char>* vectW, vector<float>* vectS, char* buffer, double seuil, int pos, int end, double score)
	{
		//Si on est arrivé à la dernière lettre,
		if (pos > end)
		{
			//On ajoute ce mot à la liste, en copiant le buffer dans le vect
			for (int i = 0; i < coreLenght; i++)
			{
				vectW->push_back(buffer[i]);
			}
			vectS->push_back(score);
			return;
		}
		else
		{
			for (char c = 0; c < 4; c++)
			{
				double nscore = score + ScoreOf(buffer[pos - 1 - coeurDeb], c, pos - 1);	//Défine le nouveau score pour cet ajout de lettre

				if (nscore + lam->MaxLeftOf(c, pos) < seuil)	//On vérifie que le nouveau score maximal atteignable soit suffisant
				{
					continue;
				}
				else										//Si les conditions sont remplies, on continue
				{
					buffer[pos-coeurDeb] = c;
					CoreWordRecursion(vectW, vectS, buffer, seuil, pos + 1,end, nscore);
				}
			}
		}
	}

	void DIPWM::FillEnumerationArray(vector<char>* vectW, vector<float>* vectS,int wordL)
	{
		//Copie dans le tableau principal
		wordCount = vectW->size() / wordL;
		words = new char[vectW->size()];
		for (int i = 0; i < vectW->size(); i++)
		{
			words[i] = CharOf(vectW->at(i));
		}
		scores = new float[vectS->size()];
		for (int i = 0; i < vectS->size(); i++)
		{
			scores[i] = vectS->at(i);
		}
	}

	bool DIPWM::EnumerateFullWords(double seuil, string currentLocation, bool displayProgression)
	{
		double seuilVal = ((maxValue - minValue) * seuil) + minValue;

		if (SearchFile(seuilVal, currentLocation, false)) { return false; }

		cout << "Launching threads to calculate words with a threshold of " << seuilVal << endl;

		vector<char> vectW = vector<char>();
		vector<float> vectS = vector<float>();

		thread threads[4];

		for (char c = 0; c < 4; c++) //On traite séparément les mots qui commencent pas A T C ou G
		{
			char* buffer = new char[wordLength];
			buffer[0] = c;					//On initialise la première lettre du buffer

			currentThreadCount++;
			threads[c] = thread(FullWordRecursion, this, &vectW, &vectS, buffer, seuilVal, 1, 0, true);
		}

		cout << "  ||>> Waiting for all threads" << endl;

		if (displayProgression) {
			cout << "  || count : "; int lastSize = 0;

			while (currentThreadCount > 0)
			{
				for (int i = 0; i < lastSize; i++) { cout << '\b'; }

				string str = to_string(vectW.size() / wordLength);
				lastSize = str.size();

				for (int i = 0; i < lastSize; i++) { cout << str[i]; }

				this_thread::sleep_for(chrono::milliseconds(50));
			}
			cout << " END " << endl;
		}

		for (int i = 0; i < 4; i++)
		{
			threads[i].join();
		}

		cout << "  ||>> Filling arrays " << endl;

		FillEnumerationArray(&vectW, &vectS, wordLength);

		usedSeuil = seuilVal;

		return true;
	}

	bool DIPWM::EnumerateCoreWords(double seuil, string currentLocation)
	{
		double seuilVal = ((maxValue - minValue) * seuil) + minValue;

		if (SearchFile(seuilVal, currentLocation,true)) { return false; }

		cout << "Calculating core words with a threshold of " << seuilVal << endl;

		vector<char> vectW = vector<char>();
		vector<float> vectS = vector<float>();

		for (char c = 0; c < 4; c++) //On traite séparément les mots qui commencent pas A T C ou G
		{
			double prefixScore = lam->MaxRightOf(c, coeurDeb);

			char* buffer = new char[coreLenght];
			buffer[0] = c;					//On initialise la première lettre du buffer
			CoreWordRecursion(&vectW, &vectS, buffer, seuilVal, coeurDeb+1,coeurFin, prefixScore);
		}

		FillEnumerationArray(&vectW, &vectS, coreLenght);

		usedSeuil = seuilVal;

		return true;
	}

	//Search
	vector<SearchResult> DIPWM::Search(string sequence, bool isCore)
	{
		trie searcher; //searcher.case_insensitive();
		
		int wl = isCore ? coreLenght : wordLength;
		for (int i = 0; i < wordCount; i++)
		{
			string word;
			for (int o = 0; o < wl; o++)
			{
				word += words[(i * wl) + o];
			}
			searcher.insert(word);
		}
		
		vector<emit<char>> tokens = searcher.parse_text(sequence);
		vector<SearchResult> vect;
		
		for (int t = 0; t < tokens.size();t++)
		{
			emit<char> token = tokens.at(t);

			SearchResult sr;

			if (!isCore) {
				sr.end = token.get_start();
				sr.start = token.get_end();

				sr.str = token.get_keyword();

				vect.push_back(sr);
			}
			else {
				sr.end = token.get_end() + (wordLength - coeurFin) - 1;
				sr.start = token.get_start() - coeurDeb;

				if (sr.start < 0 || sr.end >= sequence.size()) { continue; }

				sr.str = sequence.substr(sr.start, sr.end);

				if (WordScore(sr.str.c_str()) >= usedSeuil)
				{
					vect.push_back(sr);
				}
			}
		}

		return vect;
	}

	//Files

	bool DIPWM::SearchFile(double seuilVal, string currentLocation, bool isCore)
	{
		if (currentLocation != "")
		{
			cout << "Searching for a file containing " << (isCore ? "core" : "") << " words with a threshold of : " << seuilVal << endl;

			string fileName = FileName(seuilVal, isCore);

			if (ReadWordFile(fileName, currentLocation,isCore))
			{
				cout << "  >>  File found and data recovered : calculation canceled" << endl;
				return true;
			}
			else {
				cout << "  >>  File not found " << endl; return false;
			}
		}
		else {
			cout << "  >>  Location is null : skipping file research" << endl;
			return false;
		}
	}

	string DIPWM::FileName(double seuil, bool isCore)
	{
		string ss = to_string(seuil).substr(0, 10);

		if (isCore) {
			return id + "_core_" + ss + ".dpwmw";
		}
		else {
			return id + '_' + ss + ".dpwmw";
		}
	}

	bool DIPWM::ReadWordFile(string fileName, string currentLocation,bool isCore)
	{
		string path = currentLocation + "/" + fileName;

		cout << "  |>>  Searching for file : " << path << endl;

		ifstream fichier(path);

		if (!fichier.good())
		{
			cout << "  |!!  File not found " << endl;
			return false;
		}

		cout << "  |>>  File found : start reading" << endl;

		string line; int i = 0;
		string header = ""; string data = "";
		while (getline(fichier, line)) 
		{ 
			if (i == 0)	//Header
			{
				header += line;
			}
			else //Donnees 
			{
				data += line + '\n'; //<< On utilise getline, donc le \n est ignoré
			}
			i++;
		}

		if (header == "")
		{
			cout << "  |!!  File empty : aborting " << endl;
			return false;
		}

		cout << "  |>>  File read : parsing data " << endl;

		if(!ParsingFileData(header, data,isCore))
		{
			return false;
		}

		fichier.close();

		return true;
	}

	bool DIPWM::ParsingFileData(string header, string data, bool isCore)

	{
		cout << "    |>>  Header : " << header << endl;

		string* headerDat = new string[4]; int o = 0;
		for (int i = 0; i < 4; i++)		//On  va remplir un par un chaque element du header
		{
			headerDat[i] = "";
			while (header[o] != ' ')		//On parcourre le header jusqu'à tomber sur un séparateur
			{
				headerDat[i] += header[o];	//On memorise l'élement actuellement lu du header
				o++;
				if (o >= header.length()) { break; }
			}
			o++;
		}

		cout << "    |>>  ID : " << headerDat[0] << "  |  Wordcount : " << headerDat[2] << "  |  WordLength : " << headerDat[1] << "  |  Threshold : " << headerDat[3] << endl;
		
		//if (!isCore) { wordLength = atoi(headerDat[1].c_str()); } inutile
		int wl = isCore ? coreLenght : wordLength;

		wordCount = atoi(headerDat[2].c_str());
		usedSeuil = atof(headerDat[3].c_str());

		cout << "    |>>  Wordcount : " << wordCount << "  |  WordLength : " << wl << "  |  Threshold : " << usedSeuil << endl;

		if (wordCount <= 0) { cout << "    |!! Error : wordcount = 0 " << endl; return false; }

		int l = wordCount * wl;

		if (l >= data.length())
		{
			cout << "    |!!  Recovered data is corrupted : too short " << endl;
			return false;
		}

		words = new char[l];
		scores = new float[wordCount];

		int wid = 0;//Id de la lettre qu'on ajoute au tableau de mot
		int sid = 0;//Id du score actuellement lu
		for (int i = 0; i < data.length(); i++)
		{
			while (data[i] != '>')
			{
				words[wid] = data[i];
				i++; wid++;
			}
			i++; string buffer = "";
			while (data[i] != '\n')
			{
				buffer += data[i];
				i++;
			}
			scores[sid] = atof(buffer.c_str());
			sid++;
		
			if (wid >= l) { break; }
		}

		cout << "    |>>  Data loaded successfully : ending parsing" << endl;

		return true;
	}

	void DIPWM::WriteWordsFile(double seuil, string currentLocation,bool isCore) 
	{
		int wl = isCore ? coreLenght : wordLength;

		string header = id + ' ' + to_string(wl) + ' ' + to_string(wordCount) + ' ' + to_string(seuil) + '\n'; //Header : id seuil tailles de mots nombre de mots
		
		string fileName = FileName(seuil, isCore);
		string path = currentLocation + "/" + fileName;

		cout << "Writing word file at : " << path << endl;
		ofstream fichier(path);
		
		if (fichier.bad()) {
			cout << "  |!!  File creation failed" << endl;
		}

		fichier << header;

		for (int i = 0; i < wordCount; i++)
		{
			for (int o = 0; o < wl; o++)
			{
				fichier << words[(i * wl) + o];		
			}
			fichier << ">" << scores[i] << "\n";
		}
		
		fichier << EOF;

		cout << "File successfully created" << endl;
		fichier.close();
	}

	void DIPWM::WritesFinalSequenceWordsFile(vector<SearchResult> results, string currentLocation, string sequenceFile)
	{
		string filename =  sequenceFile; //this->id + to_string(this->usedSeuil) +
		
		string path = currentLocation + "/" + filename;
		
		ofstream file(path);

		if (!file.is_open()) {
			cout << "failed to open " << filename << '\n';
		}
		else {
			for (int i = 0; i < results.size(); i++)
			{
				SearchResult r = results.at(i);
				file << r.start << "-" << r.end << "-" << r.str <<"-"<<WordScore(r.str.c_str())<< endl;
			}
		}

		file << EOF;

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
		SetupCoeur();
		this->lat = new LAT(this->arr, this->nCol, this->nRow);
		this->lam = new LAM(this->arr, this->nCol,this->nRow);

		maxValue = lam->GetMaxValue();
		minValue = lam->GetMinValue();
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
