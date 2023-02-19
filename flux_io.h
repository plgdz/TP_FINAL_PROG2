#pragma once
#include <iostream>

#ifdef _CORRECTION__
	#include <sstream>
	extern std::string globalFileName;
	extern std::stringstream ___stream_write;
	extern std::stringstream ___stream_read;
	#define ecrire ___stream_write
	#define lire ___stream_read
#else
	#define ecrire std::cout
	#define lire std::cin
#endif

std::ostream& operator<<(std::ostream&, const char[]);