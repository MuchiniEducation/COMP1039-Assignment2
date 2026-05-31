#include "CompositePatientLoader.h"

void CompositePatientLoader::addLoader(
    std::unique_ptr<AbstractPatientDatabaseLoader> loader
) {
    _loaders.push_back(std::move(loader));
}

void CompositePatientLoader::initialiseConnection() {
    for (const std::unique_ptr<AbstractPatientDatabaseLoader>& loader : _loaders) {
        loader->initialiseConnection();
    }
}

void CompositePatientLoader::loadPatients(std::vector<Patient*>& patientsIn) {
    for (const std::unique_ptr<AbstractPatientDatabaseLoader>& loader : _loaders) {
        loader->loadPatients(patientsIn);
    }
}

void CompositePatientLoader::closeConnection() {
    for (const std::unique_ptr<AbstractPatientDatabaseLoader>& loader : _loaders) {
        loader->closeConnection();
    }
}