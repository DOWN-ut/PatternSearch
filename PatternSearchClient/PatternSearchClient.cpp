// PatternSearchClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "DIPWM.h"
#include "LAT.h"
#include "LAM.h"

using namespace std;
using namespace PatternSearch;

std::string GetCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

int main()
{
    cout << "\n\nC O U C O U  L E S  L O U L O U S\n\n";

    //string file = "D:/Documents/Etudes/FAC/L3/TER/PatternSearch/Debug/FOXP1_HUMAN.H11DI.0.A.dpwm";
    string file = GetCurrentDirectory() + "/FOXP1_HUMAN.H11DI.0.A.dpwm";

    cout << "\nLecture du fichier DIPWM : " << file << endl;

    DIPWM FOXP1 = DIPWM(file);

    cout << "\n\nContenu du tableau :" << endl;

    FOXP1.DisplayTable();
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
