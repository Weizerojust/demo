


#ifndef BUBBLESORT_CPP
#define BUBBLESORT_CPP

#include "BubbleSort.hpp"


template <class T, std::size_t N>
BubbleSort<T, N>::BubbleSort() = default;


template <class T, std::size_t N>
void BubbleSort<T, N>::Sort(std::array<T, N> & input) {
	long size = input.size();
	for (unsigned long i = 0; i < size - 1; i++) {
		for (unsigned long j= 0; j < size - i - 1; j++) {
			if (input[j] > input[j + 1]) 
				std::swap(input[j], input[j + 1]);
		}
	}
}


template <class T, std::size_t N>
BubbleSort<T, N>::~BubbleSort() = default;


#endif