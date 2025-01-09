template <typename ELEMENT_TYPE>
struct Node
{
	ELEMENT_TYPE element;
	Node<ELEMENT_TYPE>* pNextNode;

	Node(ELEMENT_TYPE& argElement)
	{
		element = argElement;
	}
};

template <typename ELEMENT_TYPE>
class ListExample
{
	Node<ELEMENT_TYPE>* pFirstNode;



public:

	ListExample()
	{
		pFirst = nullptr;
	}

	void Push(ELEMENT_TYPE& element)
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

	}

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