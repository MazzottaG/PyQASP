#### BUILD modalities
# use 
#   $ make BUILD=release 
# to compile different binaries
# use
#   $ make WITH-PYTHON=1 or make WITH-SPARQL=1 or make WITH-SQL=1
# to compile with python,sparql or sql features

WITH-PYTHON=0
WITH-PYTHON_THREE=0
WITH-PYTHON_THREE_EIGHT=0
WITH-SPARQL=0
WITH-SQL=0
WITH-HTD=0

DEXT=
DHTD=
ifeq ($(WITH-PYTHON_THREE),1)
        DEXT+= -DPYTHON -DENABLE_PYTHON -DPYTHON_THREE
else ifeq ($(WITH-PYTHON_THREE_EIGHT),1)
        DEXT+= -DPYTHON -DENABLE_PYTHON -DPYTHON_THREE
else ifeq ($(WITH-PYTHON),1)
        DEXT+= -DPYTHON -DENABLE_PYTHON
endif
ifeq ($(WITH-SPARQL),1)
        DEXT+= -DSPARQL
endif
ifeq ($(WITH-SQL),1)
        DEXT+= -DSQL
endif
ifeq ($(WITH-HTD),1)
        DHTD+= -DHTD
endif


BUILD = release

cxxflags.debug = \
 -Wall -std=c++11 -g3 $(DEXT) $(DHTD)
linkflags.debug = \
 -lm -lz

cxxflags.trace = \
 -Wall -std=c++11 -DTRACE_ON $(DEXT) $(DHTD)
linkflags.trace = \
 -lm -lz

cxxflags.release = \
 -Wall -std=c++11 -DNDEBUG -DSTATIC -O3 $(DEXT) $(DHTD)
linkflags.release = \
 -lm -lz

cxxflags.gprof = \
 -Wall -std=c++11 -DNDEBUG -O3 -g -pg $(DEXT) $(DHTD)
linkflags.gprof = \
 -lm -lz -g -pg


SOURCE_DIR = src
BUILD_DIR = build/$(BUILD)
BINARY = $(BUILD_DIR)/dlv2


REDLAND_LIB =
PYTHON_VER = 
PYTHON_LIB = 
PYTHON_INCLUDE = 
PYTHON_DEV_LIB =
SQL_LIB =
HTD_LIB =
ifeq ($(WITH-SPARQL),1)
	REDLAND_LIB := $(shell rasqal-config --cflags) $(shell rasqal-config --libs) -lrdf
endif
ifeq ($(WITH-PYTHON_THREE),1)
#	PYTHON_VER = $(shell python3 -V | grep -oP "\d\\.\d")
#	PYTHON_LIB := -I/usr/lib/python$(PYTHON_VER)
#	PYTHON_INCLUDE := -I/usr/include/python$(PYTHON_VER)
#	PYTHON_DEV_LIB := -lpython$(PYTHON_VER)m
	PYTHON_INCLUDE = $(shell python3-config --cflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g" | sed "s/-arch i386//g" | sed "s/-DNDEBUG//g" | sed "s/-Os//g" )
    PYTHON_LIB = $(shell python3-config --ldflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g")
#    PYTHON_LIB = $(shell python3-config --embed --ldflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g")
else ifeq ($(WITH-PYTHON_THREE_EIGHT),1)
	PYTHON_INCLUDE = $(shell python3-config --cflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g" | sed "s/-arch i386//g" | sed "s/-DNDEBUG//g" | sed "s/-Os//g" )
	PYTHON_LIB = $(shell python3-config --ldflags --embed | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g")
else ifeq ($(WITH-PYTHON),1)
#	PYTHON_VER = $(shell python3 -V | grep -oP "\d\\.\d")
#	PYTHON_LIB := -I/usr/lib/python$(PYTHON_VER)
#	PYTHON_INCLUDE := -I/usr/include/python$(PYTHON_VER)
#	PYTHON_DEV_LIB := -lpython$(PYTHON_VER)m
	PYTHON_INCLUDE = $(shell python2-config --cflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g" | sed "s/-arch i386//g" | sed "s/-DNDEBUG//g" | sed "s/-Os//g" )
    PYTHON_LIB = $(shell python2-config --ldflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g")
endif
ifeq ($(WITH-SQL),1)
	SQL_LIB := -lodbc
endif
ifeq ($(WITH-HTD),1)
	HTD_LIB := -lhtd
endif

GCC = g++
CXX = $(GCC) 
CXXFLAGS = $(cxxflags.$(BUILD))
LINK = $(GCC)
LINKFLAGS = $(linkflags.$(BUILD))
LIBS := $(PYTHON_LIB) $(PYTHON_INCLUDE) $(SQL_LIB) $(REDLAND_LIB) $(HTD_LIB)


SRCS = $(shell find $(SOURCE_DIR) -name '*.cpp')

OBJS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SRCS))
DEPS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.d, $(SRCS))

all: $(BINARY)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(LIBS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(LIBS) $(CXXFLAGS) -MM -MT '$(@:.d=.o)' $< -MF $@

$(BINARY): $(OBJS) $(DEPS)
	$(LINK) $(OBJS) $(LIBS) $(LINKFLAGS) -o $(BINARY)

static: $(OBJS) $(DEPS)
	$(LINK) $(OBJS) $(LIBS) $(LINKFLAGS) -static -o $(BINARY)

run: $(BINARY)
	./$(BINARY)

parser: $(SOURCE_DIR)/idlv/input/aspcore2.l $(SOURCE_DIR)/idlv/input/aspcore2.y
	flex -o$(SOURCE_DIR)/idlv/input/aspcore2_lexer.hpp $(SOURCE_DIR)/idlv/input/aspcore2.l
	bison -y -d -o$(SOURCE_DIR)/idlv/input/aspcore2_parser.c $(SOURCE_DIR)/idlv/input/aspcore2.y
	bison -y -o$(SOURCE_DIR)/idlv/input/aspcore2_parser.hpp $(SOURCE_DIR)/idlv/input/aspcore2.y
	rm -f $(SOURCE_DIR)/idlv/input/aspcore2_parser.c

########## Clean

clean-dep:
	rm -f $(DEPS)
clean: clean-dep
	rm -f $(OBJS)

distclean: clean
	rm -fr $(BUILD_DIR)

-include $(DEPS)
