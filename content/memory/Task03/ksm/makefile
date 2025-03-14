# important directories and files
SRC 	 := src
OBJ 	 := obj
INCLUDE  := include
BINARY   := ksm

#replace with anything you like
CC		?= gcc
C_FLAGS	?= -ggdb -I $(INCLUDE)

# indentify sources and vreate object files
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

# top level build rule
build: dirs $(BINARY)

# non-persistent directory creation rule
dirs:
	@mkdir -p $(OBJ)

# ksm binary creation rule
$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

# ksm objects compilation rule
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(C_FLAGS) -c -o $@ $<

# clean rule
clean:
	rm -rf $(OBJ) $(BINARY)