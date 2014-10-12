#include "ar_encoder.h"

AR_Encoder::AR_Encoder() {
    packed = false;
    low = 0;
    high = AR_MAX_VALUE;
    reverseBits = 0;
}

void AR_Encoder::putSymbol(AR_symbol s) {
    if (packed)
        return;

    int range = high - low + 1;
    high = low + range * model.freq(s) / model.totalFreq() - 1;
    low = low + range * model.freq(s - 1) / model.totalFreq();
    while (true) {
        if (high < AR_HALF)
            writeBit(0);
        else if (low >= AR_HALF)
            writeBit(1);
        else if (low >= AR_FIRST_QRT && high < AR_THIRD_QRT) {
            reverseBits++;
            low -= AR_FIRST_QRT;
            high -= AR_THIRD_QRT;
        } else
            break;

        low *= 2;
        high = high * 2 + 1;
    }

    model.update(s);
}

void AR_Encoder::putVector(std::vector<AR_symbol> s) {
    for (int i = 0; i < s.size(); i++)
        putSymbol(s[i]);
}

void AR_Encoder::writeBit(int bit) {
    res.push_back(bit);
    while (reverseBits) {
        reverseBits--;
        res.push_back(!bit);
    }
}

std::vector<bool> AR_Encoder::getEncodedResult() {
     if (packed)
         return res;
     else {
         reverseBits++;
         if (low < AR_FIRST_QRT)
             writeBit(0);
         else
             writeBit(1);

         return res;
     }
}
