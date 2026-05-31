#include "PatientFileLoader.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Patient.h"
#include "Vitals.h"

using namespace std;

void replaceAll(
    std::string& text,
    const std::string& oldValue,
    const std::string& newValue
)
{
    size_t position{ text.find(oldValue) };

    while (position != std::string::npos) {
        text.replace(position, oldValue.length(), newValue);
        position = text.find(oldValue, position + newValue.length());
    }
}

std::string normaliseDiagnosisName(const std::string& diagnosis)
{
    std::string normalisedDiagnosis{ diagnosis };

    // Handle UTF-8 curly apostrophes.
    replaceAll(normalisedDiagnosis, "\xE2\x80\x99", "'");
    replaceAll(normalisedDiagnosis, "\xE2\x80\x98", "'");

    // Handle the common Windows console mojibake display of curly apostrophes.
    replaceAll(normalisedDiagnosis, "ΓÇÖ", "'");
    replaceAll(normalisedDiagnosis, "ΓÇÿ", "'");

    return normalisedDiagnosis;
}

std::vector<Patient*> PatientFileLoader::loadPatientFile(const std::string& file)
{
    vector<Patient*> patients{};

    ifstream inFile(file);

    if (!inFile.is_open()) {
        cout << "Could not open patient file: " << file << endl;
        return patients;
    }

    string line{};

    while (getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream lineStream{ line };

        string uid{};
        string name{};
        string birthdayString{};
        string diagnosis{};
        string vitalsString{};

        getline(lineStream, uid, '|');
        getline(lineStream, name, '|');
        getline(lineStream, birthdayString, '|');
        getline(lineStream, diagnosis, '|');
        getline(lineStream, vitalsString, '|');

        diagnosis = normaliseDiagnosisName(diagnosis);

        stringstream nameStream{ name };

        string lastName{};
        string firstName{};

        getline(nameStream, lastName, ',');
        getline(nameStream, firstName, ',');

        tm birthday{};
        stringstream birthdayStream{ birthdayString };
        birthdayStream >> get_time(&birthday, "%d-%m-%Y");

        Patient* patient = new Patient(firstName, lastName, birthday);
        patient->addDiagnosis(diagnosis);

        if (!vitalsString.empty()) {
            stringstream vitalsRecordsStream{ vitalsString };
            string vitalsRecord{};

            while (getline(vitalsRecordsStream, vitalsRecord, ';')) {
                if (vitalsRecord.empty()) {
                    continue;
                }

                stringstream vitalsStream{ vitalsRecord };

                string bodyTemperatureString{};
                string bloodPressureString{};
                string heartRateString{};
                string respiratoryRateString{};

                getline(vitalsStream, bodyTemperatureString, ',');
                getline(vitalsStream, bloodPressureString, ',');
                getline(vitalsStream, heartRateString, ',');
                getline(vitalsStream, respiratoryRateString, ',');

                float bodyTemperature = stof(bodyTemperatureString);
                int bloodPressure = stoi(bloodPressureString);
                int heartRate = stoi(heartRateString);
                int respiratoryRate = stoi(respiratoryRateString);

                Vitals* vitals = new Vitals(
                    bodyTemperature,
                    bloodPressure,
                    heartRate,
                    respiratoryRate
                );

                patient->addPreviousVitals(vitals);
            }
        }

        patients.push_back(patient);
    }

    inFile.close();

    return patients;

}