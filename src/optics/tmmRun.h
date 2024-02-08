#ifndef TMM_RUN_H
#define TMM_RUN_H

#include <iostream>
#include <math.h>
#include <string>
#include <yaml-cpp/yaml.h>
//#include "manager/jobManager.h"
#include "structures/material.h"

using namespace std;

class TMM
{
    private:

    public:
		TMM();
		TMM(Mater mat):m_mat(mat){};
		~TMM();
        //auto init(const YAML::Node& in)->void;
        auto init()->void;
        auto start()->void;
        auto saveResult()->void;
        auto printPrivate()->void;

	public:
		Mater m_mat;
		//int m_int;
};

#endif
