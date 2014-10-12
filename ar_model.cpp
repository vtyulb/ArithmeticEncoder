#include "ar_model.h"

AR_Model::AR_Model() {
    for (int i = 0; i <= AR_MAX_SYMBOL; i++)
        _freq[i] = 1;

    _totalFreq = AR_MAX_SYMBOL + 1;
}

int AR_Model::freq(AR_symbol s) {
    int res = 0;
    for (int i = 0; i <= (unsigned char)s; i++)
        res += _freq[i];

    return res;
}

int AR_Model::totalFreq() {
    return _totalFreq;
}

void AR_Model::update(AR_symbol s) {
    _freq[s] += 50;
    _totalFreq += 50;
    if (_totalFreq >= AR_FIRST_QRT) {
        _totalFreq = 0;
        for (int i = 0; i <= AR_MAX_SYMBOL; i++) {
            _freq[i] /= 3;
            _freq[i]++;
            _totalFreq += _freq[i];
        }
    }
}
