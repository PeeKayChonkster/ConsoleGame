#pragma once
#include <string>

#define THROW_CGEXCEPTION(msg) throw(CGException(msg, __FILE__, __LINE__));

class CGException
{
public:
	std::wstring message;
	std::string file;
	int line;
public:
	CGException(std::wstring message, std::string file, int line) :
		message(message), file(file), line(line) {}
};