#include "tmmRun.h" 
#include <omp.h> 
#include <vector> 
#include <cmath>
#include <complex>
#include <sstream>
#include <fstream>

//using namespace arma; 
//using namespace std;

#define PI 3.1415926535897
#define y 2.6544 * 1e-3

	
//auto TMM::init(const YAML::Node& in)->void
auto TMM::init()->bool
{
	// Load light and make optic parameters
	double wave_s = m_inputs.wavelength[0];
	double wave_f = m_inputs.wavelength[1];
	double dwave = m_inputs.wavelength[2];
	double angle_s = m_inputs.polar_angles[0];
	double angle_f = m_inputs.polar_angles[1];
	double dangle = m_inputs.polar_angles[2];
	int wave_n = (wave_f - wave_s) / dwave;
	int angle_n = (angle_f - angle_s) / dangle;
	m_waves = linspace<vec>(wave_s, wave_f, (wave_n+1));
	m_angles = linspace<vec>(angle_s, angle_f, (angle_n+1));

	// Open NK lib folder and make dictionary
	//map<std::string, fs::path> material;
	fs::path cur_path = fs::current_path();
	std::string path = cur_path.generic_string() + "/" + m_inputs.nklib;
	for (const auto & files : fs::directory_iterator(path)){
		fs::path file = files.path(); 
		std::string filename = file.stem().generic_string();
		m_material[filename] = file;
	}

	//Check and Update NK lib
	cout << "NK Lib: " << m_inputs.nklib << endl;
	set<std::string> mat_set;
	// For substrate 
	mat_set.insert(m_inputs.substrate);
	loadNK(m_inputs.substrate);

	// For Stacks
	for (auto mat : m_inputs.stack){
		cout << "Mat: " << mat << endl;
		std::string mmat = mat;
		if(mat_set.find(mmat) != mat_set.end()) continue;
		mat_set.insert(mmat);
		// Check if Mat is in the list
		if (m_material.find(mat) == m_material.end()){
			cout << "[Error] No Material is found in the nklib folder.. MAT: " << mat << endl;
			return false;
		}
		else{
			// Load NK data
			cout << "[INFO] Loadng NK values for material: " << mat << endl;
			loadNK(mat);
		}
	}

	return true;
}

auto TMM::loadNK(std::string& mat)->void 
{
	vector<double> w0(5000);
	vector<double> n0(5000);
	vector<double> k0(5000);
	ifstream myfile(m_material[mat]);
	std::string str;
	int idx = 0;
	if (myfile.is_open() ) {
		while(myfile){
			getline(myfile, str);
			if (idx>0){
				auto mat_v = stringSplit(str); 
				w0[idx-1] = mat_v[0];
				n0[idx-1] = mat_v[1];
				k0[idx-1] = mat_v[2];
			}
			++idx;
		}
	}
	w0.resize(idx);
	n0.resize(idx);
	k0.resize(idx);

	// Interpolation
	vec vec_w0(w0);
	vec vec_n0(n0);
	vec vec_k0(k0);
	interp1(vec_w0, vec_n0, m_waves, m_n[mat], "linear");
	interp1(vec_w0, vec_k0, m_waves, m_k[mat], "linear");

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
    cout << "[INFO] TMM started over wavelength" << endl;
	for(int j = 0; j < m_angles.size(); ++j){
		cout << " " << endl;
		cout << "Angles: " << m_angles[j] << endl;

		// For R and T
		double ang = m_angles[j];
		m_Rs[ang].resize(m_waves.size());
		m_Rp[ang].resize(m_waves.size());
		m_R[ang].resize(m_waves.size());
		m_Ts[ang].resize(m_waves.size());
		m_Tp[ang].resize(m_waves.size());
		m_T[ang].resize(m_waves.size());
		m_As[ang].resize(m_waves.size());
		m_Ap[ang].resize(m_waves.size());
		m_A[ang].resize(m_waves.size());

		//cout << "Wave[nm] Rs Rp R(total) Ts Tp T(Total) As Ap A(Total)" << endl;
		//#pragma omp parallel for
		for (int i = 0; i < m_waves.size(); ++i){
			transferMatrix(i, j);	
		}
	}
}

