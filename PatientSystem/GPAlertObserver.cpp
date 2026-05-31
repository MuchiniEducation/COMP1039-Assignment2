#include "GPAlertObserver.h"

GPAlertObserver::GPAlertObserver() :
    _gpNotificationSystem{ std::make_unique<GPNotificationSystemFacade>() }
{
}

void GPAlertObserver::update(Patient* patient) {
    _gpNotificationSystem->sendGPNotificationForPatient(patient);
}