#ifndef TMM_RUN_H
#define TMM_RUN_H

#include <armadillo> 
#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include <filesystem>
//#include "manager/jobManager.h"
#include "structures/inputStructure.h"

namespace fs = std::filesystem;
using namespace std;
using namespace arma; 

class TMM
{
    private:

    public:
		TMM();
		TMM(Inputs inpts):
			m_inputs(inpts){};
		~TMM();
        //auto init(const YAML::Node& in)->void;
        auto init()->bool;
        auto start()->void;
        auto saveResult()->void;
		//auto transferMatrix(double& wave, double& angle)->void;
		auto transferMatrix(int& idx1, int& idx2)->void;
		auto stringSplit(string& str)->vector<double>;
		auto loadNK(string& mat)->void;
		auto getTMat(cx_mat& T_mat, complex<double>& Ni, double& di, complex<double>& angle_i, double& wave, string pol)->void;
		auto calcReflection(vector<complex<double>>& BC_sp, complex<double>& eta_s_air, complex<double>& eta_p_air, double& ang, int& idx)->void;
		auto calcTransmission(vector<complex<double>>& BC_sp, complex<double>& eta_s_air, complex<double>& eta_p_air, complex<double>& eta_s_sub, complex<double>& eta_p_sub, double& ang, int& idx)->void;
		auto calcAbsorption(vector<complex<double>>& BC_sp, complex<double>& eta_s_air, complex<double>& eta_p_air, complex<double>& eta_s_sub, complex<double>& eta_p_sub, double& ang, int& idx)->void;

	public:
		Inputs m_inputs;
		//vector<double> m_waves;
		//vector<double> m_angles;
		arma::vec m_waves;
		arma::vec m_angles;
		map<string, arma::vec> m_n;
		map<string, arma::vec> m_k;
		map<string, fs::path> m_material;
		map<double, vector<double>> m_Rs;
		map<double, vector<double>> m_Rp;
		map<double, vector<double>> m_R;
		map<double, vector<double>> m_Ts;
		map<double, vector<double>> m_Tp;
		map<double, vector<double>> m_T;
		map<double, vector<double>> m_As;
		map<double, vector<double>> m_Ap;
		map<double, vector<double>> m_A;
		
		float m_unit;
};

#endif
