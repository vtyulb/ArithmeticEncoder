#include "ar_encoder.h"
#include "ar_decoder.h"

AR_Encoder::AR_Encoder() {
    packed = false;
    low = 0;
    high = AR_MAX_VALUE;
    reverseBits = 0;
    size = 0;
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
        else if (low >= AR_HALF) {
            writeBit(1);
            low -= AR_HALF;
            high -= AR_HALF;
        } else if (low >= AR_FIRST_QRT && high < AR_THIRD_QRT) {
            reverseBits++;
            low -= AR_FIRST_QRT;
            high -= AR_FIRST_QRT;
        } else
            break;

        low *= 2;
        high = high * 2 + 1;
    }

    model.update(s);
    size++;
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

void AR_Encoder::getEncodedResult(std::vector<char> &result) {
     if (packed) {
         convert(res, result);
     } else {
         reverseBits++;
         if (low < AR_FIRST_QRT)
             writeBit(0);
         else
             writeBit(1);

         while (res.size() % 8)
             res.push_back(0);

         packed = true;

         convert(res, result);
//         AR_Decoder d(result);
//         std::vector<char> test(d.getDecoded());
//         test[0] = test[0];
     }
}

void AR_Encoder::convert(const std::vector<bool> data, std::vector<char> &res) {
    res.resize(data.size() / 8 + 4, 0);
    *(unsigned int*)res.data() = size;
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < 8; j++)
            if (data[i * 8 + j])
                res[i + 4] |= (char)(1 << j);
}
