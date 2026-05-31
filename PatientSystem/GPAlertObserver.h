#ifndef GPALERTOBSERVER_H
#define GPALERTOBSERVER_H

#include <memory>

#include "GPNotificationSystemFacade.h"
#include "PatientAlertObserver.h"

class GPAlertObserver : public PatientAlertObserver {
public:
    GPAlertObserver();

    void update(Patient* patient) override;

private:
    std::unique_ptr<GPNotificationSystemFacade> _gpNotificationSystem;
};

#endif