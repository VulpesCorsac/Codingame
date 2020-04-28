#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    // the number of temperatures to analyse
    int n;
    int closest = 10000;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        // a temperature expressed as an integer ranging from -273 to 5526
        int t;
        scanf("%d", &t);

        if ((abs(t) < abs(closest)) || (closest == -t && t > 0)) {
            closest = t;
        }        
    }

    // Write an answer using printf(). DON'T FORGET THE TRAILING \n
    // To debug: fprintf(stderr, "Debug messages...\n");
    if (closest == 10000) {
        closest = 0;    
    }

    printf("%d\n", closest);

    return 0;
}