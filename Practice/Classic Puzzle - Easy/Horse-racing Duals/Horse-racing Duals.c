#include <stdio.h>

int cmp(const void* a, const void* b) {
    int* x = (int*) a;
    int* y = (int*) b;
    return *x - *y;
}

int main() {
    int N, t,  horse[100000];
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        int pi;
        scanf("%d", &pi);
        horse[i] = pi;
    }

    qsort(horse, N, sizeof(int), cmp);

    int ans = horse[N-1];
    for (int i = 1; i < N; ++i) {
        if (ans > horse[i] - horse[i-1] ) {
            ans = horse[i] - horse[i-1];
        }
    }

    printf("%d\n", ans);

    return 0;
}