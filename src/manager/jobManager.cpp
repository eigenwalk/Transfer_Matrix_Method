#include <omp.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include "jobManager.h"

auto JobManager::init()->void
{
	cout << "JobManager::init().." << endl;
    YAML::Node config = YAML::LoadFile(m_inputfile);
	Inputs inpts;
	if (inputmanager(inpts, config["Configuration"])){
		m_tmm = new TMM(inpts);
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

auto JobManager::inputmanager(Inputs& inpts, const YAML::Node& in)->bool
{
	if (structureUpdate(inpts, in))
	{
		opticsUpdate(inpts, in);
		return true;
	}
	return false;
}

auto JobManager::structureUpdate(Inputs& inpts, const YAML::Node& in)->bool
{
	string model = in["Model"].as<string>();
	inpts.stack = in["Structure"]["stack"].as<vector<string>>();
	inpts.thick = in["Structure"]["thick"].as<vector<float>>();
	inpts.n = in["Structure"]["n"].as<vector<float>>();
	inpts.k = in["Structure"]["k"].as<vector<float>>();
	inpts.num = inpts.n.size();
	if (inpts.n.size() != inpts.k.size() or inpts.n.size() != inpts.stack.size() or inpts.n.size() != inpts.thick.size()){
		cout << "[Error] structure, thickness, n and k have different size. " << endl;
		return false;
	}
	return true;
}

auto JobManager::opticsUpdate(Inputs& inpts, const YAML::Node& in)->void
{
    //wavelength: [300, 310, 10]
    //incident_polar_angle: 0
    //incident_azimuthal_angle: 0
    //polarization: 0.5               ## 0.5 x TE : 0.5 x TM
    //dop: 1                          ## degree of polarization (to be implemented)
	inpts.wavelength = in["Optics"]["wavelength"].as<vector<float>>();
	inpts.polar_ang = in["Optics"]["incident_polar_angle"].as<float>();
	inpts.azi_ang = in["Optics"]["incident_azimuthal_angle"].as<float>();
	inpts.pol = in["Optics"]["polarization"].as<float>();
	inpts.dop = in["Optics"]["dop"].as<float>();
}

JobManager::~JobManager()
{
	delete m_tmm;
	cout << "m_tmm removed" << endl;
}
