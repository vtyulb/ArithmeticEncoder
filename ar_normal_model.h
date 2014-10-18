#ifndef AR_NORMAL_MODEL_H
#define AR_NORMAL_MODEL_H

#include <queue>
#include "ar_model.h"

class AR_Normal_Model : public AR_Model
{
    public:
        AR_Normal_Model();

        int freq(AR_symbol);
        int totalFreq();
        void update(AR_symbol);
        void resetModel();

    private:
        int _freq[AR_TOTAL_SYMBOLS];
        int _cumFreq[AR_TOTAL_SYMBOLS];
        int _totalFreq;
//        std::queue<AR_symbol> window;
};

#endif // AR_NORMAL_MODEL_H
