#pragma once
#include <string>

using namespace std;

namespace PatternSearch
{
	class LAM
	{
		public:
		//Setters
		void Set(double v, int col, int row);
		double Get(int col, int row);

		//Processors
		void Setup(double* arr, int nCol);
		void DisplayTable();

		//Constructors
		LAM();
		LAM(double* DPWM, int nCol);

		private:

		double* Tab;
		int nRow;
		int nCol;
	};
}