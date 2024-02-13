#include "tmmRun.h" 
//#include <omp.h> 
#include <vector> 
#include <cmath>
#include <complex>
#include <filesystem>
#include <sstream>

using namespace arma; 
//using namespace std;
namespace fs = std::filesystem;

#define PI 3.1415926535897

//auto TMM::init(const YAML::Node& in)->void
auto TMM::init()->bool
{
	cout << "TMM::init() " << endl;
	// Load light and make optic parameters
	double wave_s = m_inputs.wavelength[0];
	double wave_f = m_inputs.wavelength[1];
	double dwave = m_inputs.wavelength[2];
	double angle_s = m_inputs.polar_angles[0];
	double angle_f = m_inputs.polar_angles[1];
	double dangle = m_inputs.polar_angles[2];
	int wave_n = (wave_f - wave_s) / dwave;
	int angle_n = (angle_f - angle_s) / dangle;
	//m_waves.resize(wave_n+1);
	m_waves = linspace<vec>(wave_s, wave_f, (wave_n+1));
	m_angles.resize(angle_n+1);
	//for (int i = 0; i < wave_n+1; ++i){
	//	m_waves[i] = wave_s + dwave * i;
	//}
	for (int i = 0; i < angle_n+1; ++i){
		m_angles[i] = angle_s + dangle * i;
	}

	// Open NK lib folder and make dictionary
	map<std::string, fs::path> material;
	fs::path cur_path = fs::current_path();
	std::string path = cur_path.generic_string() + "/" + m_inputs.nklib;
	for (const auto & files : fs::directory_iterator(path)){
		fs::path file = files.path(); 
		std::string filename = file.stem().generic_string();
		material[filename] = file;
	}

	std::string aaa = "aaa";

	//Check and Update NK lib
	cout << "NK Lib: " << m_inputs.nklib << endl;
	set<std::string> mat_set;
	for (auto mat : m_inputs.stack){
		cout << "Mat: " << mat << endl;
		std::string mmat = mat;
		if(mat_set.find(mmat) != mat_set.end()) continue;
		mat_set.insert(mmat);
		// Check if Mat is in the list
		if (material.find(mat) == material.end()){
			cout << "[Error] No Material is found in the nklib folder.. MAT: " << mat << endl;
			return false;
		}
		else{
			// Load NK data
			cout << "Load NK values.. MAT: " << mat << endl;
			vector<double> w0;
			vector<double> n0;
			vector<double> k0;
			ifstream myfile(material[mat]);
			std::string str;
			if (myfile.is_open() ) {
				int idx = 0;
				while(myfile){
					getline(myfile, str);
					if (idx>0){
						auto mat_v = stringSplit(str); 
						w0.push_back(mat_v[0]);
						n0.push_back(mat_v[1]);
						k0.push_back(mat_v[2]);
					}
					++idx;
				}
			}
			// Interpolation
			vec vec_w0(w0);
			vec vec_n0(n0);
			vec vec_k0(k0);
			interp1(vec_w0, vec_n0, m_waves, m_n[mat], "linear");
			interp1(vec_w0, vec_k0, m_waves, m_k[mat], "linear");
			//m_n[mat].print("Interp1 n1: ");
			//m_k[mat].print("Interp1 k1: ");
		}
	}

	return true;
}


auto TMM::stringSplit(std::string& str)->vector<double> 
{
	stringstream ss(str);
	std::string val;
	vector<double> res(3);
	int idx2 = 0;
	while(getline(ss, val, '\t')){
		double num = stod(val);
		res[idx2] = num;
		++idx2;
	}
    return res;
}

auto TMM::start()->void
{
    cout << "start() called over wavelength" << endl;
	for (int i = 0; i < m_waves.size(); ++i){
		for(int j = 0; j < m_angles.size(); ++j){
			transferMatrix(i, j);	
		}
	}
}

