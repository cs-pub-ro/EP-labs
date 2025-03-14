# tool is compiled out of source; must specify PIN_ROOT
ROOT := $(abspath $(shell pwd))
PIN_ROOT := $(ROOT)/third_party/pin-3.24

CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config

# import default rules and definitions
include $(CONFIG_ROOT)/makefile.config
include $(TOOLS_ROOT)/Config/makefile.default.rules

# update tool flags
TOOL_CXXFLAGS += -I$(ROOT)/include -Wno-error

# export target tool name
export TEST_TOOL_ROOTS := minspect

# custom build rule (needed if makefile and *.cpp are not in same dir)
$(OBJDIR)%$(OBJ_SUFFIX): src/%.cpp
	$(CXX) $(TOOL_CXXFLAGS) $(COMP_OBJ)$@ $<
