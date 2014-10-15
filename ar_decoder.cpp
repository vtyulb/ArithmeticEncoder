#include "ar_decoder.h"
#include <stdio.h>

#include "ar_model.h"
#include "ar_ppm_model.h"

AR_Decoder::AR_Decoder(const std::vector<char> data):
    data(data)
{
    value = 0;
    low = 0;
    high = AR_MAX_VALUE;

    header h = *(header*)data.data();
    size = h.size;

    if (h.ppm)
        model = new AR_PPM_Model();
    else
        model = new AR_Model();

    position = 8 * sizeof(header);
    for (int i = 0; i < AR_CODE_VALUE_BITS; i++)
        value = value * 2 + getNextBit();
}

std::vector<AR_symbol> AR_Decoder::getDecoded() {
    std::vector<AR_symbol> res;
    res.resize(size);
    for (int i = 0; i < size; i++)
        res[i] = getNextSymbol();

    return res;
}

AR_symbol AR_Decoder::getNextSymbol() {
    long long range = high - low + 1;
    int cum = ((value - low + 1) * model->totalFreq() - 1) / range;
    AR_symbol symbol = 0;

    while (model->freq(symbol) <= cum)
        symbol++;

    high = low + (range * model->freq(symbol)) / model->totalFreq() - 1;
    if (symbol != 0)
        low = low + (range * model->freq(symbol - 1)) / model->totalFreq();

    while (1) {
        if (high < AR_HALF) {

        } else if (low >= AR_HALF) {
            value -= AR_HALF;
            low -= AR_HALF;
            high -= AR_HALF;
        } else if (low >= AR_FIRST_QRT && high < AR_THIRD_QRT) {
            value -= AR_FIRST_QRT;
            low -= AR_FIRST_QRT;
            high -= AR_FIRST_QRT;
        } else
            break;

        low *= 2;
        high = high * 2 + 1;
        value = 2 * value + getNextBit();
    }

    model->update(symbol);
    return symbol;
}

int AR_Decoder::getNextBit() {
    if (position >= data.size() * 8)
        return 0;

    int res = data[position / 8] == (data[position / 8] | (1 << (position % 8)));
    position++;
    return res;
}
