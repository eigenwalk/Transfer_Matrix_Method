#ifndef TMM_RUN_H
#define TMM_RUN_H

#include <iostream>
#include <math.h>
#include <string>
#include <yaml-cpp/yaml.h>
//#include "manager/jobManager.h"
#include "structures/inputStructure.h"

using namespace std;

class TMM
{
    private:

    public:
		TMM();
		TMM(Inputs inpts):m_inputs(inpts){};
		~TMM();
        //auto init(const YAML::Node& in)->void;
        auto init()->void;
        auto start()->void;
        auto saveResult()->void;
        auto printPrivate()->void;

	public:
		Inputs m_inputs;
		//int m_int;
};

#endif
