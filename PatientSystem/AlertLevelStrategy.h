#ifndef ALERTLEVELSTRATEGY_H
#define ALERTLEVELSTRATEGY_H

#include "PatientAlertLevels.h"

class Patient;
class Vitals;

class AlertLevelStrategy {
public:
    virtual ~AlertLevelStrategy() = default;

    virtual AlertLevel calculateAlertLevel(
        const Patient& patient,
        const Vitals& vitals
    ) const = 0;
};

#endif