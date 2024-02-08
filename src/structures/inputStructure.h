#ifndef INPUTS_H
#define INPUTS_H

#include <string>
#include <vector>

using namespace std;

//struct Mater{
struct Inputs{
	vector<string> stack;
	vector<float> thick;
	vector<float> n;
	vector<float> k;
	int num;
	vector<float> wavelength;
	float polar_ang;
	float azi_ang;
	float pol;
	float dop;
};

#endif
