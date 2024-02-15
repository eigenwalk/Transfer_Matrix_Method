#ifndef INPUTS_H
#define INPUTS_H

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <string>
#include <vector>
#include <set>

using namespace std;

struct Inputs{
	vector<string> stack;
	vector<float> thick;
	string substrate;
	string nklib;
	vector<float> wavelength;
	vector<float> polar_angles;
	int num;
	float pol;
	float dop;
};

#endif