auto TMM::getTMat(cx_mat& T_mat, complex<double>& Ni, double& di, complex<double>& angle_i, double& wave, string pol)->void
{
	complex<double> J(0, 1);
	complex<double> theta_c = 2.0 * PI * Ni * di * cos(angle_i) / (double)wave;
	complex<double> eta; 
	if(pol == "s") eta = y * Ni * cos(angle_i);
	else eta = y * Ni / cos(angle_i);
		 
	T_mat(0, 0) = cos(theta_c);
	T_mat(0, 1) = J * (sin(theta_c)) / eta;
	T_mat(1, 0) = J * (sin(theta_c)) * eta;
	T_mat(1, 1) = cos(theta_c);
}

auto TMM::transferMatrix(int& idx1, int& idx2)->void
{
	double wave = m_waves[idx1];
	double angle = m_angles[idx2];
	int num_layers = m_inputs.stack.size();

	complex<double> ang_inc_0(PI * angle / 180.0, 0.0);
	complex<double> J(0, 1);

	// Air Layer
	cx_mat T_mat_s_t(2, 2, fill::zeros);
	cx_mat T_mat_p_t(2, 2, fill::zeros);

	double d0 = 0;
	complex<double> N0(1.0, 0.0);
	getTMat(T_mat_s_t, N0, d0, ang_inc_0, wave, "spol");
	getTMat(T_mat_p_t, N0, d0, ang_inc_0, wave, "ppol");


	// Multi-Layers
	int tmp = 0;
	for (auto mat : m_inputs.stack){
		cx_mat T_mat_s_i(2, 2, fill::zeros);
		cx_mat T_mat_p_i(2, 2, fill::zeros);
		double di = m_inputs.thick[tmp];
		double ni = m_n[mat][idx1];
		double ki = m_k[mat][idx1] * -1;
		complex<double> Ni(ni, ki);

		complex<double> ang_inc_i = asin(1.0 * ang_inc_0 / Ni);
		getTMat(T_mat_s_i, Ni, di, ang_inc_i, wave, "s");
		getTMat(T_mat_p_i, Ni, di, ang_inc_i, wave, "p");

		// Matrix mul
		T_mat_s_t = T_mat_s_t * T_mat_s_i;
		T_mat_p_t = T_mat_p_t * T_mat_p_i;
		++tmp;
	}

	// Substrate
	double nsub = m_n[m_inputs.substrate][idx1];
	double ksub = m_k[m_inputs.substrate][idx1] * -1;
	complex<double> Nsub(nsub, ksub);
	complex<double> ang_inc_sub = asin(1.0 * ang_inc_0 / Nsub);

	complex<double> eta_s_air = y * 1.0 * cos(ang_inc_0); 
	complex<double> eta_p_air = y * 1.0 / cos(ang_inc_0); 
	complex<double> eta_s_sub = y * Nsub * cos(ang_inc_sub); 
	complex<double> eta_p_sub = y * Nsub / cos(ang_inc_sub); 

	cx_vec column_s = {1, eta_s_sub};
	cx_vec column_p = {1, eta_p_sub};
	cx_vec BC_s = T_mat_s_t * column_s;
	cx_vec BC_p = T_mat_p_t * column_p;
	vector<complex<double>> BC_sp(4);
	BC_sp[0] = BC_s(0); 
	BC_sp[1] = BC_s(1); 
	BC_sp[2] = BC_p(0); 
	BC_sp[3] = BC_p(1); 

	calcReflection(BC_sp, eta_s_air, eta_p_air, angle, idx1);
	calcTransmission(BC_sp, eta_s_air, eta_p_air, eta_s_sub, eta_p_sub, angle, idx1);
	calcAbsorption(BC_sp, eta_s_air, eta_p_air, eta_s_sub, eta_p_sub, angle, idx1);
	//cout << wave << " " << m_Rs[angle][idx1] << " " << m_Rp[angle][idx1] << " " << m_R[angle][idx1] << " " << m_Ts[angle][idx1] << " " << m_Tp[angle][idx1] << " " << m_T[angle][idx1] << " " << m_As[angle][idx1] << " " << m_Ap[angle][idx1] << " " << m_A[angle][idx1] << endl;
}

