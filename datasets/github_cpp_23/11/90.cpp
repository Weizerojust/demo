#include <iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>
#include "HeapSort.h"




HeapSort::HeapSort(const std::initializer_list<int> &array) {
  for (std::initializer_list<int>::iterator iter = array.begin(); iter != array.end(); ++iter) {
    _vec.push_back(*iter);
  }
}



void HeapSort::print() {
  std::reverse(_vec.begin(), _vec.end());
  for (std::vector<int>::iterator iter = _vec.begin(); iter != _vec.end(); ++iter) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
}



void HeapSort::heapify(std::vector<int>* vec) {
	int start = (vec->size() / 2) - 1;
	std::vector<int>& arr = *vec;

	while (start >= 0) {
		repaidHeap(&arr, start, arr.size() - 1);
		start -= 1;
	}
}



void HeapSort::repaidHeap(std::vector<int>* vec, int start, int end) {
	std::vector<int>& arr = *vec;
	int node = start;
	while ((node * 2) + 1 <= end) {
		int leftNode = node * 2 + 1;
		if (leftNode + 1 <= end &&  arr[leftNode] > arr[leftNode + 1]) {
			leftNode = leftNode + 1;
			
		}
		if (leftNode <= end && arr[leftNode] < arr[node]) {
			std::swap(arr[leftNode], arr[node]);
			node = leftNode;
		} else { break; }
	}
}



void HeapSort::heapSort() {
	
	heapify(&_vec);
	int end =_vec.size() - 1;
	while (end > 0) {
		std::swap(_vec[0], _vec[end]);
		repaidHeap(&_vec, 0, end - 1);
		end -= 1;
	}
}



HeapSort::~HeapSort() {
}

