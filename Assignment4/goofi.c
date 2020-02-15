#include <stdio.h>

int main() {
    char k = 'A';
    for(int i=0;i<10;i++) {
        printf("%c,\t", k);
        k++;
    }

    return 0;
}