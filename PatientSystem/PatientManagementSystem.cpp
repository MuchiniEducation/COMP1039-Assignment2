#include "PatientManagementSystem.h"

#include <iostream>
#include <map>
#include <tuple>

#include "Patient.h"
#include "PatientDatabaseLoader.h"
#include "PatientFileLoaderAdapter.h"
#include "Vitals.h"
#include "CompositePatientLoader.h"
#include "PatientFileLoaderAdapter.h"
#include "GPAlertObserver.h"
#include "HospitalAlertObserver.h"
#include "PatientAlertObserver.h"

#include "GPNotificationSystemFacade.h"
#include "HospitalAlertSystemFacade.h"

using namespace std;


PatientManagementSystem::PatientManagementSystem() :
	_patientDatabaseLoader{ nullptr },
	_hospitalAlertSystem{ std::make_unique<HospitalAlertSystemFacade>() },
	_gpNotificationSystem{ std::make_unique<GPNotificationSystemFacade>() }
{
	std::unique_ptr<CompositePatientLoader> loader{
		std::make_unique<CompositePatientLoader>()
	};

	loader->addLoader(std::make_unique<PatientDatabaseLoader>());
	loader->addLoader(std::make_unique<PatientFileLoaderAdapter>("patients.txt"));

	_patientDatabaseLoader = std::move(loader);
	_patientDatabaseLoader->initialiseConnection();
}

PatientManagementSystem::~PatientManagementSystem()
{
	_patientDatabaseLoader->closeConnection();

	// clear patient memory
	for (Patient* p : _patients) {
		delete p;
	}
}

void PatientManagementSystem::init()
{
	_patientDatabaseLoader->loadPatients(_patients);
	for (Patient* p : _patients) {
		_patientLookup[p->uid()] = p;
	}

	_patientAlertObservers.push_back(std::make_unique<HospitalAlertObserver>());
	_patientAlertObservers.push_back(std::make_unique<GPAlertObserver>());

	for (Patient* patient : _patients) {
		for (const std::unique_ptr<PatientAlertObserver>& observer : _patientAlertObservers) {
			patient->addObserver(observer.get());
		}
	}
}

void PatientManagementSystem::run()
{
	printWelcomeMessage();

	bool running = true;
	while (running) {
		printMainMenu();
		int option = 0;
		cin >> option;

		// handle basic errors
		if (cin.bad()) {
			cin.clear();
			cin.ignore();
			continue;
		}

		// switch based on the selected option
		switch (option) {
		case 1:
			printPatients();
			break;
		case 2:
			addVitalsRecord();
			break;
		case 3:
			running = false;
			break;
		}
	}
}

void PatientManagementSystem::addVitalsRecord()
{
	cout << "Patients" << endl;
	printPatients();
	cout << endl;
	cout << "Enter the patient ID to declare vitals for > ";
	
	string pid { "" };
	cin >> pid;
	if (_patientLookup.count(pid)) {
		float bodyTemperature;
		int bloodPressure;
		int heartRate;
		int respitoryRate;

		cout << "enter body temperature: ";
		cin >> bodyTemperature;
		cout << "enter blood pressure: ";
		cin >> bloodPressure;
		cout << "enter heart rate: ";
		cin >> heartRate;
		cout << "enter respitory rate: ";
		cin >> respitoryRate;

		Vitals* v = new Vitals(bodyTemperature, bloodPressure, heartRate, respitoryRate);
		_patientLookup[pid]->addVitals(v);
	}
	else {
		cout << "Patient not found" << endl;
	}
}

void PatientManagementSystem::printWelcomeMessage() const
{
	cout << "WELCOME TO HEALTHCO 3000" << endl;
	cout << "------------------------" << endl;
}

void PatientManagementSystem::printMainMenu() const
{
	cout << endl << "Select an option:" << endl;
	cout << "1. List patients" << endl;
	cout << "2. Add vitals record" << endl;
	cout << "3. Quit" << endl;
	cout << "> ";
}

void PatientManagementSystem::printPatients() const
{
	for (Patient* p : _patients) {
		std::cout << *p << std::endl;
	}
}
