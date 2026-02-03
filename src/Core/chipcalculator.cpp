#include "chipcalculator.h"

QList<int> ChipCalculator::getChipsForAmount(int amount)
{
    QList<int> resultChips;

    const QList<int> denominations = {1000, 500, 100, 50, 25, 10};

    int tempAmount = amount;

    for (int value : denominations) {
        while (tempAmount >= value) {
            resultChips.append(value);
            tempAmount -= value;
        }
    }

    return resultChips;
}
