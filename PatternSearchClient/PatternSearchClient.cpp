
// PatternSearchClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if defined(_WIN64_) || defined(_WIN32_) || defined(WIN64) || defined(WIN32) || defined(_WIN64) || defined(_WIN32)
#define IS_WINDOWS 1
#else
#define IS_WINDOWS 0
#endif

#if IS_WINDOWS
#include <Windows.h>
#include "../PatternSearchLib/DIPWM.h"
#include "../PatternSearchLib/LAT.h"
#include "../PatternSearchLib/LAM.h"
#else
#include "../PatternSearchLib/sdsl-source/include/sdsl/suffix_arrays.hpp"
#include "../PatternSearchLib/DIPWM.h"
#include "../PatternSearchLib/LAT.h"
#include "../PatternSearchLib/LAM.h"
#include "../PatternSearchLib/SDSL.h"
#include <libgen.h>       // dirname
#include <unistd.h>       // readlink
#include <linux/limits.h> // PATH_MAX
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
    if (count != -1)
    {
        path = dirname(result);
        return path;
    }
    else
    {
        return NULL;
    }
#endif
}

int main(int argc, char *args)
{
    cout << "\n\nStart\n\n";

    START:

    cout << "\n\n Entrez le fichier contenant la DIPWM a analyser : " << endl;
    string DIPWMFILE;
    cin >> DIPWMFILE;
    string file = GetCurrentDirectory() + "/" + DIPWMFILE;

    ifstream fichierD(file);
    if (!fichierD.good())
    {
        cout << "  ||>> Fichier introuvable " << endl;
        goto START;
    }

    // string file = "D:/Documents/Etudes/FAC/L3/TER/PatternSearch/Debug/motif_HUMAN.H11DI.0.A.dpwm";

    // string file = GetCurrentDirectory() + "\\" + "test.dpwm";

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
    char loop = 'y';
    while (loop)
    {
        cout << "\n\nEntrez un seuil (% du maximum):  ";

        double seuil;
        cin >> seuil;
        cout << endl;

        cout << "Entrez <f> pour enumerer les mots complets, <c> pour les mots du coeur" << endl;

        char mode;
        cin >> mode;
        cout << endl;

        if (mode == 'f')
        {
            seuil = motif.EnumerateFullWords(seuil, GetCurrentDirectory(), true);
            isCore = false;
        }
        else if (mode == 'c')
        {
            seuil = motif.EnumerateCoreWords(seuil, GetCurrentDirectory());
            isCore = true;
        }
        else
        {
            cout << "  >> Mauvaise entree" << endl;
            continue;
        }

        if (motif.WordCount() <= 0)
        {
            cout << "  >> Aucun mot genere !" << endl;
            continue;
        }

        motif.DisplayWords(10, isCore);

        cout << "\nEcrire un fichier de mots ? <y> ou <n>" << endl;
        char write;
        cin >> write;

        if (write == 'y')
        {
            motif.WriteWordsFile(motif.UsedSeuil(), GetCurrentDirectory(), isCore);
        }

        char loopSeq = 'y';
        while (loopSeq == 'y')
        {
            cout << "\n\nEntrez le fichier contenant la sequence a analyser : ";
            string sequenceFile;
            cin >> sequenceFile;
            string sequenceName = sequenceFile;
            sequenceFile = GetCurrentDirectory() + "/" + sequenceFile;

        LINECHOICE:

            int debLine, finLine;
            cout << "  ||>> Entrez la premiere ligne a analyser : ";
            cin >> debLine;
            cout << "  ||>> Entrez la derniere ligne a analyser : ";
            cin >> finLine;
            cout << endl;

            cout << "Lecture des lignes [" << debLine << "-" << finLine << "] de la sequence dans : " << sequenceFile << endl;

            ifstream fichier(sequenceFile);
            if (!fichier.good())
            {
                cout << "  ||>> Fichier introuvable " << endl;
                cin.clear();
                continue;
            }

            vector<string> sequenceLines;
            int sequenceSize = 0;
            string header;
            string line;
            int lineId = 0;
            while (getline(fichier, line) && lineId < finLine)
            {
                if (lineId == 0)
                {
                    header = line;
                }
                else if (lineId >= debLine)
                {
                    sequenceLines.push_back(line);
                    sequenceSize += line.size();
                }
                lineId++;
            }

            cout << "  ||>> header : " << header << endl;
            cout << "  ||>> " << sequenceLines.size() << " lignes a analyser" << endl;
            if (sequenceLines.size() >= 4)
            {
                for (int i = 0; i < 4; i++)
                {
                    int id = min(sequenceLines.size() - 1, (sequenceLines.size() / 3) * i);
                    if (i == 2)
                    {
                        cout << "    ||>> ... ... ..." << endl;
                    }
                    else
                    {
                        cout << "    ||>> Ligne " << id + debLine << " : " << sequenceLines.at(id).substr(0, 30) << " ... " << sequenceLines.at(id).size() << endl;
                    }
                }
            }
            else
            {
                cout << "    ||>> Ligne 0 : " << sequenceLines.at(0).substr(0, 30) << " ... " << sequenceLines.at(0).size() << endl;
            }

            char c;
            cout << "  ||>> Entrez <y> pour continuer, <n> pour changer de lignes" << endl;
            cin >> c;

            if (c != 'y')
            {
                goto LINECHOICE;
            }

            cout << "  ||>> Generation d'une chaine contenant les lignes [" << debLine << "-" << finLine << "]" << endl;

            char *sequenceArr = new char[sequenceSize];
            int sid = 0;
            for (int i = 0; i < sequenceLines.size(); i++)
            {
                for (int o = 0; o < sequenceLines.at(i).size(); o++)
                {
                    sequenceArr[sid] = sequenceLines.at(i)[o];
                    sid++;
                }
            }
            string sequence = string(sequenceArr);

            cout << "    ||>>  Taille de la chaine : " << sequence.size() << " > " << sequence.substr(0, 50) << " ... " << endl;

        LABELMETHODE:

            cout << "  ||>> Selectionner une méthode a utiliser : <a> pour Aho_corasik, <s> pour SDSL :";
            char methode; cin >> methode; cout << endl;

            if (methode == 'a')
            {
                cout << "  ||>> Analyse de la chaine avec aho-corasick" << endl;

                vector<SearchResult> results = motif.Search(sequence, isCore); // results.insert(results.end(), r.begin(), r.end());

                cout << "    ||>> " << results.size() << " resultats" << endl;

                for (int i = 0; i < results.size(); i++)
                {
                    SearchResult r = results.at(i);
                    cout << "      > " << r.start << "-" << r.end << " >> " << r.str << endl;
                }

                cout << "\nEcrire un fichier de resultats ? <y> ou <n>" << endl;
                write;
                cin >> write;
                if (write == 'y')
                {
                    cout << "  ||>> Entrez le nom du fichier de resultats : "; 
                    string rName;
                    cin >> rName; cout << endl;
                    motif.WritesFinalSequenceWordsFile(results, GetCurrentDirectory(), rName);
                    cout << "  ||>> Fichier enregistre" << endl;
                }
            }
            else if (methode == 's')
            {
#if IS_WINDOWS
                cout << "SDSL n'est disponible que sur Linux" << endl;
                goto LABELMETHODE;
#else
                string fileOut;
                cout << "Saisir un nom de fichier en sortie" << endl;
                cin >> fileOut;
                string fileList = motif.FileName(motif.UsedSeuil(), isCore);
                SDSL::fm_index(sequenceName, fileList, fileOut);
#endif
            }
            else
            {
                goto LABELMETHODE;
            }

            cout << "\nEntrez <y> pour analyser une nouvelle sequence" << endl;

            cin >> loopSeq;
        }

        cout << "\nEntrez <y> pour recommencer avec un nouveau seuil" << endl;
        cin >> loop;
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
