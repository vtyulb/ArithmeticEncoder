#ifndef ARMODEL_H
#define ARMODEL_H

typedef unsigned char AR_symbol;

const unsigned int AR_TOTAL_SYMBOLS = sizeof(AR_symbol) * 256;
const unsigned int AR_CODE_VALUE_BITS = 30;
const unsigned int AR_MAX_VALUE = (1 << AR_CODE_VALUE_BITS) - 1;
const unsigned int AR_FIRST_QRT = AR_MAX_VALUE / 4 + 1;
const unsigned int AR_HALF = AR_FIRST_QRT * 2;
const unsigned int AR_THIRD_QRT = AR_FIRST_QRT * 3;

#include <queue>

struct header {
    unsigned int ppm:1;
    unsigned int size:31;
};

class AR_Model
{
    public:
        AR_Model() {}

        virtual int freq(AR_symbol) {}
        virtual int totalFreq() {}
        virtual void update(AR_symbol) {}
        virtual void resetModel() {}
};

#endif // MODEL_H
