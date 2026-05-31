#include "AndromedaAlertStrategy.h"

#include "Patient.h"
#include "Vitals.h"

AlertLevel AndromedaAlertStrategy::calculateAlertLevel(
    const Patient& patient,
    const Vitals& vitals
) const
{
    if (vitals.BP() > 140) {
        return AlertLevel::Red;
    }

    if (vitals.BP() > 130) {
        return AlertLevel::Orange;
    }

    if (vitals.BP() > 110) {
        return AlertLevel::Yellow;
    }

    return AlertLevel::Green;
}