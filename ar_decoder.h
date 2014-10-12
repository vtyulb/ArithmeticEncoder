#ifndef AR_DECODER_H
#define AR_DECODER_H

#include <vector>
#include "ar_model.h"

class AR_Decoder {
    public:
        AR_Decoder(std::vector<char> data);
        std::vector<AR_symbol> getDecoded();

    private:
        unsigned int low;
        unsigned int high;
        unsigned int value;
        int size;
        long long position;
        AR_Model model;
        std::vector<char> data;

        AR_symbol getNextSymbol();
        int getNextBit();
};

#endif // AR_DECODER_H
