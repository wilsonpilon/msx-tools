# Makefile para o programa MSX-Tools

# Variaveis
TARGET = msxtools
IDIR = inc
ODIR = obj
LDIR = lib
SDIR = src
LIBS = -lm -lcdk -lncurses -lboost_program_options
INCS = -I/usr/include/boost
CPP = g++
CPPFLAGS = -g -Wall
_DEPS = msx.h hex_editor.h window.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ = main.o msx_tools.o hex_editor.o window.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
RM = rm -rf

all: $(TARGET)

# Programa principal
$(TARGET): $(OBJ)
	$(CPP) -o $@ $^ $(CPPFLAGS) $(LIBS)

# Subprogramas
$(ODIR)/%o: $(SDIR)/%cpp $(DEPS)
	$(CPP) -c -o $@ $< -I$(IDIR) $(INCS) $(CPPFLAGS)

.PHONY: clean

# Limpeza de arquivos objeto, backup e core
clean:
	$(RM) $(ODIR)/*.o *~ core $(IDIR)/*~

# Limpeza total
distclean: clean
	$(RM) -rf $(TARGET)