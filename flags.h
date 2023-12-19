#ifndef FLAGS_H
#define FLAGS_H
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if !(__STDC_VERSION__ >= 199901L)
#error "Must have at least C99"
#endif

struct option {
	char short_name;
	bool * variable;
};

void flags(const struct option table[static 1], int argc,
	   const char * argv[static argc]) {
	if (argc <= 1) return;

	struct option entry = {0};
	for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
		*entry.variable = false;
	}

	const char * arg = {0};
	for (size_t i = 1; i != (size_t) argc; ++i) {
		arg = argv[i];
		if (arg[0] != '-') continue;
		for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
			if (strchr(arg, entry.short_name) != NULL)
				*entry.variable = true;
		}
	}
}

#endif // !FLAGS_H
