#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../flags.h"

#define UNITTEST_MAIN
#include "unittest.h"

int main(int argc, const char * argv[]) {

#include "unittest.c"

	summary();
	return 0;
}
