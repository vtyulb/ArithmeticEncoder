#include "ar_encoder.h"

AR_Encoder::AR_Encoder() {
    packed = false;
    low = 0;
    high = AR_MAX_VALUE;
}

std::vector<AR_symbol> AR_Encoder::getEncodedResult() {
    packed = true;
}

void AR_Encoder::putSymbol(AR_symbol s) {
    if (packed)
        return;

    int range = high - low + 1;
    high = low + range * model.freq(s) / model.totalFreq();
    low = low + range * model.freq(s - 1) / model.totalFreq();
}

void AR_Encoder::putVector(std::vector<AR_symbol> s) {
    for (int i = 0; i < s.size(); i++)
        putSymbol(s[i]);
}
