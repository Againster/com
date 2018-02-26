top_builddir = .

include $(top_builddir)/Makefile.inc

INCS = -I.

DEFS =

LIBS =  -lpthread

BIN = demo

OBJS = main.o \
			 thread_factory.o \
			 lz_queue.o \
			 executor_service.o \
			 thread.o

all: release
debug: $(BIN)
release: $(BIN)

$(BIN): $(OBJS)
	@echo -n "Linking $@... "
	@$(LD) -O3 -o $@ $(OBJS) $(LIBS)
	@echo "Done."

.PHONY: clean

clean:
	@echo -n "Cleaning... "
	@$(RM) $(BIN) $(BIN_1) $(OBJS) $(OBJS_1)
	@echo "Done."
