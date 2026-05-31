#ifndef CORDYCEPSALERTSTRATEGY_H
#define CORDYCEPSALERTSTRATEGY_H

#include "AlertLevelStrategy.h"

class CordycepsAlertStrategy : public AlertLevelStrategy {
public:
    AlertLevel calculateAlertLevel(
        const Patient& patient,
        const Vitals& vitals
    ) const override;
};

#endif