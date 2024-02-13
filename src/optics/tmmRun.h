#ifndef TMM_RUN_H
#define TMM_RUN_H

#include <armadillo> 
#include <iostream>
#include <math.h>
#include <string>
#include <map>
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
        auto init()->bool;
        auto start()->void;
        auto saveResult()->void;
        auto printPrivate()->void;
		//auto transferMatrix(double& wave, double& angle)->void;
		auto transferMatrix(int& idx1, int& idx2)->void;
		auto stringSplit(std::string& str)->vector<double>;

	public:
		Inputs m_inputs;
		//vector<double> m_waves;
		arma::vec m_waves;
		std::map<std::string, arma::vec> m_n;
		std::map<std::string, arma::vec> m_k;
		vector<double> m_angles;
		
		float m_unit;

		//int m_int;
};

#endif
