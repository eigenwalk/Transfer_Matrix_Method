#include <armadillo>
#include <omp.h>
#include <string>
#include <vector>
#include "tmmRun.h"

using namespace arma;
using namespace std;

//auto TMM::init(const YAML::Node& in)->void
auto TMM::init()->void
{
	cout << "TMM::init() " << endl;
	cout << "wavelength: " << m_inputs.wavelength[0] << endl;
	// Make T-Mat
	// Load light and make optic parameters
	//  
}


auto TMM::start()->void
{
    cout << "start() called" << endl;
	cout << m_inputs.n[1] << " " << m_inputs.n[3] << endl;
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



