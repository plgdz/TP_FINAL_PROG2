#include "flux_io.h"

#ifdef _CORRECTION__
	std::stringstream ___stream_write("");
	std::stringstream ___stream_read("");
#endif

std::ostream& operator<<(std::ostream& stream, const char str[]) {
	for (size_t i = 0; str[i] != '\0'; i++)
	{
		stream << str[i];
	}

	return stream;
}