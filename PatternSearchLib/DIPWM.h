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

		//Constructors
		DIPWM();
		DIPWM(string text);

	private:

		string id;
		string text;
		LAM lam;
		LAT lat;

	};
}