//auto TMM::transferMatrix(double& wave, double& angle)->void
auto TMM::transferMatrix(int& idx1, int& idx2)->void
{
	double wave = m_waves[idx1];
	double angle = m_angles[idx2];
	int num_layers = m_inputs.stack.size();
	//cout << "num of layers: " << m_inputs.stack.size() << endl;
	double ang_inc_0 = PI * angle / 180.0;
	double N0 = 1.0;
	double d0 = 1.0;
	complex<double> J(0, 1);

	// Air Layer
	double y = 2.6544 * 1e-3;

	cx_mat T_mat_s_t(2, 2, fill::zeros);
	cx_mat T_mat_p_t(2, 2, fill::zeros);
	T_mat_s_t(0, 0) = 1.0 + 0j;
	T_mat_s_t(0, 1) = 0.0 + 0j;
	T_mat_s_t(1, 0) = 0.0 + 0j;
	T_mat_s_t(1, 1) = 1.0 + 0j;
	T_mat_p_t(0, 0) = 1.0 + 0j;
	T_mat_p_t(0, 1) = 0.0 + 0j;
	T_mat_p_t(1, 0) = 0.0 + 0j;
	T_mat_p_t(1, 1) = 1.0 + 0j;
	complex<double> eta_s_air = y * 1.0 * cos(ang_inc_0); 
	complex<double> eta_p_air = y * 1.0 / cos(ang_inc_0); 
	complex<double> eta_s_sub = y * 1.0 * cos(ang_inc_0); 
	complex<double> eta_p_sub = y * 1.0 / cos(ang_inc_0); 
	int tmp = 0;
	for (auto mat : m_inputs.stack){
	//for (int i = 0; i < num_layers; ++i){
		cx_mat T_mat_s_i(2, 2, fill::zeros);
		cx_mat T_mat_p_i(2, 2, fill::zeros);
		double di = m_inputs.thick[tmp];
		//double ni = m_inputs.n[i];
		//double ki = m_inputs.k[i] * -1;
		double ni = m_n[mat][idx1];
		double ki = m_k[mat][idx1] * -1;
		complex<double> Ni(ni, ki);

		complex<double> ang_inc_i = asin(1.0 * ang_inc_0 / Ni);
		complex<double> theta_c = 2.0 * PI * Ni * di * cos(ang_inc_i) / (double)wave;

		complex<double> eta_s = y * Ni * cos(ang_inc_i); 
		complex<double> eta_p = y * Ni / cos(ang_inc_i); 
		eta_s_sub = eta_s;
		eta_p_sub = eta_p;
		T_mat_s_i(0, 0) = cos(theta_c);
		T_mat_s_i(0, 1) = J * (sin(theta_c)) / eta_s;
		T_mat_s_i(1, 0) = J * (sin(theta_c)) * eta_s;
		T_mat_s_i(1, 1) = cos(theta_c);
		T_mat_p_i(0, 0) = cos(theta_c);
		T_mat_p_i(0, 1) = J * (sin(theta_c)) / eta_p;
		T_mat_p_i(1, 0) = J * (sin(theta_c)) * eta_p;
		T_mat_p_i(1, 1) = cos(theta_c);
		T_mat_s_t = T_mat_s_t * T_mat_s_i;
		T_mat_p_t = T_mat_p_t * T_mat_p_i;
		++tmp;
	}
	cx_vec column_s = {1, eta_s_sub};
	cx_vec column_p = {1, eta_p_sub};
	cx_vec BC_s = T_mat_s_t * column_s;
	cx_vec BC_p = T_mat_p_t * column_p;
	complex<double> B_s =BC_s(0); 
	complex<double> C_s =BC_s(1); 
	complex<double> B_p =BC_p(0); 
	complex<double> C_p =BC_p(1); 
	complex<double> R_s_0 = (eta_s_air * B_s - C_s) / (eta_s_air * B_s + C_s);
	complex<double> R_p_0 = (eta_p_air * B_p - C_p) / (eta_p_air * B_p + C_p);
	double R_s = pow(abs(R_s_0), 2);
	double R_p = pow(abs(R_p_0), 2);
	double R_tot = R_s * m_inputs.pol + R_p * (1 - m_inputs.pol);
	//cout << "R_s: " << R_s << endl;
	//cout << "R_p: " << R_p << endl;
	cout << "R_tot: " << R_s << " " << R_p << " " << R_tot << endl;
	
}

auto TMM::saveResult()->void
{
    cout << "saveResult() called" << endl;

}

auto TMM::printPrivate()->void
{
	cout << "TMM::printPrivate: "  << endl;
}

TMM::TMM(){
}


TMM::~TMM(){
}

/*  Armadillo examples

    cx_mat A(5, 4, fill::zeros);
    cout << A.n_rows << endl;
    A.print("A:");
    mat B = abs(A);
    B.print();
    mat A1(10, 10, fill::randu);
    cx_vec eigval;
    cx_mat eigvec;
    eig_gen(eigval, eigvec, A1);
    //eig_gen(eigval, eigvec, A, "balance");
    eigval.print();
    eigvec.print();
    cx_mat Y = fft2(eigvec);
    Y.print("FFT result:");
*/



