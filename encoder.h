#ifndef ENCODER_H
#define ENCODER_H

typedef char AR_symbol;

class Encoder {
    public:
        Encoder();

    private:
        void updateModel(AR_symbol last);

        int freq[sizeof(AR_symbol) * 256 - 1];
};

#endif // ENCODER_H
