#ifndef KEPRALSALERTSTRATEGY_H
#define KEPRALSALERTSTRATEGY_H

#include "AlertLevelStrategy.h"

class KepralsAlertStrategy : public AlertLevelStrategy {
public:
    AlertLevel calculateAlertLevel(
        const Patient& patient,
        const Vitals& vitals
    ) const override;
};

#endif