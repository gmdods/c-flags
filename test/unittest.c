#include <string.h>
#ifndef UNITTEST_MAIN
#include "unittest.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../flags.h"

#endif /* ifndef UNITTEST_MAIN */

#define ARGC_ARGV(n, ...) \
	n + 1, (const char * [n + 2]) { \
		[0] = __FILE__, __VA_ARGS__, [n + 1] = NULL \
	}

unittest("boolean parsing") {
	bool all = 0, verbose = 0, quiet = 0;
	const struct option table[] = {
	    {'a', "all", FLAG_BOOL, &all},
	    {'v', "verbose", FLAG_BOOL, &verbose},
	    {'q', "quiet", FLAG_BOOL, &quiet},
	    {0},
	};

	flags(table, ARGC_ARGV(2, "-a", "-v"));
	ensure(all), ensure(verbose), ensure(!quiet);

	flags(table, ARGC_ARGV(1, "-av"));
	ensure(all), ensure(verbose), ensure(!quiet);

	flags(table, ARGC_ARGV(1, "-q"));
	ensure(!all), ensure(!verbose), ensure(quiet);

	flags(table, ARGC_ARGV(2, "-a", "--verbose"));
	ensure(all), ensure(verbose), ensure(!quiet);
}

unittest("string parsing") {
	bool verbose = 0;
	char * string = NULL;
	unsigned opt = 0;
	const struct option table[] = {
	    {'v', "verbose", FLAG_BOOL, &verbose},
	    {'c', "compile", FLAG_STRING, &string},
	    {'O', "opt", FLAG_UINT, &opt},
	    {0},
	};
	static const char * compile = "print(1 + 2)";

	flags(table, ARGC_ARGV(1, "-v"));
	ensure(string == NULL), ensure(verbose), ensure(opt == 0);

	flags(table, ARGC_ARGV(2, "-c", compile));
	ensure(!strcmp(string, compile)), ensure(!verbose), ensure(opt == 0);

	flags(table, ARGC_ARGV(3, "-v", "-c", compile));
	ensure(!strcmp(string, compile)), ensure(verbose), ensure(opt == 0);

	flags(table, ARGC_ARGV(4, "-v", "-c", compile, "-O3"));
	ensure(!strcmp(string, compile)), ensure(verbose), ensure(opt == 3);
}
