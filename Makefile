GCC := gcc
FLAGS := -O3
SUFFIX :=

SRCDIR := src
INCDIR := include
OBJDIR := obj
LIBDIR := lib
BINDIR := bin

PROJECT := string
LIBNAME := lib$(PROJECT)

debug: SUFFIX = d
debug: FLAGS = -g -Wall
test: SUFFIX = d

debug: $(LIBDIR)/$(LIBNAME)$(SUFFIX).a

release: $(LIBDIR)/$(LIBNAME)$(SUFFIX).a

dirs:
	mkdir -p ./$(LIBDIR)
	mkdir -p ./$(OBJDIR)
	mkdir -p ./$(BINDIR)

$(OBJDIR)/$(LIBNAME)$(SUFFIX).o: dirs $(SRCDIR)/$(LIBNAME).c $(INCDIR)/$(LIBNAME).h
	$(GCC) $(FLAGS) -c -o $(OBJDIR)/$(LIBNAME)$(SUFFIX).o $(SRCDIR)/$(LIBNAME).c -I$(INCDIR)

$(LIBDIR)/$(LIBNAME)$(SUFFIX).a: $(OBJDIR)/$(LIBNAME)$(SUFFIX).o
	ar rcs $(LIBDIR)/$(LIBNAME)$(SUFFIX).a $(OBJDIR)/$(LIBNAME)$(SUFFIX).o

test: debug $(SRCDIR)/test.c
	gcc -o $(BINDIR)/test $(SRCDIR)/test.c -l$(PROJECT)$(SUFFIX) -L$(LIBDIR) -I$(INCDIR)

.PHONY: clean

clean:
	rm -rf ./$(LIBDIR)
	rm -rf ./$(OBJDIR)
	rm -rf ./$(BINDIR)