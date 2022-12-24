// lab2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

template <typename T> 
class MyArray final {
    private:
        T* pArr;
        int size; // именно текущий размер
        int capacity; // общая вместимость
    public:
        // конструктор по умолчанию
        MyArray() { 
            pArr = (T*)malloc(16 * sizeof(T));
            capacity = 16;
            size = 0;
        };
        // конструктор с заданной вместимостью
        MyArray(int setCapacity) {
            pArr = (T*)malloc(capacity * sizeof(T));
            capacity = setCapacity;
            size = 0;
        };
        // деструктор
        ~MyArray() {
            for (int i = 0; i < size; i++)
                pArr[i].~T();
            free(pArr);
        };
        // копирующий конструктор
        MyArray(const MyArray& other) {
            pArr = (T*)malloc(other.size * sizeof(*pArr));
            for (int i = 0; i < other.size; i++)
                new(pArr + i) T(other.pArr[i]);
            size = other.size;
            capacity = other.capacity;
        };
        // перемещающий конструктор
        MyArray(MyArray&& other) {
            pArr = other.pArr;
            size = other.size;
            capacity = other.capacity;
            other.capacity = 0;
            other.size = 0;
            other.pArr = nullptr;
        };
        // своп
        void swap(MyArray& other) {
            std::swap(pArr, other.pArr);
            std::swap(size, other.size);
            std::swap(capacity, other.capacity);
        };
        // оператор присваивания на основе copy-swap
        MyArray& operator=(MyArray other) {
            swap(other);
            return *this;
        };
        //// перемещающий оператор присваивания
        //MyArray& operator=(MyArray&& other) {
        //    if (this != &other) {
        //        pArr = other.pArr;
        //        size = other.size;
        //        capacity = other.capacity;
        //        other.capacity = 0;
        //        other.size = 0;
        //        other.pArr = nullptr;
        //    }
        //    return *this;
        //};
        // геттеры
        T* getPointer() {
            return pArr;
        };
        int getSize() {
            return size;
        };
        int getCapacity() {
            return capacity;
        };
        // квадратные скобочки
        const T& operator[](int index) const {
            return pArr[index];
        };
        T& operator[](int index) {
            return pArr[index];
        };
        // вставка в конец массива
        int insert(const T& value) {
            // надо проверить а есть ли куда вставлять?
            if (size>=capacity)
            { 
                // занимаем память под массив с учетом нового элемента
                capacity *= 2;
                T* pTemp = (T*)malloc(capacity * sizeof(T));
                // переносим в нее все что было
                for (int i = 0; i < size; i++) {
                    new (pTemp + i) T(move(pArr[i]));
                    pArr[i].~T();
                }
                // очищаем старую память
                free(pArr);
                pArr = pTemp;
                pTemp = nullptr;
 
            }
            // в любом случае вставляем элемент в конец массива
            new (pArr + size) T(value);
            size=size+1;
            // возвращаем нужный индекс
            return (size - 1);
        };

        // вставка по индексу
        int insert(const T& value, int index) {
            // если мы хотим вставить куда-то за пределы текущей вместимости или текущего размера 
            // просто вставляем в конец
            if (index >= capacity||index>=size) {
                index = insert(value);
            }
            // если вставляем в середину 
            else {
                // есть ли место?
                if (size == capacity)
                    capacity *= 2;
                // все элементы правее сдвигаем на одну позицию влево
                new (pArr + size) T(move(pArr[size - 1]));
                for (int i = size - 1; i > index; i--) {
                    pArr[i] = move(pArr[i - 1]);
                }
                // элементу в нужном месте присваиваем нужное значение
                pArr[index] = value;
                size++;
            }
            // и возвращаем место куда вставляли
            return index;
        };
        // удаление по индексу
        void remove(int index) {
            // сдвигаем все правее на одну позицию
            for (int i = index; i < size - 1; i++) {
                pArr[i] = move(pArr[i + 1]);
            }
            // последний элемент теперь не нужен
            pArr[size - 1].~T();
            size--;
        };
        // итератор
        public:
            // обычный
            class MyIterator {
            private:
                MyArray<T>* head;
                T* current;
                bool isReversed;
                int step;
            public:
                MyIterator(MyArray<T>* array, bool reversed) {
                    head = array;
                    isReversed = reversed;
                    if (isReversed) {
                        step = -1;
                        current = (array->pArr + (array->size - 1));
                    }
                    else {
                        step = 1;
                        current = array->pArr;
                    }
                };
                const T& get() const {
                    return *current;
                };
                void next() {
                    current += step;
                };
                bool hasNext() const {
                    
                    if (isReversed)
                        return(current > head->pArr);
                    else
                        return (current < (head->pArr + (head->size - 1)));
                };
                void set(const T& value) {
                    *current = value;
                };
            };
            // константный
            class MyConstIterator {
            private:
                MyArray<T>* head;
                T* current;
                bool isReversed;
                int step;
            public:
                MyConstIterator(MyArray<T>* const array, bool reversed) {
                    head = array;
                    isReversed = reversed;
                    if (isReversed) {
                        step = -1;
                        current = (array->pArr + (array->size - 1));
                    }
                    else {
                        step = 1;
                        current = array->pArr;
                    }
                };
                const T& get() const {
                    return *current;
                };
                void next() {
                    current += step;
                };
                bool hasNext() const {

                    if (isReversed)
                        return(current > head->pArr);
                    else
                        return (current < (head->pArr + (head->size - 1)));
                };
            };
    // получение итераторов
    public:
        MyIterator iterator() {
            return MyIterator(this, false);
        };
        MyConstIterator iterator() const {
            return MyConstIterator(this, false);
        };
        MyIterator reverseIterator() {
            return MyIterator(this, true);
        };
        MyConstIterator reverseIterator() const {
            return MyConstIterator(this, true);
        };
    
};

//int main()
//{
//    /*MyArray<int> newArray;
//    for (int i = 0; i < 10; ++i) {
//        newArray.insert(i + 1);
//    }
//    auto iter = newArray.iterator();
//    for (int i = 0; i < newArray.getSize(); i++) {
//        cout << iter.get() << endl;
//        iter.next();
//    }*/
//
//}

