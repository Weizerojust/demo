#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand( time(NULL) ); 
    int a[10], i, j;
    
    for (i = 0; i < 10; i++) {
        a[i] = rand() % 100;
        printf("%d, ", a[i]);
    }
    
    
    for (i = 0; i <= 9; i++) {
        for (j = 0; j <= 8; j++) {
            if (a[j] > a[j+1]) {
                int aux = a[j];
                a[j] = a[j+1];
                a[j+1] = aux;
            }
        }
    }
    
    
    printf("\n");
    for (i = 0; i < 10; i++) {
        printf("%d, ", a[i]);
    }
}




