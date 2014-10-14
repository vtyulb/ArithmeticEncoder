#include "ar_model.h"

AR_Model::AR_Model() {
    for (int i = 0; i <= AR_MAX_SYMBOL; i++)
        _freq[i] = 1;

    _totalFreq = AR_MAX_SYMBOL + 1;
    update(32);
}

int AR_Model::freq(AR_symbol s) {
    return _cumFreq[s];
}

int AR_Model::totalFreq() {
    return _totalFreq;
}

void AR_Model::update(AR_symbol s) {
    static int count = 0;
    const int diff = 5;
    count++;

    window.push(s);

    _freq[s] += diff;
    _totalFreq += diff;

    if (count > 2000) {
        _freq[window.front()] -= diff;
        _totalFreq -= diff;
        window.pop();
    }

    if (_totalFreq >= AR_FIRST_QRT) {
        _totalFreq = 0;
        for (int i = 0; i <= AR_MAX_SYMBOL; i++) {
            _freq[i] /= 2;
            if (_freq[i] == 0)
                _freq[i] = 1;

            _totalFreq += _freq[i];
        }
    }

    _cumFreq[0] = _freq[0];
    for (int i = 1; i <= AR_MAX_SYMBOL; i++)
        _cumFreq[i] = _cumFreq[i - 1] + _freq[i];
}
