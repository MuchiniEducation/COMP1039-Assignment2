#pragma once

#include <memory>
#include <vector>

#include "AbstractPatientDatabaseLoader.h"

class CompositePatientLoader : public AbstractPatientDatabaseLoader {
public:
    CompositePatientLoader() = default;

    void addLoader(std::unique_ptr<AbstractPatientDatabaseLoader> loader);

    void initialiseConnection() override;
    void loadPatients(std::vector<Patient*>& patientsIn) override;
    void closeConnection() override;

private:
    std::vector<std::unique_ptr<AbstractPatientDatabaseLoader>> _loaders;
};