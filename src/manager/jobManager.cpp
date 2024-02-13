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
	inpts.stack = in["Structure"]["stack"].as<vector<std::string>>();
	inpts.thick = in["Structure"]["thick"].as<vector<float>>();
	inpts.superstrate = in["Structure"]["superstrate"].as<std::string>();
	inpts.substrate = in["Structure"]["substrate"].as<std::string>();
	inpts.nklib = in["Structure"]["nklib"].as<std::string>();
	inpts.num = inpts.stack.size();

	//To be deleted
	inpts.n = in["Structure"]["n"].as<vector<float>>();
	inpts.k = in["Structure"]["k"].as<vector<float>>();

	if (inpts.stack.size() != inpts.thick.size()){
		cout << "[Error] structure, thickness, n and k have different size. " << endl;
		return false;
	}
	return true;
}

auto JobManager::opticsUpdate(Inputs& inpts, const YAML::Node& in)->void
{
    //wavelength: [300, 310, 10]
    //incident_polar_angle: [0]
    //polarization: 0.5               ## 0.5 x TE : 0.5 x TM
    //dop: 1                          ## degree of polarization (to be implemented)
	inpts.wavelength = in["Optics"]["wavelength"].as<vector<float>>();
	inpts.polar_angles = in["Optics"]["incident_polar_angle"].as<vector<float>>();
	inpts.pol = in["Optics"]["polarization"].as<float>();
	inpts.dop = in["Optics"]["dop"].as<float>();
}


JobManager::~JobManager()
{
	delete m_tmm;
	cout << "m_tmm removed" << endl;
}
