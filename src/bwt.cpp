#include "bwt.h"
#include <algorithm>
#include <memory.h>
#include <vector>
#include <stdio.h>

//namespace {
//    void sortIt(int *target, int *destination, )
//}

std::vector<char> BWT_Direct(std::vector<char> data) {
    int N = data.size();

//    int *p = new int[N];
//    int *c = new int[N];
//    int *cnt = new int[N];
//    int *pn = new int[N];
//    int *cn = new int[N];
    std::vector<int> p(N), c(N), cnt(256 + N), pn(N), cn(N);

    memset(cnt.data(), 0, sizeof(int) * 256);
    for (int i = 0; i < N; i++)
        cnt[(unsigned char)data[i]]++;

    for (int i = 1; i < 256; i++)
        cnt[i] += cnt[i - 1];

    for (int i = 0; i < N; i++)
        p[--cnt[(unsigned char)data[i]]] = i;

    int classes = 0;
    c[p[0]] = classes++;
    for (int i = 1; i < N; i++) {
        if (data[p[i]] != data[p[i - 1]])
            classes++;

        c[p[i]] = classes - 1;
    }

    for (int stage = 0; (1 << stage) < N; stage++) {
        for (int i = 0; i < N; i++)
            pn[i] = (p[i] - (1 << stage) + N) % N;

        memset(cnt.data(), 0, sizeof(int) * classes);
        for (int i = 0; i < N; i++)
            cnt[c[pn[i]]]++;

        for (int i = 1; i < N; i++)
            cnt[i] += cnt[i - 1];

        for (int i = N - 1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];

        classes = 0;
        cn[p[0]] = classes++;
        for (int i = 1; i < N; i++) {
            if (c[p[i - 1]] != c[p[i]])
                classes++;
            else if (c[(p[i - 1] + (1 << stage)) % N] != c[(p[i] + (1 << stage)) % N])
                classes++;

            cn[p[i]] = classes - 1;
        }
        memcpy(c.data(), cn.data(), sizeof(int) * N);
    }

    std::vector<char> res;
    for (int i = 0; i < N; i++)
        res.push_back(data[(p[i] + N - 1 + N) % N]);

    res.resize(res.size() + sizeof(int));
    for (int i = 0; i < N; i++)
        if (p[i] == 0)
            *(int*)(res.data() + res.size() - sizeof(int)) = i;

    /*std::vector<char> check = BWT_Backward(res);
    for (int i = 0; i < data.size(); i++)
        if (check[i] != data[i]) {
            printf("err %d: %d %d\n", i, check[i], data[i]);
        }*/

    return res;
}

std::vector<char> BWT_Backward(std::vector<char> data) {
    int x = *(int*)(data.data() + data.size() - sizeof(int));

    data.resize(data.size() - 4);

    int r[256];
    memset(r, 0, 256 * sizeof(int));

    for (int i = 0; i < data.size(); i++)
        r[(unsigned char)data[i]]++;

    int k = r[0];
    for (int i = 0; i < 255; i++) {
        int t = r[i + 1];
        r[i + 1] += r[i];
        r[i] -= k;
        k = t;
    }
    r[255] -= k;

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
