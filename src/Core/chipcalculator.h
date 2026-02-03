#ifndef CHIPCALCULATOR_H
#define CHIPCALCULATOR_H

#include <QList>

class ChipCalculator
{
public:
    static QList<int> getChipsForAmount(int amount);
};

#endif // CHIPCALCULATOR_H
