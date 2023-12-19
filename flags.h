#ifndef FLAGS_H
#define FLAGS_H
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if !(__STDC_VERSION__ >= 199901L)
#error "Must have at least C99"
#endif

enum flag {
	FLAG_BOOL,
	FLAG_STRING,
};

struct option {
	char short_name;
	const char * long_name;
	enum flag type;
	void * variable;
};

static inline void flags_default(const struct option table[static 1]) {
	struct option entry = {0};
	for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
		switch (entry.type) {
		case FLAG_BOOL: *((bool *) entry.variable) = false; break;
		case FLAG_STRING:
			*((const char **) entry.variable) = NULL;
			break;
		default: break;
		}
	}
}

static inline size_t flag_assign(struct option entry, size_t argn,
				 const char * argv[static argn]) {
	switch (entry.type) {
	case FLAG_BOOL: *((bool *) entry.variable) = true; return 0;
	case FLAG_STRING:
		if (0 == argn) return 0;
		*((const char **) entry.variable) = argv[1];
		return 1;
	default: return 0;
	}
}

static inline size_t flag_dispatch(const struct option table[static 1],
				   size_t argn,
				   const char * argv[static argn]) {
	const char * arg = argv[0];
	struct option entry = {0};
	size_t delta = 0;
	if (!strcmp(arg, "--")) {
		for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
			if (strcmp(arg + 2, entry.long_name)) continue;
			delta += flag_assign(entry, argn - delta, argv + delta);
		}
	} else if (arg[0] == '-') {
		for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
			if (!strchr(arg + 1, entry.short_name)) continue;
			delta += flag_assign(entry, argn - delta, argv + delta);
		}
	}
	return delta;
}

void flags(const struct option table[static 1], size_t argc,
	   const char * argv[static argc]) {
	if (argc <= 1) return;

	flags_default(table);
	for (size_t i = 1; i != argc; ++i) {
		i += flag_dispatch(table, argc - i, argv + i);
	}
}

#endif // !FLAGS_H
