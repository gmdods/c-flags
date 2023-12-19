#ifndef UNITTEST_MAIN
#include "unittest.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../flags.h"

#endif /* ifndef UNITTEST_MAIN */

unittest("character parsing") {
	bool all = 0, verbose = 0, quiet = 0;
	const struct option table[] = {
	    {'a', "all", &all},
	    {'v', "verbose",  &verbose},
	    {'q', "quiet", &quiet},
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
