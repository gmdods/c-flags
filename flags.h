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
	FLAG_UINT,
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
		case FLAG_UINT: *((unsigned *) entry.variable) = 0; break;
		case FLAG_STRING:
			*((const char **) entry.variable) = NULL;
			break;
		default: break;
		}
	}
}

static inline short flag_assign(struct option entry, const char * endp,
				size_t argn, const char * argv[static argn]) {

	switch (entry.type) {
	case FLAG_BOOL: {
		*((bool *) entry.variable) = true;
		return 0;
	}
	case FLAG_UINT: {
		if (!endp || !*endp) {
			if (0 == argn) return -1;
			endp = argv[1];
		}
		*((unsigned *) entry.variable) = atoi(endp);
		return endp == argv[1];
	}
	case FLAG_STRING: {
		if (!endp || !*endp) {
			if (0 == argn) return -1;
			endp = argv[1];
		}
		*((const char **) entry.variable) = endp;
		return endp == argv[1];
	}
	default: return -1;
	}
}

static inline short flag_dispatch(const struct option table[static 1],
				  size_t argn, const char * argv[static argn]) {
	const char * arg = argv[0];
	struct option entry = {0};

	if (!strcmp(arg, "--")) {
		for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
			if (strcmp(arg + 2, entry.long_name)) continue;
			short delta = flag_assign(entry, NULL, argn, argv);
			if (delta) return delta;
		}
	} else if (arg[0] == '-') {
		const char * endp = NULL;
		for (size_t t = 0; (entry = table[t]).variable != NULL; ++t) {
			endp = strchr(arg + 1, entry.short_name);
			if (!endp) continue;
			short delta = flag_assign(entry, endp + 1, argn, argv);
			if (delta) return delta;
		}
	}
	return 0;
}

void flags(const struct option table[static 1], size_t argc,
	   const char * argv[static argc]) {
	if (argc <= 1) return;

	flags_default(table);

	for (size_t i = 1; i != argc; ++i) {
		short delta = flag_dispatch(table, argc - i, argv + i);
		if (delta < 0) return;
		i += delta;
	}
}

#endif // !FLAGS_H
