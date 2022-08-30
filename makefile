ELF      = a.out
LDFLAGS  = $(shell pkg-config --libs glew glut)

SDIR     = src
IDIR     = include
ODIR     = obj

SRC      = main.cpp\
	   util.cpp\
	   mutil.cpp\
	   world_transform.cpp\
	   camera_transform.cpp\
	   perspective_transform.cpp

DEP      = util.h\
	   mutil.h\
	   world_transform.h\
	   camera_transform.h\
	   perspective_transform.h\
	   projection_params.h

$(ELF): $(patsubst %,$(ODIR)/%.o,$(basename $(SRC)))
	g++ $^ $(LDFLAGS) -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp $(patsubst %,$(IDIR)/%,$(DEP))
	mkdir -p $(ODIR)
	g++ -Wall -std=c++20 -O0 -g -I$(IDIR) -c $< -o $@

clean:
	rm -rf $(ODIR)/ $(ELF)

.PHONY: clean 
