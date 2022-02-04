#pragma once

#include <string>

using namespace std;

namespace PatternSearch
{
	class LAT
	{

		public:

		//Accessors

		//Processors
		void Setup(double* arr, int nCol, int nRow);
		void DisplayTable();

		//Constructors
		LAT();
		LAT(double* arr, int nCol, int nRow);

		private:

		double* Tab;
		int nCol;
	};
}