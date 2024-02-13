//#define _GLIBCXX_USE_CXX11_ABI 0

#include <iostream>
#include <cstdlib>
#include "src/manager/jobManager.h"

using namespace std;

int main(int argc, char* argv[]){
    cout << " " << endl;
    cout << "[INFO] TMM model started ..." << endl;

    try{
        if (argc <= 1){
            throw std::string("[Error] No input file (input.yaml) is not given..");
        }
		std::string inputfile = argv[1];
        cout << "[INFO] Input file: " << inputfile << endl;
        JobManager JOB(inputfile);
        JOB.init();
        JOB.start();
        JOB.makeReport();
    }
    catch(std::string error){
        // TEMP (te be deleted)
		std::string inputfile = "input.yaml";
        cout << "[INFO] Input file2: " << inputfile << endl;
        JobManager JOB(inputfile);
        JOB.init();
        JOB.start();
        JOB.makeReport();
        // upto here
        cout << error << endl;
    }

    cout << "[INFO] End of simulation ...";
    cout << " " << endl;
    return EXIT_SUCCESS;
}
