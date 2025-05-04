# important directories
SRC := src
BIN := bin
OBJ := obj
INC := include

# compiler settings (common for app and instrumentation lib)
CXX      := g++
CXXFLAGS := -I$(INC) -gdwarf-5

# determine targets
APP_SOURCES = $(wildcard $(SRC)/app/*.cpp)
APP_OBJECTS = $(patsubst $(SRC)/app/%.cpp, $(OBJ)/app-%.o, $(APP_SOURCES))

INS_SOURCES = $(wildcard $(SRC)/ins/*.cpp)
INS_OBJECTS = $(patsubst $(SRC)/ins/%.cpp, $(OBJ)/ins-%.o, $(INS_SOURCES))

# default target
build: $(BIN)/http-get

$(BIN)/http-get: $(APP_OBJECTS) | $(BIN)/ $(BIN)/libinstool.so
	$(CXX) -o $@ $^ $(LDFLAGS) -rdynamic -L$(BIN) -linstool

$(BIN)/libinstool.so: $(INS_OBJECTS) | $(BIN)/
	$(CXX) -shared -o $@ $^ -ldl

$(OBJ)/app-%.o: $(SRC)/app/%.cpp | $(OBJ)/
	$(CXX) -c -finstrument-functions -rdynamic $(CXXFLAGS) -o $@ $<

$(OBJ)/ins-%.o: $(SRC)/ins/%.cpp | $(OBJ)/
	$(CXX) -c -fPIC $(CXXFLAGS) -o $@ $<

%/:
	@mkdir -p $@

clean:
	@rm -rf $(OBJ) $(BIN)

.SECONDARY:

.PHONY: clean build

