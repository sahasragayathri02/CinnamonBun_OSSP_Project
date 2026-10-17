CC ?= cc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -O2
TARGET = pipeline
all: $(TARGET)
$(TARGET): pipeline.c
	$(CC) $(CFLAGS) -o $(TARGET) pipeline.c
test: $(TARGET)
	./tests/test_pipeline.sh
clean:
	rm -f $(TARGET)
.PHONY: all test clean
