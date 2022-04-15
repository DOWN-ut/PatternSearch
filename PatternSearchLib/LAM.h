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

		double MaxOf(char c, int pos);
		double GetMaxValue();
		double GetMinValue();

		//Processors
		void Setup(double* arr, int nCol, bool maxOrMin);
		void DisplayTable();

		//Constructors
		LAM();
		LAM(double* DPWM, int nCol);

		private:

		double* Tab;
		int nRow;
		int nCol;
		double maxValue;
		double minValue;

		double* diPinv; // à faire : intégrer au constructeur
	};
}