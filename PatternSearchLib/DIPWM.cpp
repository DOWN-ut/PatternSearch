#include "DIPWM.h"
#include <string>
#include <iostream>
#include <fstream>
#include "LAM.h"
#include "LAT.h"

using namespace std;

namespace PatternSearch
{
	//Accessors
	string DIPWM::Id()
	{
		return this->id;
	}
	string DIPWM::GetText()
	{
		return this->text;
	}
	LAM DIPWM::Lam()
	{
		return this->lam;
	}
	LAT DIPWM::Lat()
	{
		return this->lat;
	}

	//


	//Constructors
	DIPWM::DIPWM(string filePath)
	{
		text = "";
		ifstream file;
		file.open(filePath);
		while(getline(file,text)){}
		file.close();
	}
}