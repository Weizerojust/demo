







#include <stdio.h>
#include "merge-sort.hpp"

void merge_sort(int* A, int low, int high)
{
    int mid;
    if (low < high) {
        mid = (high + low)/2;
        merge_sort(A,low, mid);
        merge_sort(A, mid + 1, high);
        merge(A, low, mid, high);
    }
}

void merge(int* A, const int low, const int mid, const int high)
{
    
    
    int n1 = mid - low + 1;
    int n2 = high - mid;
    int L[n1 + 1];
    int R[n2 + 1];
    
    for (int i = 0; i < n1 ; i++) {
        L[i] = A[low + i];
    }
    for (int j = 0; j < n2 ; j++) {
        R[j] = A[mid + 1 + j];
    }
    
    
    L[n1] = 1000;
    R[n2] = 1000;
    
    int i = 0;
    int j = 0;
    for (int k = low; k <= high; k++) {
        if(L[i] < R[j]){
            A[k] = L[i];
            i++;
        } else{
            A[k] = R[j];
            j++;
        }
    }
}