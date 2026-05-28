#include "PatientFileLoaderAdapter.h"

PatientFileLoaderAdapter::PatientFileLoaderAdapter(const std::string& filename) :
    _filename(filename),
    _fileLoader()
{
}

void PatientFileLoaderAdapter::initialiseConnection() {}

void PatientFileLoaderAdapter::loadPatients(std::vector<Patient*>& patientIn)
{
    std::vector<Patient*> loadedPatients = _fileLoader.loadPatientFile(_filename);

    for (Patient* patient : loadedPatients) {
        patientIn.push_back(patient);
    }
}

void PatientFileLoaderAdapter::closeConnection() {}