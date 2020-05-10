#include <stdio.h>

int main() {
    while (1) {
        int max_height = -1;
        int idx = -1;
        for (int i = 0; i < 8; i++) {
            int mountain_h;
            scanf("%d", &mountain_h);
            if (mountain_h > max_height) {
                max_height = mountain_h;
                idx = i;
            }
        }
        printf("%d\n", idx);
    }

    return 0;
}