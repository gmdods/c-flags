#include <string.h>
#ifndef UNITTEST_MAIN
#include "unittest.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../flags.h"

#endif /* ifndef UNITTEST_MAIN */

unittest("boolean parsing") {
	bool all = 0, verbose = 0, quiet = 0;
	const struct option table[] = {
	    {'a', "all", FLAG_BOOL, &all},
	    {'v', "verbose", FLAG_BOOL, &verbose},
	    {'q', "quiet", FLAG_BOOL,&quiet},
	    {0},
	};

	flags(table, 3, (const char * [4]){__FILE__, "-a", "-v", NULL});
	ensure(all); ensure(verbose); ensure(!quiet);

	flags(table, 2, (const char * [3]){__FILE__, "-av", NULL});
	ensure(all); ensure(verbose); ensure(!quiet);

	flags(table, 2, (const char * [3]){__FILE__, "-q", NULL});
	ensure(!all); ensure(!verbose); ensure(quiet);

	flags(table, 3, (const char * [4]){__FILE__, "-a", "--verbose", NULL});
	ensure(all); ensure(verbose); ensure(!quiet);
}


unittest("string parsing") {
	bool verbose = 0, quiet = 0;
	char * string = NULL;
	const struct option table[] = {
	    {'v', "verbose", FLAG_BOOL, &verbose},
	    {'q', "quiet", FLAG_BOOL, &quiet},
	    {'c', "compile", FLAG_STRING, &string},
	    {0},
	};
	static const char * compile = "print(1 + 2)";
	static const char * regex = "[0-9]+";

	flags(table, 2, (const char * [3]){__FILE__, "-v", NULL});
	ensure(string == NULL); ensure(verbose); ensure(!quiet);

	flags(table, 3, (const char * [4]){__FILE__, "-c", compile, NULL});
	ensure(!strcmp(string, compile)); ensure(!verbose); ensure(!quiet);

	flags(table, 4, (const char * [5]){__FILE__, "-v", "-c", compile, NULL});
	ensure(!strcmp(string, compile)); ensure(verbose); ensure(!quiet);
}
