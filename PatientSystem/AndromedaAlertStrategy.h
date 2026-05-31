#ifndef ANDROMEDAALERTSTRATEGY_H
#define ANDROMEDAALERTSTRATEGY_H

#include "AlertLevelStrategy.h"

class AndromedaAlertStrategy : public AlertLevelStrategy {
public:
    AlertLevel calculateAlertLevel(
        const Patient& patient,
        const Vitals& vitals
    ) const override;
};

#endif