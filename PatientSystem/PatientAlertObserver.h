#ifndef PATIENTALERTOBSERVER_H
#define PATIENTALERTOBSERVER_H

class Patient;

class PatientAlertObserver {
public:
    virtual ~PatientAlertObserver() = default;

    virtual void update(Patient* patient) = 0;
};

#endif