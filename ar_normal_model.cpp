#include "ar_normal_model.h"

AR_Normal_Model::AR_Normal_Model() {
    resetModel();
}

int AR_Normal_Model::freq(AR_symbol s) {
    return _cumFreq[s];
}

int AR_Normal_Model::totalFreq() {
    return _totalFreq;
}

void AR_Normal_Model::update(AR_symbol s) {
    static int count = 0;
    const int diff = 5;
    count++;

    _freq[s] += diff;
    _totalFreq += diff;

    if (_totalFreq >= AR_FIRST_QRT) {
        _totalFreq = 0;
        for (int i = 0; i < AR_TOTAL_SYMBOLS; i++) {
            _freq[i] /= 2;
            if (_freq[i] == 0)
                _freq[i] = 1;

            _totalFreq += _freq[i];
        }
    }

    _cumFreq[0] = _freq[0];
    for (int i = 1; i < AR_TOTAL_SYMBOLS; i++)
        _cumFreq[i] = _cumFreq[i - 1] + _freq[i];
}

void AR_Normal_Model::resetModel() {
    for (int i = 0; i < AR_TOTAL_SYMBOLS; i++)
        _freq[i] = 1;

    _totalFreq = AR_TOTAL_SYMBOLS;
    update(32);
}
