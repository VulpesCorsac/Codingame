#include <stdio.h>

int abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int main() {
    int surface_n, last_land_x, last_land_y;
    int landing_y, landing_x, landing_length;
    scanf("%d", &surface_n);
    for (int i = 0; i < surface_n; i++) {
        int land_x;
        int land_y;
        scanf("%d%d", &land_x, &land_y);
        if (i > 0 && land_y == last_land_y) {
            landing_y = land_y;
            landing_x = (land_x + last_land_x) / 2;
            landing_length = abs(land_x - last_land_x);
        }
        last_land_x = land_x;
        last_land_y = land_y;
    }

    while (1) {
        int X, Y, h_speed, v_speed, fuel, rotate, power;
        scanf("%d%d%d%d%d%d%d", &X, &Y, &h_speed, &v_speed, &fuel, &rotate, &power);

        if (abs(landing_x - X) < (landing_length + 100) / 2) {
            if (h_speed > 10) {
                printf("50 4\n");
            } else if (h_speed < -10) {
                printf("-50 4\n");
            } else {
                if (v_speed < -39) {
                    printf("0 4\n");
                } else {
                    printf("0 2\n");
                }
            }
        } else {
            if (landing_x > X) {
                if (h_speed > 55) {
                    printf("20 4\n");
                } else {
                    printf("-20 4\n");
                }
            } else {
                if (h_speed < -55) {
                    printf("-20 4\n");
                } else {
                    printf("20 4\n");
                }
            }
        }
    }

    return 0;
}