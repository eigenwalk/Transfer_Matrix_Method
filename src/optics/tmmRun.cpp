#include <armadillo> 
#include <omp.h> 
#include <string> 
#include <vector> 
#include <cmath>
#include <complex>
#include "tmmRun.h" 

using namespace arma; 
using namespace std;
#define PI 3.141592

//auto TMM::init(const YAML::Node& in)->void
auto TMM::init()->void
{
	cout << "TMM::init() " << endl;
	cout << "wavelength: " << m_inputs.wavelength[0] << endl;
	// Load light and make optic parameters

	float wave_s = m_inputs.wavelength[0];
	float wave_f = m_inputs.wavelength[1];
	float dwave = m_inputs.wavelength[2];
	int wave_n = (wave_f - wave_s) / dwave;
	m_waves.resize(wave_n+1);
	for (int i = 0; i < wave_n+1; ++i){
		m_waves[i] = wave_s + dwave * i;
	}
}

auto TMM::start()->void
{
    cout << "start() called over wavelength" << endl;
	for (auto wave: m_waves){
		transferMatrix(wave);	
	}
}

auto TMM::transferMatrix(float& wave)->void
{
	int num_layers = m_inputs.stack.size();
	cout << "num of layers: " << m_inputs.stack.size() << endl;
	double ang_inc_0 = PI * m_inputs.polar_ang / 180.0;
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
	complex<double> eta_s_air = y * 1.0 * cos(PI * ang_inc_0 / 180.0); 
	complex<double> eta_p_air = y * 1.0 / cos(PI * ang_inc_0 / 180.0); 
	complex<double> eta_s_sub = y * 1.0 * cos(PI * ang_inc_0 / 180.0); 
	complex<double> eta_p_sub = y * 1.0 / cos(PI * ang_inc_0 / 180.0); 
	for (int i = 0; i < num_layers; ++i){
		cx_mat T_mat_s_i(2, 2, fill::zeros);
		cx_mat T_mat_p_i(2, 2, fill::zeros);
		double di = m_inputs.thick[i];
		double ni = m_inputs.n[i];
		double ki = m_inputs.k[i] * -1;
		complex<double> Ni(ni, ki);
		complex<double> ang_inc_i = asin(1.0 * ang_inc_0 / Ni);
		complex<double> theta_c = 2.0 * PI * Ni * di * cos(ang_inc_i) / (double)wave;

		complex<double> eta_s = y * Ni * cos(PI * ang_inc_i / 180.0); 
		complex<double> eta_p = y * Ni / cos(PI * ang_inc_i / 180.0); 
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
	cout << "R_s: " << R_s << endl;
	cout << "R_p: " << R_p << endl;
	cout << "R_tot: " << 0.5 * R_p + 0.5 * R_s << endl;
	
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



