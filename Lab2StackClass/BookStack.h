#pragma once

//C++ standart includes:
#include <stdexcept>

#define MAX_BLOCK_AMOUNT sizeof(MAX_BLOCK_AMOUNT_TYPE)
#define MAX_ELEMENT_AMOUNT_PER_BLOCK sizeof(MAX_ELEMENT_AMOUNT_PER_BLOCK_TYPE)

template <typename ELEMENT_TYPE>
class BookStack
{
	typedef unsigned char MAX_BLOCK_AMOUNT_TYPE;
	ELEMENT_TYPE** ppElement;
	MAX_BLOCK_AMOUNT_TYPE indexOfCurrentBlock;

	typedef unsigned char MAX_ELEMENT_AMOUNT_PER_BLOCK_TYPE;
	MAX_ELEMENT_AMOUNT_PER_BLOCK_TYPE indexOfFutureElementInCurrentBlock;

	//must have the size of sizeof(MAX_BLOCK_AMOUNT_TYPE + MAX_ELEMENT_AMOUNT_PER_BLOCK_TYPE)
	typedef int16_t MAX_ELEMENT_TOTAL_AMOUNT_TYPE;



public:
	BookStack()
	{
		ppElement = new ELEMENT_TYPE * [MAX_BLOCK_AMOUNT];
		indexOfCurrentBlock = 0;

		ppElement[indexOfCurrentBlock] = new ELEMENT_TYPE[MAX_ELEMENT_AMOUNT_PER_BLOCK];
		indexOfFutureElementInCurrentBlock = 0;
	}

	void push(const ELEMENT_TYPE& const element)
	{
		ppElement[indexOfCurrentBlock][indexOfFutureElementInCurrentBlock] = element;
		if (indexOfFutureElementInCurrentBlock < MAX_ELEMENT_AMOUNT_PER_BLOCK - 1)
		{
			indexOfFutureElementInCurrentBlock++;
		}
		else
		{
			if (indexOfCurrentBlock < MAX_BLOCK_AMOUNT - 1)
			{
				indexOfCurrentBlock++;
				ppElement[indexOfCurrentBlock] = new ELEMENT_TYPE[MAX_ELEMENT_AMOUNT_PER_BLOCK];
				indexOfFutureElementInCurrentBlock = 0;
			}
			else
			{
				throw std::out_of_range("Failed to push the element to the stack: Failed to create a new block: The maximum amount of blocks was reached");
			}
		}
	}

	ELEMENT_TYPE& pop()
	{
		if (indexOfFutureElementInCurrentBlock > 0)
		{
			indexOfFutureElementInCurrentBlock--;
			return ppElement[indexOfCurrentBlock][indexOfFutureElementInCurrentBlock];
		}
		else
		{
			if (indexOfCurrentBlock > 0)
			{
				delete[] ppElement[indexOfCurrentBlock];
				indexOfCurrentBlock--;
				indexOfFutureElementInCurrentBlock = MAX_ELEMENT_AMOUNT_PER_BLOCK - 1;
			}
			else
			{
				throw std::out_of_range("Failed to pop the element from the stack: There are no elements left");
			}
		}
	}

	BookStack& operator<<(const ELEMENT_TYPE& const element)
	{
		push(element);
		return this;
	}

	BookStack& operator>>(ELEMENT_TYPE& const elementHolder)
	{
		elementHolder = pop();
		return this;
	}

	MAX_ELEMENT_TOTAL_AMOUNT_TYPE getTotalAmountOfElements()
	{
		return indexOfCurrentBlock * MAX_ELEMENT_AMOUNT_PER_BLOCK + indexOfFutureElementInCurrentBlock;
	}

	void sieve()
	{
		//более эффективный алгоритм (в разработке):
		/*MAX_ELEMENT_TOTAL_AMOUNT_TYPE currentTotalAmountOfElements{ getTotalAmountOfElements() };
		MAX_ELEMENT_TOTAL_AMOUNT_TYPE totalAmountOfElementsAfterSieve{ std::ceil(currentTotalAmountOfElements/2.) };
		MAX_BLOCK_AMOUNT_TYPE amountOfBlocksAfterSieve{ std::ceil(totalAmountOfElementsAfterSieve / (float)MAX_ELEMENT_AMOUNT_PER_BLOCK) };
		if (amountOfBlocksAfterSieve)
		{
			for (MAX_ELEMENT_AMOUNT_PER_BLOCK_TYPE elementCounter{ 0 }; elementCounter < ; ++elementCounter)
			{

			}
		}
		for (MAX_BLOCK_AMOUNT_TYPE blockCounter{ 1 }; blockCounter < amountOfBlocksAfterSieve-1; ++blockCounter)
		{
			for (MAX_ELEMENT_AMOUNT_PER_BLOCK_TYPE elementCounter{ 0 }; elementCounter < MAX_ELEMENT_AMOUNT_PER_BLOCK; ++elementCounter)
			{

			}
		}*/
		//last block not full may be for(){}
			
			
		/*for (MAX_ELEMENT_TOTAL_AMOUNT_TYPE elementCounter{ getTotalAmountOfElements() % 2 == 0 ? 1 : 0 }; elementCounter < indexOfFutureElementInCurrentBlock; ++elementCounter)
		{
			pop();
		}
		for (MAX_BLOCK_AMOUNT_TYPE blockCounter{ 1 }; blockCounter < (indexOfCurrentBlock + 1) / 2.; ++blockCounter)
		{
			for (MAX_ELEMENT_TOTAL_AMOUNT_TYPE elementCounter{ getTotalAmountOfElements() % 2 == 0 ? 1 : 0 }; elementCounter < MAX_ELEMENT_AMOUNT_PER_BLOCK; ++elementCounter)
			{
				pop();
			}
		}*/
	}

	~BookStack()
	{
		for (unsigned char blockCounter{ 0 }; blockCounter < indexOfCurrentBlock + 1; ++blockCounter)
		{
			delete[] ppElement[blockCounter];
		}
		delete[] ppElement;
	}
};
