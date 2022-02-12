#pragma once
#include <string>

using namespace std;

namespace PatternSearch
{
	class LAM
	{
		public:
		//Processors
		double max4(double a, double b, double c, double d);
		double score(int pos, int lettre, double* arr, int nCol);
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