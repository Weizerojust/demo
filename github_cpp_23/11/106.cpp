#include <cstdlib>
#include <fstream>  
#include <ctime>
#include <limits.h>
#include <iostream>
#include <stdlib.h>     
#include <time.h>


const short A = 10000; 
const unsigned int MAX = 150000; 

using namespace std;


void losuj(unsigned int tab[], unsigned int n)
{
	for (unsigned int i = 0; i < n; i++)
	{
		
		tab[i] = rand() % ULONG_MAX + 1;
	}
}
void losujRosnac(unsigned int tab[], unsigned int n)
{
	tab[0] = 1;
	for (unsigned int i = 1; i <n; i++)
	{
		
		tab[i] = tab[i - 1] + 1;
	}
}
void losujMalejac(unsigned int tab[], unsigned int n)
{
	tab[0] = n + 1;
	for (unsigned int i = 1; i <n; i++)
	{
		
		tab[i] = tab[i - 1] - 1;
	}
}
void losujV(unsigned int tab[], unsigned int n)
{
	tab[0] = n;
	unsigned int i = 0;
	for (i; i <(n / 2); i++)
	{

		tab[i] = tab[i - 1] - 1;
	}
	for (i; i < n; i++)
	{
		tab[i] = tab[i - 1] + 1;
	}
}
void losujStale(unsigned int tab[], unsigned int n)
{
	tab[0] = rand() % ULONG_MAX + 1;
	for (unsigned int i = 0; i <n; i++)
	{
		
		tab[i] = tab[0];
	}
}

void wypisz(unsigned int data[], unsigned int n)
{
	for (unsigned int i = 0; i<n; i++)
	{
		cout << data[i] << ", ";
	}
}

bool if_sorted(unsigned int data[], unsigned int n)
{
	for (unsigned int i = 0; i < n - 1; i++)
	{
		if (data[i] > data[i + 1])
			return 0;
	}
	return 1;
}

bool if_sorted_heap(unsigned int data[], unsigned int n)
{
	for (unsigned int i = 1; i < n; i++)
	{
		if (data[i] > data[i + 1])
			return 0;
	}
	return 1;
}

void heapify(unsigned int data[], unsigned int hsize, unsigned int i)
{
	unsigned int largest, l = 2 * i, r = (2 * i) + 1;
	if (l <= hsize && data[l] > data[i])
		largest = l;
	else
		largest = i;
	if (r <= hsize && data[r] > data[largest])
		largest = r;
	if (largest != i)
	{
		swap(data[largest], data[i]);
		heapify(data, hsize, largest);
	}
}

void build_heap(unsigned int data[], unsigned int hsize)
{
	for (unsigned int i = hsize / 2; i > 0; i--)
	{
		heapify(data, hsize, i);
	}
}

void heap_sort(unsigned int data[], unsigned int n)
{
	build_heap(data, n);
	for (unsigned int i = n; i > 1; i--)
	{
		swap(data[i], data[1]);
		n--;
		heapify(data, n, 1);
	}
}

void merge_sort(unsigned int *tab, unsigned int pocz, unsigned int size, unsigned int *b)
{
	int sr = ((pocz + size) / 2) - 1;
	if ((sr - pocz) > 0) merge_sort(tab, pocz, sr + 1, b);
	if ((size - sr) > 2) merge_sort(tab, sr + 1, size, b);
	int i = pocz;
	int j = sr + 1;
	for (int k = pocz; k < size; k++)
	{
		if (((i <= sr) && (j >= size)) || (((i <= sr) && (j < size)) && (tab[i] <= tab[j])))
		{
			b[k] = tab[i];
			i = i + 1;
		}
		else
		{
			b[k] = tab[j];
			j = j + 1;
		}
	}
	for (int k = pocz; k < size; k++)
		tab[k] = b[k];
}

void insert_sort(unsigned int data[], unsigned int n)
{
	for (unsigned int i = 1; i<n; i++)
	{
		unsigned int key = data[i];
		unsigned int j = i - 1;
		while (j >= 0 && data[j] > key)
		{
			data[j + 1] = data[j];
			data[j] = key;
			j -= 1;
		}
	}
}

