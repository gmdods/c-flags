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
	const char * long_name;
	bool * variable;
};

static inline void flags_default(const struct option table[static 1]) {
	struct option entry = {0};
	for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
		*entry.variable = false;
	}
}

static inline void flags_short(const struct option table[static 1],
			       const char * arg) {
	struct option entry = {0};
	for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
		if (strchr(arg, entry.short_name) != NULL)
			*entry.variable = true;
	}
}

static inline void flags_long(const struct option table[static 1],
			      const char * arg) {
	struct option entry = {0};
	for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
		if (!strcmp(arg, entry.long_name)) *entry.variable = true;
	}
}

void flags(const struct option table[static 1], int argc,
	   const char * argv[static argc]) {
	if (argc <= 1) return;

	flags_default(table);
	const char * arg = {0};
	for (size_t i = 1; i != (size_t) argc; ++i) {
		arg = argv[i];
		if (!strcmp(arg, "--")) {
			flags_long(table, arg + 2);
		} else if (arg[0] == '-') {
			flags_short(table, arg + 1);
		}
	}
}

#endif // !FLAGS_H
