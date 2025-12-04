#pragma once
#include <iostream>

/*
	Makes a class printable by overloading the << operator.
	Derived classes implement the print() function to define their own printing behavior.
*/
class IPrintable
{
	friend std::ostream& operator<<(std::ostream& os, const IPrintable& obj);
public:
	virtual void print(std::ostream& os) const = 0;
	virtual ~IPrintable() = default;
};

