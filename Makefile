# This is a makefile template

# create xsd build command lines: (to be integrated into the makefile)
# xsd cxx-tree --disable-warning all --type-naming ucc --hxx-suffix .h --cxx-suffix .cpp --output-dir ./src/utils/ --generate-doxygen simulationConfig.xsd
# xsd cxx-tree --disable-warning all --generate-doxygen --hxx-suffix .h --cxx-suffix .cpp --output-dir ./src/outputWriter --generate-serialization src/outputWriter/vtk-unstructured.xsd



# Compiler
# --------
CC=g++
#CC=icpc

SOURCES=\
	src/FileReader.cpp\
	src/MolSim.cpp\
	src/outputWriter/vtk-unstructured.cpp\
	src/outputWriter/VTKWriter.cpp\
	src/outputWriter/XYZWriter.cpp\
	src/Particle.cpp\
	src/utils/Settings.cpp\
	src/Simulator.cpp\
	src/ParticleContainer.cpp\
	src/CellListContainer.cpp\
	src/ScenarioFactory.cpp\
	src/utils/MaxwellBoltzmannDistribution.cpp\
	src/utils/ParticleGenerator.cpp\
	src/utils/SimulationConfig.cpp\
	src/outputWriter/RenderOutputWriter.cpp\
	src/utils/Thermostat.cpp

# Compiler flags
# -------------------------------------------------------------------------
CFLAGS= -g -fpermissive -std=gnu++0x -O3 -Wno-deprecated -fopenmp

# Linker flags
# ------------
LDFLAGS= -lxerces-c -llog4cxx -lcppunit -lpthread -lglut -lopengl32 -fopenmp

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
	
gltest: gltest.o src/Particle.cpp src/outputWriter/RenderOutputWriter.o src/ParticleContainer.o
	g++ gltest.o src/outputWriter/RenderOutputWriter.o src/ParticleContainer.o src/Particle.o -llog4cxx -lopengl32 -lglut -o gltest

mptest: mptest.cpp
	g++ mptest.cpp -fopenmp -o mptest

