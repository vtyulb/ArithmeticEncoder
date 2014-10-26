#include "bwt.h"
#include <algorithm>
#include <vector>

namespace {
    const unsigned int module =  655360001;

    std::vector<unsigned long long> hashes1;
    std::vector<unsigned long long> h1;
    std::vector<unsigned long long> hashes2;
    std::vector<unsigned long long> h2;
    std::vector<char> *r;

    unsigned long long calc1(int a, int count) {
        if (a == 0)
            return hashes1[a + count - 1];

        return hashes1[a + count - 1] - hashes1[a - 1] * h1[count];
    }

    unsigned long long calc2(int a, int count) {
        if (a == 0)
            return hashes2[a + count - 1];

        return (hashes2[a + count - 1] - hashes2[a - 1] * h2[count] + module) % module;
    }

    bool equal(int a, int b, int count) {
        if (count == 0)
            return true;

        if (calc1(a, count) != calc1(b, count))
            return false;

        return true;
    }

    bool lesser(int a, int b) {
        int left = 0;
        int right = r->size() / 2;

        while (left < right - 1)
            if (equal(a, b, (left + right) / 2))
                left = (left + right) / 2;
            else
                right = (left + right) / 2;

        return r->at(a + left) < r->at(b + left);
    }
}

std::vector<char> BWT_Direct(std::vector<char> data) {
    int N = data.size();

    int *res = new int[N];

    for (int i = 0; i < N; i++) {
        res[i] = i;
        data.push_back(data[i]);
    }


    hashes1.push_back(data[0] + int(1));
    hashes2.push_back(data[0] + int(1));
    h1.push_back(1);
    h2.push_back(1);
    for (int i = 1; i < N * 2; i++) {
        hashes1.push_back(hashes1[i - 1] * 999999937 + data[i] + 1);
        hashes2.push_back((hashes2[i - 1] * 337) % module + data[i] + 1);
        hashes2[i] %= module;
        h1.push_back(h1[i - 1] * 999999937);
        h2.push_back(h2[i - 1] * 337);
    }

    r = &data;
    std::sort(res, res + N, lesser);

    hashes1.clear();
    hashes2.clear();
    h1.clear();
    h2.clear();

    std::vector<char> result;
    for (int i = 0; i < N; i++)
        result.push_back(data[res[i] + N - 1]);

    result.resize(result.size() + 4);
    for (int i = 0; i < N; i++)
        if (res[i] == 0)
            *(int*)(result.data() + N) = i;

    delete res;
    return result;
}

std::vector<char> BWT_Backward(std::vector<char> data) {
    int x = *(int*)(data.data() + data.size() - 4);

    data.resize(data.size() - 4);

    int r[257];
    for (int i = 0; i < 256; i++)
        r[i] = 0;

    for (int i = 0; i < data.size(); i++)
        r[(unsigned char)data[i]]++;

    int k = 0;
    for (int i = 0; i < 256; i++) {
        int t = r[i + 1];
        r[i + 1] += r[i];
        r[i] -= k;
        k = t;
    }

    std::vector<int> magic;
    magic.resize(data.size());
    for (int i = 0; i < data.size(); i++)
        magic[r[(unsigned char)data[i]]++] = i;

    std::vector<char> res;
    for (int i = 0; i < data.size(); i++) {
        x = magic[x];
        res.push_back(data[x]);
    }

    return res;
}
