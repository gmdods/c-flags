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
	    {'a', FLAG_BOOL, "all", &all},
	    {'v', FLAG_BOOL, "verbose", &verbose},
	    {'q', FLAG_BOOL, "quiet", &quiet},
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
	const struct option table[] = {
	    {'v', FLAG_BOOL, "verbose", &verbose},
	    {'c', FLAG_STRING, "compile", &string},
	    {0},
	};
	static const char * compile = "print(1 + 2)";

	flags(table, ARGC_ARGV(1, "-v"));
	ensure(string == NULL), ensure(verbose);

	flags(table, ARGC_ARGV(2, "-c", compile));
	ensure(!strcmp(string, compile)), ensure(!verbose);

	flags(table, ARGC_ARGV(3, "-v", "-c", compile));
	ensure(!strcmp(string, compile)), ensure(verbose);
}

unittest("argument parsing") {
	bool verbose = 0;
	char * string = NULL;
	unsigned opt = 0;
	const struct option table[] = {
	    {'v', FLAG_BOOL, "verbose", &verbose},
	    {'s', FLAG_STRING, "standard", &string},
	    {'O', FLAG_UINT, "opt", &opt},
	    {0},
	};
	static const char * option = "option";

	flags(table, ARGC_ARGV(2, "-v", "-s=option"));
	ensure(!strcmp(string, option)), ensure(verbose), ensure(opt == 0);

	flags(table, ARGC_ARGV(4, "-v", "-s", option, "-O3"));
	ensure(!strcmp(string, option)), ensure(verbose), ensure(opt == 3);
}
