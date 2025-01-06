//This project files includes:
#include "Stack.h"

inline bool hasFlag(uint32_t flags, uint32_t flag)
{
	return flags & flag;
}

template <class elementsType, uint32_t flags, unsignedIntegerType elementsMaxNumMinType, elementsMaxNumMinType maxNumOfElements>
class Stack
{
	//unfixed size, overflow check, underflow check
	elementsType* pElements;
	elementsMaxNumMinType indexOfFutureElement{ 0 };
public:
	Stack()
	{
		pElements = new elementsType[maxNumOfElements];
	}

	//&?
	void push(elementsType element)
	{
		if constexpr (hasFlag(flags, NO_OVERFLOW_CHECK))
		{

		}
		if constexpr (indexOfFutureElement < maxNumOfElements)
		{
			pElements[indexOfFutureElement] = element;
			indexOfFutureElement++;
			return true;
		}
		else
		{
			return false;
		}
	}

	void pop()
	{
		if constexpr ()
			indexOfFutureElement--;
	}

	~Stack()
	{

	}
};