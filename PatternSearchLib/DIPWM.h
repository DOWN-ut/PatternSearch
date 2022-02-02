#pragma once

#include <string>
#include "LAM.h"
#include "LAT.h"

using namespace std;

namespace PatternSearch
{
	class DIPWM
	{
	public:

		//Accessors
		string Id();
		string GetText();
		LAM Lam();
		LAT Lat();
		double Get(int x, int y);

		//Processors
		void Setup();

		//Prints
		void DisplayTable();

		//Constructors
		DIPWM();
		DIPWM(string file);

	private:

		string id;
		string text;

		double* arr;
		int nCol;int nRow;

		LAM lam;
		LAT lat;

	};
}