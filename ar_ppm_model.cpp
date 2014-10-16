#include "ar_ppm_model.h"

#include <stdlib.h>
#include <memory.h>

AR_PPM_Model::AR_PPM_Model() {
    for (int i = 0; i < AR_PPM_MODEL_ORDER; i++)
        lastSymbols.push_back(32);

    cached = false;
}

void AR_PPM_Model::update(AR_symbol s) {
    for (int i = 0; i < AR_PPM_MODEL_ORDER; i++) {
        int *block = getCurrentBlock(i);
        block[s] += AR_PPM_MODEL_AGRO / (AR_PPM_MODEL_ORDER - i);
    }

    lastSymbols.push_back(s);
    lastSymbols.pop_front();

    cached = false;
}

int AR_PPM_Model::freq(AR_symbol s) {
    if (!cached)
        cacheIt();

    return cumFreq[s];
}

int AR_PPM_Model::totalFreq() {
    if (!cached)
        cacheIt();

    return _totalFreq;
}

void AR_PPM_Model::cacheIt() {
    int *block = getCurrentBlock(AR_PPM_MODEL_ORDER - 1);

    cumFreq[0] = block[0];
    for (int i = 1; i < AR_TOTAL_SYMBOLS; i++)
        cumFreq[i] = cumFreq[i - 1] + block[i];

    _totalFreq = cumFreq[AR_TOTAL_SYMBOLS - 1];

    if (_totalFreq >= AR_FIRST_QRT) {
        for (int i = 0; i < AR_TOTAL_SYMBOLS; i++)
            block[i] = block[i] / 2 + 1;

        cacheIt();
    } else
        cached = true;
}

int *AR_PPM_Model::getCurrentBlock(int order) {
    std::list<AR_symbol>::iterator it = lastSymbols.begin();

    struct index i;
    i.order = order;
    for (int j = 0; j < AR_PPM_MODEL_ORDER; j++)
        i.r[j] = *(it++);

    if (table.find(i) == table.end()) {
        int *block = new int[AR_TOTAL_SYMBOLS];
        for (int j = 0; j < AR_TOTAL_SYMBOLS; j++)
            block[j] = 1;

        table[i] = block;
        return block;
    }

    return table[i];
}

void AR_PPM_Model::resetModel() {
    std::map<struct index, int* >::iterator i = table.begin();
    while (i != table.end())
        delete (*(i++)).second;

    table.clear();
    cached = false;
}
