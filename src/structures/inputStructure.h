#ifndef INPUTS_H
#define INPUTS_H

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <string>
#include <vector>
#include <set>

using namespace std;

//struct Mater{
struct Inputs{
	vector<string> stack;
	vector<float> thick;
	string superstrate;
	string substrate;
	string nklib;
	vector<float> wavelength;
	vector<float> polar_angles;
	vector<float> n;
	vector<float> k;
	int num;
	//float polar_ang;
	//float azi_ang;
	float pol;
	float dop;
};

#endif
