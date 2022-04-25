
// PatternSearchClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#if defined(_WIN64_) || defined(_WIN32_) || defined(WIN64) || defined(WIN32)  || defined(_WIN64) || defined(_WIN32)
    #include <Windows.h>
    #include "../PatternSearchLib/DIPWM.h"
    #include "../PatternSearchLib/LAT.h"
    #include "../PatternSearchLib/LAM.h"
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
    #if defined(_WIN64_) || defined(_WIN32_) || defined(WIN64) || defined(WIN32) || defined(_WIN64) || defined(_WIN32)
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

    cout << "\n\n Entrez le fichier contenant la DIPWM a analyser : " << endl;
    string DIPWMFILE;
    cin >> DIPWMFILE;
    string file = GetCurrentDirectory() + "/" + DIPWMFILE ;

    ifstream fichierD(file);
    if (!fichierD.good())
    {
        cout << "  ||>> Fichier introuvable " << endl;
        return 0;
    }
    
    //string file = "D:/Documents/Etudes/FAC/L3/TER/PatternSearch/Debug/motif_HUMAN.H11DI.0.A.dpwm";

   //string file = GetCurrentDirectory() + "\\" + "test.dpwm";

    cout << "\nLecture du fichier DIPWM : " << file << endl;

    DIPWM motif(file);
     
    cout << "\n\nContenu de la matrice :" << endl;

    motif.DisplayTable();

    cout << "\n\nContenu de la LAT :" << endl;

    motif.Lat()->DisplayTable();

    cout << "\n\nContenu de la LAM :" << endl;
   
    motif.Lam()->DisplayLeftTable();
    motif.Lam()->DisplayRightTable();

    cout << "Valeur maximum :" << motif.Lam()->GetMaxValue() << endl;
    cout << "Valeur minimum :" << motif.Lam()->GetMinValue() << endl;

    bool isCore;
    while (true)
    {
    cout << "\n\nEntrez un seuil (% du maximum):  ";

    double seuil; cin >> seuil;cout << endl;

        cout << "Entrez <f> pour enumerer les mots complets, <c> pour les mots du coeur" << endl;

        char mode;   cin >> mode; cout << endl;

        if (mode == 'f') {
            seuil = motif.EnumerateFullWords(seuil, GetCurrentDirectory(),true); isCore = false;
        }
        else if (mode == 'c')
        {
            seuil = motif.EnumerateCoreWords(seuil, GetCurrentDirectory()); isCore = true;
        }
        else {
            cout << "  >>  Mauvaise entree" << endl; continue;
        }


        if (motif.WordCount() <= 0) {
            cout << "  >> Aucun mot genere !" << endl; continue;
        }
        else{ break; }
    }

    motif.DisplayWords(10,isCore);

    cout << "\nEcrire un fichier de mots ? <y> ou <n>" << endl;
    char write; cin >> write;
    if (write == 'y') {
        motif.WriteWordsFile(motif.UsedSeuil(), GetCurrentDirectory(), isCore);
    }

    char loopSeq = 'y';
    while (loopSeq == 'y') 
    {
        cout << "\n\nEntrez le fichier contenant la sequence a analyser : " << endl;
        string sequenceFile;
        cin >> sequenceFile;
        string sequenceName = sequenceFile;
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

        vector<SearchResult> results = motif.Search(sequence,isCore);

        cout << "\n>> " << results.size() << " resultats" << endl;

        for (int i = 0; i < results.size(); i++)
        {
            SearchResult r = results.at(i);
            cout << "   |" << r.start << "-" << r.end << " >> " << r.str << endl;
        }

        motif.WritesFinalSequenceWordsFile(results, GetCurrentDirectory(), sequenceName);

        cout << "\nEntrez (y) pour analyser une nouvelle sequence" << endl;
        cin >> loopSeq;
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
