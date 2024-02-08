#include <armadillo>
#include <omp.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include "jobManager.h"

//#include <yaml-cpp/depthguard.h>
//#include "yaml-cpp/parser.h"
//#include "yaml-cpp/exceptions.h"

using namespace arma;

auto JobManager::init()->void
{
	cout << "JobManager::init().." << endl;
    YAML::Node config = YAML::LoadFile(m_inputfile);
	Mater mater;
	if (inputmanager(mater, config["Configuration"])){
		m_tmm = new TMM(mater);
	}
	else{
		cout << "[Error] Input size is different.." << endl;
	}
}

auto JobManager::start()->void
{
    cout << "start() called" << endl;
	m_tmm->init();
	m_tmm->start();
	m_tmm->saveResult();
}

auto JobManager::makeReport()->void
{
    cout << "makeReport() called" << endl;
}

auto JobManager::printModelInfo()->void
{
    cout << "printModelInfo() in Job Manager called .." << endl;
}

auto JobManager::inputmanager(Mater& mater, const YAML::Node& in)->bool
{
	string model = in["Model"].as<string>();
	mater.stack = in["Structure"]["stack"].as<vector<string>>();
	mater.thick = in["Structure"]["thick"].as<vector<float>>();
	mater.n = in["Structure"]["n"].as<vector<float>>();
	mater.k = in["Structure"]["k"].as<vector<float>>();
	mater.num = mater.n.size();
	//YAML::Node optics = in["Optics"];
	
	if (mater.n.size() != mater.k.size() or mater.n.size() != mater.stack.size() or mater.n.size() != mater.thick.size()){
		cout << "[Error] structure, thickness, n and k have different size. " << endl;
		return false;
	}
	return true;
}

JobManager::~JobManager()
{
	delete m_tmm;
	cout << "m_tmm removed" << endl;
}