auto TMM::calcReflection(vector<complex<double>>& BC_sp, complex<double>& eta_s_air, complex<double>& eta_p_air, double& ang, int& idx)->void
{
	complex<double> R_s_0 = (eta_s_air * BC_sp[0] - BC_sp[1]) / (eta_s_air * BC_sp[0] + BC_sp[1]);
	complex<double> R_p_0 = (eta_p_air * BC_sp[2] - BC_sp[3]) / (eta_p_air * BC_sp[2] + BC_sp[3]);
	double R_s = pow(abs(R_s_0), 2);
	double R_p = pow(abs(R_p_0), 2);
	double R_tot = R_s * m_inputs.pol + R_p * (1 - m_inputs.pol);
	m_Rs[ang][idx] = R_s;
	m_Rp[ang][idx] = R_p;
	m_R[ang][idx] = R_tot;
}
auto TMM::calcTransmission(vector<complex<double>>& BC_sp, complex<double>& eta_s_air, complex<double>& eta_p_air, complex<double>& eta_s_sub, complex<double>& eta_p_sub, double& ang, int& idx)->void
{
	double T_s = (4.0 * eta_s_air.real() * eta_s_sub.real()) / pow(abs(eta_s_air * BC_sp[0] + BC_sp[1]), 2);
	double T_p = (4.0 * eta_p_air.real() * eta_p_sub.real()) / pow(abs(eta_p_air * BC_sp[2] + BC_sp[3]), 2);
	double T_tot = T_s * m_inputs.pol + T_p * (1 - m_inputs.pol);
	m_Ts[ang][idx] = T_s;
	m_Tp[ang][idx] = T_p;
	m_T[ang][idx] = T_tot;
}

auto TMM::calcAbsorption(vector<complex<double>>& BC_sp, complex<double>& eta_s_air, complex<double>& eta_p_air, complex<double>& eta_s_sub, complex<double>& eta_p_sub, double& ang, int& idx)->void
{
	complex<double> tmp_s = (BC_sp[0] * conj(BC_sp[1]) - eta_s_sub);
	complex<double> tmp_p = (BC_sp[2] * conj(BC_sp[3]) - eta_p_sub);
	double A_s = (4.0 * eta_s_air.real() * tmp_s.real()) / pow(abs(eta_s_air * BC_sp[0] + BC_sp[1]), 2);
	double A_p = (4.0 * eta_p_air.real() * tmp_p.real()) / pow(abs(eta_p_air * BC_sp[2] + BC_sp[3]), 2);
	double A_tot = A_s * m_inputs.pol + A_p * (1 - m_inputs.pol);
	m_As[ang][idx] = A_s;
	m_Ap[ang][idx] = A_p;
	m_A[ang][idx] = A_tot;
}

auto TMM::saveResult()->void
{
	for(int j = 0; j < m_angles.size(); ++j){
		string ang_s = to_string(m_angles[j]);
		ang_s = ang_s.substr(0, ang_s.find('.') + 3);
		string file = "TRA_result_angle_" + ang_s + ".txt";
		fs::path cur_path = fs::current_path();
		string path = cur_path.generic_string() + "/results/";
		if (!fs::is_directory(path)) fs::create_directory(path);
		string outfile = path + file; 

		cout << "[INFO] Saving results.. file: " << file << endl;

		// For R and T
		double ang = m_angles[j];
		//m_Rs[ang];
		ofstream newFile;
		newFile.open(outfile);

		newFile << "wavelength[nm]" << '\t' <<  "Rs" << '\t' << "Rp"  << '\t' << "R" << '\t' <<  "Ts" << '\t' <<  "Tp" << '\t' <<  "T"  << '\t' <<  "As" << '\t' <<  "Ap" << '\t' <<  "A" << '\n';
		int idx = 0;
		for (auto wave : m_waves){
			newFile << wave << "\t" << m_Rs[ang][idx] << "\t"  << m_Rp[ang][idx] << "\t" << m_R[ang][idx] << "\t" << m_Ts[ang][idx] << "\t" << m_Tp[ang][idx] << "\t" << m_T[ang][idx] << "\t" << m_As[ang][idx] << "\t" << m_Ap[ang][idx] << "\t" << m_A[ang][idx] << '\n'; 
			++idx;
		} 
		newFile.close();
	}

}


TMM::TMM(){
}

TMM::~TMM(){
}

