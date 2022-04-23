#pragma once
#include <string>

using namespace std;

namespace PatternSearch
{
	class LAM
	{
		public:
		//Setters
		void Set(double* toFill,double v, int col, int row);
		double Get(double* arr, int col, int row);
		double GetLeft(int col, int row);
		double GetRight(int col, int row);

		double MaxLeftOf(char c, int pos);
		double MaxRightOf(char c, int pos);
		double GetMaxValue();
		double GetMinValue();

		//Processors
		void Setup(double* toFill, double* arr, int nCol, bool maxOrMin);

		void DisplayLeftTable();
		void DisplayRightTable();
		void DisplayTable(double* arr);

		void FlipTable(double* arrFrom, double* arrTo, int nCol);

		//Constructors
		LAM();
		LAM(double* DPWM, int nCol, int nRow);

		private:

		double* tabLeft;
		double* tabRight;

		int nRow;
		int nCol;
		double maxValue;
		double minValue;
	};
}