#pragma once

//C++ standart includes:
#include <concepts>

enum STACK_FLAGS
{
	FIXED_SIZE = 0b00000000'00000000'00000000'00000001,
	NO_OVERFLOW_CHECK = 0b00000000'00000000'00000000'00000010,
	NO_UNDERFLOW_CHECK = 0b00000000'00000000'00000000'00000100
};

template <class elementsType>
concept unsignedIntegerType = std::integral<elementsType> && std::is_unsigned_v<elementsType>;

template <class elementsType, uint32_t flags, unsignedIntegerType elementsMaxNumMinType = bool, elementsMaxNumMinType maxNumOfElements = 100>
class Stack;