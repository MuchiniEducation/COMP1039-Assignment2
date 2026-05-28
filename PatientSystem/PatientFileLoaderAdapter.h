#pragma once

#include <string>
#include <vector>

#include "AbstractPatientDatabaseLoader.h"
#include "PatientFileLoader.h"

class PatientFileLoaderAdapter : public AbstractPatientDatabaseLoader {
public:
    PatientFileLoaderAdapter(const std::string& filename);

    virtual void initialiseConnection() override;
    virtual void loadPatients(std::vector<Patient*>& patientIn) override;
    virtual void closeConnection() override;

private:
    std::string _filename;
    PatientFileLoader _fileLoader;
};