# This is a makefile template

# create xsd build command lines: (to be integrated into the makefile)
# xsd cxx-tree --disable-warning all --type-naming ucc --hxx-suffix .h --cxx-suffix .cpp --output-dir ./src/utils/ --generate-doxygen simulationConfig.xsd
# xsd cxx-tree --disable-warning all --generate-doxygen --hxx-suffix .h --cxx-suffix .cpp --output-dir ./src/outputWriter --generate-serialization src/outputWriter/vtk-unstructured.xsd



# Compiler
# --------
CC=g++
#CC=icpc

include files.mk


# Compiler flags
# -------------------------------------------------------------------------
CFLAGS=-g -fpermissive -std=c++0x -O3 -Wno-deprecated 

# Linker flags
# ------------
LDFLAGS= -lxerces-c -llog4cxx -lcppunit

INCLUDES= -I./src -I./libxsd

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=MolSim

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 

clean:
	rm $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

xsd: src/utils/SimulationConfig.cpp\
	src/outputWriter/vtk-unstructured.cpp

src/utils/SimulationConfig.cpp: simulationConfig.xsd
	xsd cxx-tree --disable-warning all --type-naming ucc --hxx-suffix .h --cxx-suffix .cpp --output-dir ./src/utils/ --generate-doxygen simulationConfig.xsd

src/outputWriter/vtk-unstructured.cpp: src/outputWriter/vtk-unstructured.xsd
	xsd cxx-tree --disable-warning all --generate-doxygen --hxx-suffix .h --cxx-suffix .cpp --output-dir ./src/outputWriter --generate-serialization src/outputWriter/vtk-unstructured.xsd

test: all
	./$(EXECUTABLE) -test all
