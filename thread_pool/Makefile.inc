CC = gcc
CXX = g++
AR = ar
LD = g++
MAKE = make
CP = cp -fp
RM = rm -f
MD = mkdir -p
MV = mv -f
RANLIB = ranlib
ARFLAGS = rc

debug: CFLAGS = -c -g -Wall -DDEBUG
debug: CXXFLAGS = -c -g -Wall -DDEBUG
debug: LDFLAGS =

release: CFLAGS = -c -O2 -g -Wall -DNDEBUG
release: CXXFLAGS = -c -O2 -g -Wall -DNDEBUG
release: LDFLAGS =

.c.o:
	@echo -n "Compiling $<... "
	@$(CC) $(CFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."

.cc.o:
	@echo -n "Compiling $<... "
	@$(CXX) $(CXXFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."

.cpp.o:
	@echo -n "Compiling $<... "
	@$(CXX) $(CXXFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."

.cxx.o:
	@echo -n "Compiling $<... "
	@$(CXX) $(CXXFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."