void selection_sort(unsigned int data[], unsigned int n)
{
	for (unsigned int i = n - 1; i >= 1; i--)
	{
		unsigned int maxi = i;
		for (int j = i - 1; j >= 0; j--)
		{
			if (data[j] > data[maxi])
			{
				maxi = j;
			}
		}
		swap(data[i], data[maxi]);
	}
}

void resetuj_tablice(unsigned int glowna[], unsigned int temp[], unsigned int n)
{
	for (unsigned int i = 0; i < n; i++)
	{
		temp[i] = glowna[i];
	}
}

void przesun_tablice(unsigned int data[], unsigned int n)
{
	for (unsigned int i = n; i > 0; i--)
	{
		data[i] = data[i - 1];
	}
}

int main()
{
	srand(time(NULL));
	unsigned int B[MAX + 1];
	unsigned int losowa[MAX + 1], temp[MAX + 1];   
												   
	double selection_time = 0, insert_time = 0, merge_time = 0, heap_time = 0, quick_time = 0;

	unsigned int n = A;   

	ofstream wyj;   
	wyj.open("wyniki.txt");     
	wyj << "Wyniki pomiarow czasu wykonywania algorytmow sortowania:" << endl << endl << endl << "Czas sortowania dla:" << endl << endl;

	

	wyj << "a) losowego zestawu danych:" << endl << endl;

	losuj(losowa, MAX); 

	n = A;      

	wyj << "2) selection-sort: " << endl;
	for (short i = 0; i<15; i++)
	{
		resetuj_tablice(losowa, temp, n);
		clock_t begin = clock();
		selection_sort(temp, n);
		clock_t end = clock();
		selection_time = double(end - begin) / CLOCKS_PER_SEC;
		wyj << "pomiar dla " << n << " elementow: " << selection_time << "s" << endl;
		n += A;
	}

	n = A;

	n = A;

	

	wyj << endl << "b) rosnacego zestawu danych:" << endl << endl;

	losujRosnac(losowa, MAX);


	n = A;

	wyj << "2) selection-sort: " << endl;
	for (short i = 0; i<15; i++)
	{
		resetuj_tablice(losowa, temp, n);
		clock_t begin = clock();
		selection_sort(temp, n);
		clock_t end = clock();
		selection_time = double(end - begin) / CLOCKS_PER_SEC;
		wyj << "pomiar dla " << n << " elementow: " << selection_time << "s" << endl;
		n += A;
	}

	n = A;

	

	wyj << endl << "c) malejacego zestawu danych:" << endl << endl;

	losujMalejac(losowa, MAX);


	n = A;

	wyj << "2) selection-sort: " << endl;
	for (short i = 0; i<15; i++)
	{
		resetuj_tablice(losowa, temp, n);
		clock_t begin = clock();
		selection_sort(temp, n);
		clock_t end = clock();
		selection_time = double(end - begin) / CLOCKS_PER_SEC;
		wyj << "pomiar dla " << n << " elementow: " << selection_time << "s" << endl;
		n += A;
	}

	n = A;

	

	wyj << endl << "d) v-ksztaltnego zestawu danych:" << endl << endl;

	losujV(losowa, MAX);

	n = A;

	wyj << "2) selection-sort: " << endl;
	for (short i = 0; i<15; i++)
	{
		resetuj_tablice(losowa, temp, n);
		clock_t begin = clock();
		selection_sort(temp, n);
		clock_t end = clock();
		selection_time = double(end - begin) / CLOCKS_PER_SEC;
		wyj << "pomiar dla " << n << " elementow: " << selection_time << "s" << endl;
		n += A;
	}

	

	wyj << endl << "e) stalego zestawu danych:" << endl << endl;

	losujStale(losowa, MAX);

	n = A;

	wyj << "2) selection-sort: " << endl;
	for (short i = 0; i<15; i++)
	{
		resetuj_tablice(losowa, temp, n);
		clock_t begin = clock();
		selection_sort(temp, n);
		clock_t end = clock();
		selection_time = double(end - begin) / CLOCKS_PER_SEC;
		wyj << "pomiar dla " << n << " elementow: " << selection_time << "s" << endl;
		n += A;
	}

	wyj.close();        
	return 0;
}
