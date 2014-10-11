#ifndef ARENCODER_H
#define ARENCODER_H

#include <vector>
#include "ar_model.h"

class AR_Encoder {
    public:
        AR_Encoder();

        void putSymbol(AR_symbol);
        void putVector(std::vector<AR_symbol>);
        std::vector<AR_symbol> getEncodedResult();

    private:
        std::vector<AR_symbol> res;
        bool packed;

        int low;
        int high;
        AR_Model model;
};

#endif // ARENCODER_H
