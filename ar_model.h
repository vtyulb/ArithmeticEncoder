#ifndef ARMODEL_H
#define ARMODEL_H

typedef unsigned char AR_symbol;

const int AR_MAX_SYMBOL = sizeof(AR_symbol) * 256 - 1;
const int AR_CODE_VALUE_BITS = 16;
const int AR_MAX_VALUE = (1 << AR_CODE_VALUE_BITS) - 1;
const int AR_MAX_FREQUENCY = 65535;
const int AR_FIRST_QRT = AR_MAX_VALUE / 4 + 1;
const int AR_HALF = AR_FIRST_QRT * 2;
const int AR_THIRD_QRT = AR_FIRST_QRT * 3;

class AR_Model
{
    public:
        AR_Model();

        double freq(AR_symbol);
        int totalFreq();
        void update(AR_symbol);

    private:
        int _freq[AR_MAX_SYMBOL];
        int _totalFreq;
};

#endif // MODEL_H
