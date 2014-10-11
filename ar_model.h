#ifndef ARMODEL_H
#define ARMODEL_H

typedef char AR_symbol;

const int AR_MAX_SYMBOL = sizeof(AR_symbol) * 256 - 1;
const int AR_MAX_VALUE = (1 << 16) - 1;
const int AR_MAX_FREQUENCY = 65535;

class AR_Model
{
    public:
        AR_Model();

        int freq(AR_symbol);
        void update(AR_symbol);

    private:
        int _freq[AR_MAX_SYMBOL];
        int total_freq;
};

#endif // MODEL_H
