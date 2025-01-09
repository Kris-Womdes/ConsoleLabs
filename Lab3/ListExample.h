#pragma once

template <typename ELEMENT_TYPE>
struct Node
{
	ELEMENT_TYPE element;
	Node<ELEMENT_TYPE>* pNextNode;

	Node(ELEMENT_TYPE& argElement)
	{
		element = argElement;
		pNextNode = nullptr;
	}
};

template <typename ELEMENT_TYPE>
class ListExample
{
	Node<ELEMENT_TYPE>* pFirstNode;



public:

	ListExample()
	{
		pFirstNode = nullptr;
	}

	void Push(ELEMENT_TYPE element)
	{
		if (pFirstNode)
		{
			Node<ELEMENT_TYPE>* pGuessingLastNode = pFirstNode;
			while (pGuessingLastNode->pNextNode)
			{
				pGuessingLastNode = pGuessingLastNode->pNextNode;
			}
			pGuessingLastNode->pNextNode = new Node<ELEMENT_TYPE>(element);
		}
		else
		{
			pFirstNode = new Node<ELEMENT_TYPE>(element);
		}
	}

	bool Pop(ELEMENT_TYPE& element)
	{
		if (pFirstNode)
		{
			Node<ELEMENT_TYPE>* pLastNode = pFirstNode;
			unsigned int indexLastNode{ 0 };
			while (pLastNode->pNextNode)
			{
				pLastNode = pLastNode->pNextNode;
				++indexLastNode;
			}
			element = pLastNode->element;
			delete pLastNode;
			if (indexLastNode)
			{
				pLastNode = pFirstNode;
				while (indexLastNode - 1)
				{
					pLastNode = pLastNode->pNextNode;
					--indexLastNode;
				}
				pLastNode->pNextNode = nullptr;
			}
			else
			{
				pFirstNode = nullptr;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	friend void sortSelection(ListExample<ELEMENT_TYPE>& list);

	~ListExample()
	{
		while (pFirstNode)
		{
			Node<ELEMENT_TYPE>* pNextNode = pFirstNode->pNextNode;
			delete pFirstNode;
			pFirstNode = pNextNode;
		}
	}
};

template <typename ELEMENT_TYPE>
void sortSelection(ListExample<ELEMENT_TYPE>& list)
{
	if (list.pFirstNode)
	{
		Node<ELEMENT_TYPE>* pNode_min = { list.pFirstNode->element };
		for (pNode{ list.pFirstNode }; pNode->pNextNode; pNode = pNode->pNextNode)
		{
			if (pNode->pNextNode->element < pNode_min->element)
			{
				pNode_min = pNode->pNextNode;
			}
		}
	}
}