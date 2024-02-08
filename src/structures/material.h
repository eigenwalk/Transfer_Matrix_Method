#ifndef Material_H
#define Material_H

#include <string>
#include <vector>

using namespace std;

struct Mater{
	vector<string> stack;
	vector<float> thick;
	vector<float> n;
	vector<float> k;
	int num;
};
//}m_mater;

#endif
