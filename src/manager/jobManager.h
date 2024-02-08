#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include <iostream>
#include <math.h>
#include <string>
#include "optics/tmmRun.h"
#include "structures/inputStructure.h"

using namespace std;

class JobManager
{
    private:

    public:
        //JobManager(int id, string model){
        JobManager(string inputfile){
            m_inputfile = inputfile;
        }
		~JobManager();
        auto init()->void;
        auto start()->void;
        auto makeReport()->void;
        auto printModelInfo()->void;
		auto inputmanager(Inputs& inpts, const YAML::Node& in)->bool;
		auto structureUpdate(Inputs& inpts, const YAML::Node& in)->bool;
		auto opticsUpdate(Inputs& inpts, const YAML::Node& in)->void;
	public:
		//TMM* m_tmm{nullptr};
		TMM* m_tmm;
        string m_inputfile;
		
};

#endif

