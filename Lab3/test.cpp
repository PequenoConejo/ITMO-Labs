#include "pch.h"
#include "../lab3/lab3.h"

#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

static const auto lowerIntComp = [](int a, int b) {return a < b; };
static const auto greaterIntComp = [](int a, int b) {return a > b; };
static const auto lowerFloatComp = [](float a, float b) {return a < b; };
static const auto greaterFloatComp = [](float a, float b) {return a > b; };
static const auto lowerStringComp = [](std::string a, std::string b) {return a < b; };
static const auto greaterStringComp = [](std::string a, std::string b) {return a > b; };
const int arraySize = 50;


// ������� �� ������ ��������
// �����
TEST(QuickSort, IntOneArray) {
	int a[1] = { 10 };
	QuickSort(a, a, lowerIntComp);
	ASSERT_EQ(a[0], 10);
}

// ������������
TEST(QuickSort, FloatOneArray) {
	float a[1] = { 7.5 };
	QuickSort(a, a, lowerFloatComp);
	ASSERT_EQ(a[0], 7.5);
}

// ������
TEST(QuickSort, StringOneArray) {
	string a[1] = { "string" };
	QuickSort(a, a, lowerStringComp);
	ASSERT_EQ(a[0], "string");
}

// ��������������� �������
// �����
TEST(QuickSort, IntSortedArray) {
	int a[arraySize];
	for (int i = 0; i < arraySize; i++)
		a[i] = i;
	QuickSort(a, a+arraySize-1, lowerIntComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] <= a[i + 1]);
}

// ������������ 
TEST(QuickSort, FloatSortedArray) {
	float a[arraySize];
	for (int i = 0; i < arraySize; i++)
		a[i] = i+(float)i*0.01;
	QuickSort(a, a + arraySize - 1, lowerFloatComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] <= a[i + 1]);
}

// ������
TEST(QuickSort, StringSortedArray) {
	string a[10] = { "Aa", "Ab", "Ac", "Ad", "Ae", "Af", "Ag", "Ah", "Ai", "Aj" };
	QuickSort(a, a + 9, lowerStringComp);
	for (int i = 0; i < 9; i++)
		ASSERT_TRUE(a[i] <= a[i + 1]);
}

// ��������������� �������
// �����
TEST(QuickSort, IntArray) {
	srand(unsigned(time(0)));
	int a[arraySize];
	for (int i = 0; i < arraySize; i++)
		a[i] = rand()%(arraySize*2);
	QuickSort(a, a + arraySize - 1, lowerIntComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] <= a[i + 1]);
}


// ������������ 
TEST(QuickSort, FloatArray) {
	srand(unsigned(time(0)));
	float a[arraySize];
	for (int i = 0; i < arraySize; i++)
		a[i] = rand() % (arraySize * 2) + 0.01*i;
	QuickSort(a, a + arraySize - 1, lowerFloatComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] <= a[i + 1]);
}

// ������
TEST(QuickSort, StringArray) {
	srand(unsigned(time(0)));
	string a[arraySize];
	for (int i = 0; i < arraySize; i++) {
		a[i] = 'a' + rand() % 26;
		a[i] += 'a' + rand() % 26;
	}
	QuickSort(a, a + arraySize - 1, lowerStringComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] <= a[i + 1]);
}

// ���������� � ������ �������
TEST(QuickSort, IntGreaterArray) {
	srand(unsigned(time(0)));
	int a[arraySize];
	for (int i = 0; i < arraySize; i++)
		a[i] = rand() % (arraySize * 2);
	QuickSort(a, a + arraySize - 1, greaterIntComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] >= a[i + 1]);
}


// ������������ 
TEST(QuickSort, FloatGreaterArray) {
	srand(unsigned(time(0)));
	float a[arraySize];
	for (int i = 0; i < arraySize; i++)
		a[i] = rand() % (arraySize * 2) + 0.01 * i;
	QuickSort(a, a + arraySize - 1, greaterFloatComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] >= a[i + 1]);
}

// ������
TEST(QuickSort, StringGreaterArray) {
	srand(unsigned(time(0)));
	string a[arraySize];
	for (int i = 0; i < arraySize; i++) {
		a[i] = 'a' + rand() % 26;
		a[i] += 'a' + rand() % 26;
	}
	QuickSort(a, a + arraySize - 1, greaterStringComp);
	for (int i = 0; i < arraySize - 1; i++)
		ASSERT_TRUE(a[i] >= a[i + 1]);
}


