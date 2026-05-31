#include "Patient.h"
#include "AndromedaAlertStrategy.h"
#include "CordycepsAlertStrategy.h"
#include "KepralsAlertStrategy.h"
#include "PatientAlertObserver.h"
#include "Vitals.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>



using namespace std;

const std::string Diagnosis::CORDYCEPS_BRAIN_INFECTION = "Cordyceps Brain Infection";
const std::string Diagnosis::KEPRALS_SYNDROME = "Kepral's Syndrome";
const std::string Diagnosis::ANDROMEDA_STRAIN= "Andromeda Strain";


Patient::Patient(const std::string& firstName, const std::string& lastName, std::tm birthday) :
	Person(firstName, lastName, birthday),
	_alertLevel(AlertLevel::Green),
	_alertStrategy{ nullptr }
{
}

int Patient::age() const
{	
	// an inaccurate age estimate but fine for assignment purposes
	return 2022 - (1900 + _birthday.tm_year);
}

std::string Patient::uid() const
{
	std::stringstream ss;
	ss << (char)std::tolower(_lastName.at(0)) 
	   << (char)std::tolower(_firstName.at(0))
	   << std::setfill('0') << std::setw(2) << (_birthday.tm_mon + 1) 
	   << _birthday.tm_year;
	return ss.str();
}

std::string Patient::humanReadableID() const
{
	return "" + _lastName + ", " + _firstName + " (" + uid() + ")";
}

std::ostream& operator<<(std::ostream& os, const Patient& p)
{
	os << p.uid() << "|" << p._lastName << "," << p._firstName << "|" << std::put_time(&p._birthday, "%d-%m-%Y") << "|" << p.primaryDiagnosis();
	
	os << "|";
	for (int i = 0; i < p.vitals().size(); ++i) {
		auto v = p.vitals()[i];
		os << *v;
		if (i != p.vitals().size() - 1) {
			os << ";";
		}
	}
	return os;
}

void Patient::addDiagnosis(const std::string& diagnosis)
{
	_diagnosis.push_back(diagnosis);

	if (_alertStrategy != nullptr) {
		return;
	}

	if (diagnosis == Diagnosis::CORDYCEPS_BRAIN_INFECTION) {
		_alertStrategy = std::make_unique<CordycepsAlertStrategy>();
	}
	else if (diagnosis == Diagnosis::KEPRALS_SYNDROME) {
		_alertStrategy = std::make_unique<KepralsAlertStrategy>();
	}
	else if (diagnosis == Diagnosis::ANDROMEDA_STRAIN) {
		_alertStrategy = std::make_unique<AndromedaAlertStrategy>();
	}
}

const std::string& Patient::primaryDiagnosis() const
{
	return _diagnosis.front();
}

void Patient::addVitals(const Vitals* v)
{
	_vitals.push_back(v);
	
	if (_alertStrategy != nullptr) {
		setAlertLevel(_alertStrategy->calculateAlertLevel(*this, *v));
	}
}

void Patient::addPreviousVitals(const Vitals* v)
{
	_vitals.push_back(v);
}

const std::vector<const Vitals*> Patient::vitals() const
{
	return _vitals;
}

void Patient::setAlertLevel(AlertLevel level)
{
	const AlertLevel previousAlertLevel{ _alertLevel };

	_alertLevel = level;

	if (_alertLevel > AlertLevel::Green) {
		cout << "Patient: " << humanReadableID() << " has an alert level: ";
		switch (_alertLevel) {
		case AlertLevel::Yellow:
			cout << "Yellow";
			break;
		case AlertLevel::Orange:
			cout << "Orange";
			break;
		case AlertLevel::Red:
			cout << "Red";
			break;
		default:
			break;
		}
		cout << endl;
	}

	if (previousAlertLevel != AlertLevel::Red && _alertLevel == AlertLevel::Red) {
		notifyObservers();
	}
}

void Patient::addObserver(PatientAlertObserver* observer)
{
	_observers.push_back(observer);
}

void Patient::removeObserver(PatientAlertObserver* observer)
{
	auto position = std::find(_observers.begin(), _observers.end(), observer);

	if (position != _observers.end()) {
		_observers.erase(position);
	}
}

void Patient::notifyObservers()
{
	for (PatientAlertObserver* observer : _observers) {
		observer->update(this);
	}
}
