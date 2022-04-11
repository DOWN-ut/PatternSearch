
// PatternSearchClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#if defined(_WIN64_) || defined(_WIN32_) || defined(WIN64) || defined(WIN32)
    #include <Windows.h>
    #include "DIPWM.h"
    #include "LAT.h"
    #include "LAM.h"
#else
    #include "../PatternSearchLib/DIPWM.h"
    #include "../PatternSearchLib/LAT.h"
    #include "../PatternSearchLib/LAM.h"
    #include <libgen.h>         // dirname
    #include <unistd.h>         // readlink
    #include <linux/limits.h>   // PATH_MAX
#endif

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace PatternSearch;

std::string GetCurrentDirectory()
{
    #if defined(_WIN64_) || defined(_WIN32_) || defined(WIN64) || defined(WIN32)
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    #else
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        const char *path;
        if (count != -1) {
            path = dirname(result);
            return path;
        }
        else{
            return NULL;
        }
    #endif
}

int main(int argc, char * args)
{
    cout << "\n\nStart\n\n";


    //string file = "D:/Documents/Etudes/FAC/L3/TER/PatternSearch/Debug/FOXP1_HUMAN.H11DI.0.A.dpwm";
   string file = GetCurrentDirectory() + "/FOXP1_HUMAN.H11DI.0.A.dpwm";
   //string file = GetCurrentDirectory() + "\\" + "test.dpwm";

    cout << "\nLecture du fichier DIPWM : " << file << endl;

    DIPWM FOXP1 = DIPWM(file);
     
    cout << "\n\nContenu de la matrice :" << endl;

    FOXP1.DisplayTable();

    cout << "\n\nContenu de la LAT :" << endl;

    FOXP1.Lat()->DisplayTable();

    cout << "\n\nContenu de la LAM :" << endl;
   
    FOXP1.Lam()->DisplayTable();

    cout << "Valeur maximum :" << FOXP1.Lam()->GetMaxValue() << endl;
    cout << "Valeur minimum :" << FOXP1.Lam()->GetMinValue() << endl;

    cout << "\n\nEntrez un seuil (% du maximum):  ";

    double seuil;
    cin >> seuil;
    cout << endl;

    seuil = FOXP1.CalculateWords(seuil, GetCurrentDirectory());

    FOXP1.DisplayWords(10);

    FOXP1.WriteWordsFile(FOXP1.UsedSeuil(), GetCurrentDirectory());

    cout << "\n\n Entrez le fichier contenant la sequence a analyser : " << endl;
    string sequenceFile;
    cin >> sequenceFile;
    sequenceFile = GetCurrentDirectory() + "/" + sequenceFile;
    cout << "Analyse de la sequence dans : " << sequenceFile << endl;

    string sequence;
    ifstream fichier(sequenceFile);
    if (!fichier.good())
    {
        cout << "  ||>> Fichier introuvable " << endl;
        return 0;
    }

    getline(fichier, sequence);

    cout << "Sequence :  " << sequence << endl;

    vector<SearchResult> results = FOXP1.Search(sequence);

    cout << "\n Resultats : \n" << endl;
    
    for (int i = 0; i < results.size();i++) 
    {
        SearchResult r = results.at(i);
        cout << r.start << "-" << r.end << " >> " << r.str << endl;
    }

    cout << "\n\nEND" << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
