#ifndef AR_PPM_MODEL_H
#define AR_PPM_MODEL_H

#include "ar_model.h"
#include <list>
#include <map>

const int AR_PPM_MODEL_ORDER = 3;
const int AR_PPM_MODEL_AGRO = 4;

struct index {
    int r[AR_PPM_MODEL_ORDER];

    bool operator < (const index &i) const {
        for (int j = 0; j < AR_PPM_MODEL_ORDER; j++)
            if (r[j] < i.r[j])
                return true;
            else if (r[j] > i.r[j])
                return false;

        return false;
    }
};

class AR_PPM_Model : public AR_Model {
    public:
        AR_PPM_Model();

        int freq(AR_symbol);
        int totalFreq();
        void update(AR_symbol);

    private:
        std::list<AR_symbol> lastSymbols;
        std::map<index, int* > table;

        //cache block
        int cumFreq[AR_TOTAL_SYMBOLS];
        bool cached;
        int _totalFreq;
        //end of cache block

        void cacheIt();
        int *getCurrentBlock();

};

#endif // AR_PPM_MODEL_H
