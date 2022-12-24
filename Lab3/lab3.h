#include <iostream>
#include <windows.h>
#include <fstream>
#include <time.h>

using namespace std;

// экспериментально найденное значение, где
// сортировка вставками становится быстрее
constexpr int valueToUseInsertionSort = 17;

// нахождение медианы трех значений
template<typename T, typename Compare>
T FindMedian(T& a, T& b, T& c, Compare compare) {
    // c < a < b или b < a < c
    if ((compare(a, b) && compare(c, a)) || ((compare(a, c) && compare(b, a)))) { 
        return a;
    }
    // c < b < a или a < b < c
    else if ((compare(b, a) && compare(c, b)) || (compare(b, c) && compare(a, b))) { 
        return b;
    }
    else {
        return c;
    }
}

// сортировка вставками
template<typename T, typename Compare>
void InsertionSort(T* first, T* last, Compare comp) {
    if (first == last) { return; }
    T iValue;
    T* i;
    for (T* l = first + 1; l <= last; l++) {
        iValue = (*l);
        i = l;
        while (i > first && comp(iValue, *(i - 1))) {
            (*i) = move(*(i - 1));
            i--;
        }
        (*i) = move(iValue);
    }
}

// быстрая сортировка
// по умолчанию использует улучшение со вставками
template<typename T, typename Compare>
void QuickSort(T* first, T* last, Compare compare, bool useInsertionSort = true) {
    if (last <= first) return;
    // if (first == NULL || last == NULL) return;
    while (first < last) {
        // используем ли сортировку вставками?
        if (useInsertionSort) {
            // если ее эффективно использовать
            if ((last - first) < valueToUseInsertionSort) {
                InsertionSort(first, last, compare);
            }
        }
        // выбираем опорный элемент по медиане
        T pivot = FindMedian(*first, *last, *(first + ((last - first) / 2)), compare);
        T* left = first;
        T* right = last;
        // пробежать массив, выбирая элементы, которые нужно перенести в другую часть
        while (true) {
            while (compare(*left, pivot)) {
                left++;
            }
            while (compare(pivot, *right)) {
                right--;
            }
            if (left >= right) {
                break;
            }
            swap(*left, *right);
            left++;
            right--;
        }
        // выбираем меньшую часть, которую сортируем рекурсивно
        if (last - right > right - first) {
            QuickSort(first, right, compare, useInsertionSort);
            // сдвигаем правую границу сортировки для следующей итерации
            first = right + 1;
        }
        else {
            QuickSort(right + 1, last, compare, useInsertionSort);
            // сдвигаем левую границу для следующей итерации
            last = right;
        }
    }
}

// замер времени выполнения
double PCFreq = 0.0;
__int64 CounterStart = 0;
void counterStart() {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    PCFreq = double(li.QuadPart / 1000000.0);
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double counterEnd() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

// для определения величины массива, где сортировка вставками станет быстрее
int main()
{
    // цикл по разным размерам массива
    for (int i = 0; i < 50; i++) {
        double insertTimeAll = 0;
        double quickTimeAll = 0;
        // внутри цикл по количеству тестовых запусков
        for (int j = 0; j < 100; j++) {
            // создаем массивы, заполняем
            int* insertSortArray = new int[i];
            int* quickSortArray = new int[i];
            for (int k = 0; k < i; k++) {
                insertSortArray[k] = i - k; // худший случай для сортировки вставками
                quickSortArray[k] = i - k;
            }
            // расчет времени
            counterStart();
            InsertionSort(insertSortArray, insertSortArray + i - 1, [](int a, int b) { return a < b; });
            double insertTime = counterEnd();
            counterStart();
            QuickSort(quickSortArray, quickSortArray + i - 1, [](int a, int b) { return a < b; }, false);
            double quickTime = counterEnd();
            insertTimeAll += insertTime;
            quickTimeAll += quickTime;
            // очистить память
            delete[] quickSortArray;
            delete[] insertSortArray;
        }
        insertTimeAll /= 100;
        quickTimeAll /= 100;
        cout << "Array size " << i << endl;
        cout << "Insertion sort time " << insertTimeAll << endl;
        cout << "Quick sort time " << quickTimeAll << endl;
        if (quickTimeAll > insertTimeAll)
            cout << "     Insertion faster" << endl;
        else cout << "     Quick faster" << endl;
        cout << endl;
    }

    return 0;
}