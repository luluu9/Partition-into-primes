// Podziały.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define MAX_PRIME 1 << 8 // byte = 8 bits

template <class T>
void generatePrimes(T* tab, int maxNumber, unsigned int* primesNumber) {
	int index = 0;
	for (int toCheck = 2; toCheck <= maxNumber; toCheck++) {
		bool isPrime = true;
		for (int checker = 2; checker <= toCheck / 2; checker++) {
			if (toCheck % checker == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			tab[index] = toCheck;
			index++;
		}
	}
	*primesNumber = index;
}

template <class T>
void printTab(T* tab, int lastIndex) {
	for (int i = 0; i < lastIndex; i++) {
		printf("%d, ", tab[i]);
	}
	printf("\n");
}

// wypelniam tablice najmniejszymi lp (czyli 2) az numberToPartition odjąć suma tablicy
// będzie mniejsza niż dwa. 
// jeżeli to odejmowanie == 0, to mam pierwszego kandydata,
// w innym przypadku usuwam dwójkę i wstawiam 3
// jak to uogólnić?
// zabieram dwie z lewej
// i dopasowuje najmniejszą (ale większe od zabranej) która też pasuje
// jeżeli jakaś pasuje, ale nie daje odpowiedniej sumy, zabieram kolejną (ale nie tą, którą dostawiłem)
// teraz pozwalam na wrzucanie wszystkich liczb pierwszych (także mniejszych od zabranej)
// jednak szukam tej liczb od największej (aktualnie wrzuconej) do najmniejszej pierwszej
// 
// tym algorytmem chyba nie znajde 5+5
// 
// jeżeli żadna nie pasuje, zabieram pierwszą z lewej

// nowy plan:
// trochę brute force, ale wydaje się działać:
// np. dla 17-7:
// 10 = 7 + 3 (biore najwieksze i probuje dopasowac najwieksze mozliwe, jak dopasowanie dziala zaraz pokaze)
// teraz zabieram z prawej i probuje dopasowac mniejsze od zabranej, czyli 3
// 10 = 7 + 2 (wklepuje 2, bo jako jedyne pasuje mniejsze)
// reszta = 1, więc zabieram dwójkę i próbuje dopasować mniejsze od zabranej
// nie ma takiej, dlatego zabieram kolejną, i próbuje dopasować mniejszą od niej, czyli 5<7:
// 10 = 5, największa pasująca to 5, więc wkładam
// 10 = 5 + 5, teraz zabieram z prawej piatke i wkladam mniejsze:
// 10 = 5 + 3 (r. 2 więc dokladam 2)
// 10 = 5 + 3 + 2 // r=0, więc zabieram od prawej
// 10 = 5 + 3 (nie ma lp mniejszej od 2, więc zabieram 3)
// 10 = 5 + 2 + 2 (r=1, więc zabieram)
// 10 = 5 + 2
// 10 = 5
// 10 = 3 + 3 + 3 (r=1)

template <class T>
int findPrime(unsigned int num, T* primesArray, unsigned int primesNumber) {
	int l = 0, p = primesNumber - 1;
	while (l < p) {
		unsigned int mid = (l + p) / 2;
		if (primesArray[mid] < num)
			l = mid + 1;
		else if (primesArray[mid] > num)
			p = mid - 1;
		else
			return mid;
	}
	if (primesArray[l] == num)
		return l;
	return -1;
}


template <class T>
int findFromEnd(unsigned int num, T* array, unsigned int arrayEndIndex) {
	for (int i = arrayEndIndex; i >= 0; i--)
		if (array[i] == num)
			return i;
	return -1;
}

template <class T>
int findClosestTo(int num, T* array, unsigned int arrayEndIndex) {
	if (arrayEndIndex == -1) return -1;
	for (int i = arrayEndIndex; i >= 0; i--)
		if (num - array[i] >= 0)
			return i;
	return -1;
}

void printPartition(unsigned int k, unsigned int* partition, unsigned int partNumber) {
	printf("%u+", k);
	for (unsigned int i = 0; i < partNumber - 1; i++) {
		printf("%u+", partition[i]);
	}
	printf("%u\n", partition[partNumber - 1]);
}


template <class T>
void removeLastNumberFromPartition(
	unsigned int &partIndex, 
	unsigned int* partition, 
	int &largestPrimeIndex, 
	unsigned int &rest,
	T* primesArray, 
	unsigned int primesNumber
	) 
{
	partIndex--;
	unsigned int lastPrime = partition[partIndex]; // pop last value
	partition[partIndex] = 0;
	// set largestPrime to be smaller than (but closest to) popped
	largestPrimeIndex = findClosestTo(lastPrime-1, primesArray, primesNumber); 
	rest += lastPrime;
}

template <class T>
void findPartitions(unsigned int n, unsigned int k, T* primesArray, unsigned int primesNumber) {
	int numberToPartition = n - k;
	int primeMaxIndex = findPrime(k, primesArray, primesNumber);
	if (primeMaxIndex == -1) return; // k is not a prime, skip

	unsigned int partition[MAX_PRIME] = { }, partIndex = 0, rest = numberToPartition;
	int largestPrimeIndex = primeMaxIndex;

	// special cases
	if (k == 2) {
		if (numberToPartition % 2 == 1)
			return;
	}
	if (numberToPartition < 2)
		return;

	while (rest != 0) {
		// znajdujemy najwieksza mieszczaca sie w reszcie
		largestPrimeIndex = findClosestTo(rest, primesArray, largestPrimeIndex);
		if (largestPrimeIndex == -1) { // rest is equal to 1
			if (partition[0] == 2) // all of combinations is used
				break;
			removeLastNumberFromPartition(partIndex, partition, largestPrimeIndex, rest, primesArray, primesNumber);
			continue;
			//break;
		}
		partition[partIndex] = primesArray[largestPrimeIndex];
		partIndex++;
		rest -= primesArray[largestPrimeIndex];

		if (rest == 0) {
			printPartition(k, partition, partIndex);
			removeLastNumberFromPartition(partIndex, partition, largestPrimeIndex, rest, primesArray, primesNumber);
			if (largestPrimeIndex == -1) { // there is no smaller, end this try, maybe while?
				// i need to figure out another condition, bcs if last digit is 2
				// itll break
				if (partIndex == 0) // k + 2 or 3 is the only solution
					break;
			}
		}
	}
}


int main() {

	unsigned int primesNumber;
	unsigned short primes[MAX_PRIME];
	generatePrimes(primes, MAX_PRIME, &primesNumber);
	printTab(primes, primesNumber);

	unsigned short partitionsToDo, n, k;
	scanf_s("%hu", &partitionsToDo);

	for (int i = 0; i < partitionsToDo; i++) {
		scanf_s("%hu %hu", &n, &k);
		findPartitions(n, k, primes, primesNumber);
	}

}

