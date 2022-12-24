#include "pch.h"
#include "../lab2/lab2.h"

const int arraySize = 20;

// конструкторы
TEST(ArrayConstruct, Default) {
	MyArray<int> a;
	ASSERT_EQ(a.getSize(), 0);
	ASSERT_EQ(a.getCapacity(), 16);
}

TEST(ArrayConstruct, Sized) {
	MyArray<int> a(arraySize);
	ASSERT_EQ(a.getSize(), 0);
	ASSERT_EQ(a.getCapacity(), arraySize);
}

TEST(ArrayConstruct, Copy) {
	MyArray<int> a(arraySize);
	a.insert(10);
	MyArray<int> b(a);
	ASSERT_EQ(b.getSize(), a.getSize());
	ASSERT_EQ(b.getCapacity(), a.getCapacity());
}

TEST(ArrayConstruct, Move) {
	MyArray<int> a(arraySize);
	a.insert(10);
	MyArray<int> b(move(a));
	ASSERT_EQ(a.getSize(), 0);
	ASSERT_EQ(b.getSize(), 1);
}

// оператор

TEST(Operator, Copy) {
	MyArray<int> a(arraySize);
	a.insert(10);
	MyArray<int> b(arraySize);
	b.insert(15);
	b.insert(20);
	b = a;
	ASSERT_EQ(b.getSize(), 1);
	ASSERT_EQ(b[0], 10);
	ASSERT_EQ(a.getSize(), 1);
	ASSERT_EQ(a[0], 10);
}

// работа с элементами

TEST(Elements, Insert) {
	MyArray<int> a(arraySize);
	for (int i = 0; i < arraySize; i++)
		a.insert(i);
	for (int i = 0; i < arraySize; i++)
		ASSERT_EQ(a[i],i);
}

TEST(Elements, Change) {
	MyArray<int> a(arraySize);
	for (int i = 0; i < arraySize; i++)
		a.insert(i);
	for (int i = 0; i < arraySize; i++)
		a[i] += 5;
	for (int i = 0; i < arraySize; i++)
		ASSERT_EQ(a[i], i+5);
}

TEST(Elements, InsertOverCapacity) {
	 MyArray<int> a(arraySize);
	for (int i = 0; i < arraySize; i++)
		a.insert(i);
	a.insert(15);
	ASSERT_EQ(a[arraySize], 15);
	a.insert(20, 5);
	for (int i = 0; i < a.getSize(); i++)
		cout << a[i] << " ";
	ASSERT_EQ(a.getCapacity(), arraySize*2);
	ASSERT_EQ(a.getSize(), arraySize + 2);
}

TEST(Elements, InsertIndex) {
	MyArray<int> a(arraySize);
	for (int i = 0; i < arraySize; i++)
		a.insert(i);
	a.insert(15, 5);
	ASSERT_EQ(a[5], 15);
	ASSERT_EQ(a.getCapacity(), arraySize * 2);
	ASSERT_EQ(a.getSize(), arraySize + 1);
}

TEST(Elements, Remove) {
	MyArray<int> a(arraySize);
	for (int i = 0; i < arraySize; i++)
		a.insert(i);
	a.remove(5);
	ASSERT_EQ(a.getSize(), arraySize - 1);
}

// итераторы

TEST(Iterator, Iterator) {
	MyArray<int> a(arraySize);
	for (int i = 0; i < arraySize; i++)
		a.insert(i);
	auto iter = a.iterator();
	for (int i = 0; i < arraySize; i++) {
		ASSERT_EQ(iter.get(), i);
		iter.next();
	}
}

TEST(Iterator, Reversed) {
	MyArray<int> a(arraySize);
	for (int i = 0; i < arraySize; i++)
		a.insert(i);
	auto iter = a.reverseIterator();
	for (int i = 0; i < arraySize; i++) {
		ASSERT_EQ(iter.get(), arraySize-i-1);
		iter.next();
	}
}

TEST(Iterator, Set) {
	MyArray<int> a(arraySize);
	auto iter = a.iterator();
	for (int i = 0; i < arraySize; i++) {
		iter.set(i);
		iter.next();
	}
	for (int i = 0; i < arraySize; i++)
		ASSERT_EQ(a[i],i);
}

// массив строк

TEST(StringTest, Construct) {
	MyArray<std::string> a(arraySize);
	a.insert("String", 0);
	ASSERT_EQ(a.getCapacity(), arraySize);
	ASSERT_EQ(a.getSize(), 1);
	ASSERT_EQ(a[0], "String");
}

TEST(StringTest, Insert) {
	MyArray<std::string> a(arraySize);
	for (int i = 0; i <= arraySize; i++)
		a.insert("StringStringStringStringStringStringStringStringStringStringStringStringString");
	a.insert("StringStringStringStringStringStringStringStringStringStringStringStringString");
	ASSERT_EQ(a.getCapacity(), arraySize*2);
	ASSERT_EQ(a[a.getSize()-1], "StringStringStringStringStringStringStringStringStringStringStringStringString");
}

TEST(StringTest, Iterator) {
	MyArray<std::string> b(arraySize);
	auto iter = b.iterator();
	for (int i = 0; i < arraySize; i++)
		b.insert("String");
	for (int i = 0; i < arraySize; i++) {
		iter.set("StringStringStringStringStringStringStringStringStringStringStringStringString");
		iter.next();
	}
	ASSERT_EQ(b.getCapacity(), arraySize);
	ASSERT_EQ(b[arraySize-1], "StringStringStringStringStringStringStringStringStringStringStringStringString");
	auto reti = b.reverseIterator();
	for (int i = 0; i < arraySize; i++) {
		ASSERT_EQ(reti.get(), "StringStringStringStringStringStringStringStringStringStringStringStringString");
		reti.next();
	}
}