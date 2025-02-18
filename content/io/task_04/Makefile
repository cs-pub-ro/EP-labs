# important directories
SRC = src
BIN = bin
OBJ = obj
INC = include

# compiler settings
CXX      = clang++
CXXFLAGS = -std=c++23 -O2 -I$(INC)
LDFLAGS  = -lGL -ldl -lglfw

# identify sources and generate object targets
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(addprefix $(OBJ)/,$(addsuffix .o, $(basename $(notdir $(SOURCES)))))

# targets
build: $(BIN)/balls

$(BIN)/balls: $(OBJECTS) | $(BIN)/
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)/
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	@rm -rf $(BIN) $(OBJ)

%/:
	@mkdir -p $@

# meta
.PHONY: build clean
.SECONDARY:

