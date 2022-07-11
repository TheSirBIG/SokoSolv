#pragma once
template<typename T> class openArray
{
public:
	openArray(int len = 0);
	~openArray();

	void put(T elem);
	T get(int idx);
	int getNum();
private:
	int minLen;
	int curIdx;
	struct baseStruct
	{
		T* mas;
		baseStruct* next;
		baseStruct* prev;
	};
	baseStruct* baseAddr;
	int elemSize;
};

template <typename T>
int openArray<T>::getNum()
{
	return curIdx;
}

template <typename T>
openArray<T>::openArray(int len)
{
	minLen = (len == 0) ? 100 : len;
	elemSize = sizeof(T);
	curIdx = 0;
	baseAddr = new baseStruct;
	baseAddr->next = baseAddr->prev = nullptr;
	baseAddr->mas = new T[minLen];
}

template <typename T>
openArray<T>::~openArray()
{
	baseStruct* bs;

	bs = baseAddr;
	while (bs->next != nullptr) bs = bs->next;
	while (bs != baseAddr)
	{
		delete bs->mas;
		bs = bs->prev;
		delete bs->next;
	}
	delete baseAddr->mas;
	delete baseAddr;
}

template<typename T>
void openArray<T>::put(T elem)
{
	baseStruct* bs;

	bs = baseAddr;
	for (int i = 0; i < curIdx / minLen; i++)
		bs = bs->next;
	bs->mas[curIdx % minLen] = elem;
	curIdx++;
	if ((curIdx % minLen) == 0)
	{
		//нужен следующий набор массива
		bs->next = new baseStruct;
		(bs->next)->prev = bs;
		(bs->next)->next = nullptr;
		(bs->next)->mas = new T[minLen];
	}
}

template<typename T>
T openArray<T>::get(int idx)
{
	baseStruct* bs;

	bs = baseAddr;
	for (int i = 0; i < curIdx / minLen; i++)
		bs = bs->next;
	return bs->mas[curIdx % minLen];
}
