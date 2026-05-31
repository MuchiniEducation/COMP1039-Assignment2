#ifndef HOSPITALALERTOBSERVER_H
#define HOSPITALALERTOBSERVER_H

#include <memory>

#include "HospitalAlertSystemFacade.h"
#include "PatientAlertObserver.h"

class HospitalAlertObserver : public PatientAlertObserver {
public:
    HospitalAlertObserver();

    void update(Patient* patient) override;

private:
    std::unique_ptr<HospitalAlertSystemFacade> _hospitalAlertSystem;
};

#endif