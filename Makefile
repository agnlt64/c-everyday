# Find all subdirectories
SUBDIRS := $(wildcard */)
CC := gcc

# Default target
all: $(SUBDIRS)

# Rule for each subdirectory
$(SUBDIRS):
	@echo "Building in $@"
	@$(MAKE) -f $(CURDIR)/Makefile build-in-dir DIR=$@

# Build in specific directory
.PHONY: build-in-dir
build-in-dir:
	@if ls $(DIR)*.c >/dev/null 2>&1; then \
		$(CC) $(DIR)*.c -o $(DIR)$(notdir $(DIR:%/=%)); \
		echo "Created executable: $(DIR)$(notdir $(DIR:%/=%))" ; \
	else \
		echo "No C files found in $(DIR)" ; \
	fi

# Clean target
.PHONY: clean
clean:
	@for dir in $(SUBDIRS); do \
		rm -f $$dir$$(basename $$dir); \
		echo "Cleaned $$dir"; \
	done

.PHONY: all $(SUBDIRS)