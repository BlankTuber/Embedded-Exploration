#include "tones.h"
#include <stdio.h>

int main() {
    int count = sizeof(tones) / sizeof(tones[0]);
    for (int i = 0; i < count; i++) {
        printf("%i\n", tones[i]);
    }

    return 0;
}
