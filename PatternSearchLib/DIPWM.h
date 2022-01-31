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

		//Processors


		//Constructors
		DIPWM();
		DIPWM(string file);

	private:

		string id;
		string text;
		LAM lam;
		LAT lat;

	};
}