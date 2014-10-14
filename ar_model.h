#ifndef ARMODEL_H
#define ARMODEL_H

typedef unsigned char AR_symbol;

const unsigned int AR_MAX_SYMBOL = sizeof(AR_symbol) * 256 - 1;
const unsigned int AR_CODE_VALUE_BITS = 16;
const unsigned int AR_MAX_VALUE = (1 << AR_CODE_VALUE_BITS) - 1;
const unsigned int AR_FIRST_QRT = AR_MAX_VALUE / 4 + 1;
const unsigned int AR_HALF = AR_FIRST_QRT * 2;
const unsigned int AR_THIRD_QRT = AR_FIRST_QRT * 3;

#include <queue>

class AR_Model
{
    public:
        AR_Model();

        int freq(AR_symbol);
        int totalFreq();
        void update(AR_symbol);

    private:
        int _freq[AR_MAX_SYMBOL + 1];
        int _cumFreq[AR_MAX_SYMBOL + 1];
        int _totalFreq;
        std::queue<AR_symbol> window;
};

#endif // MODEL_H
