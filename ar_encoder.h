#ifndef ARENCODER_H
#define ARENCODER_H

#include <vector>
#include "ar_model.h"

class AR_Encoder {
    public:
        AR_Encoder();

        void putSymbol(AR_symbol);
        void putVector(std::vector<AR_symbol>);
        std::vector<bool> getEncodedResult();

    private:
        std::vector<bool> res;
        bool packed;

        unsigned int low;
        unsigned int high;
        int reverseBits;
        AR_Model model;

        void writeBit(int bit);
};

#endif // ARENCODER_H
