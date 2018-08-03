UNAME := $(shell uname)
CXX=g++
CC=gcc

OPTIONS = -fPIC -Wall -m32
ROOTCFLAGS   = $(shell root-config --cflags)
ROOTMAIN     = $(shell root-config --libs)
ROOTGMAIN     = $(shell root-config --glibs)
CERNLIB = `cernlib pawlib graflib packlib mathlib kernlib` -lg2c -lm
JHF2KMLIB = $(WCSIM)/libWCSimRoot.so -L$(WCSIM) -lWCSim
INC = includes/
# Root, glui, glut libraries are OS dependent
#ifeq ($(UNAME), Linux)
#ROOTLIBS = -L/usr/lib/root -lNew $(ROOTMAIN) $(ROOTGMAIN) -lMinuit -L/usr/lib/root/5.18
#INCLUDES = -I/usr/include/root -I$(INC) -I$(WCSIM)/include
#endif
#ifeq ($(UNAME), Darwin)
ROOTLIBS = -L$(ROOTSYS)/lib $(ROOTMAIN) $(ROOTGMAIN) -lMinuit
INCLUDES = -I$(ROOTSYS)/include -I$(INC) -I$(WCSIM)/include
#endif


LDLIBS +=  $(JHF2KMLIB) $(ROOTLIBS)
OBJS = main.o handleroot.o residualtime.o cylinderstep.o sphericaltransform.o oglHistogram.o RGBcolorSpectrum.o glutstuff.o pointmodification.o drawevent.o
ALL = $(OPTIONS) $(LDLIBS) $(GLLIBS) $(INCLUDES)
ifeq ($(shell uname),Darwin)
	GLLIBS = -framework OpenGL -framework GLUT -framework GLUI
else
	GLLIBS = -lglut -lGLU -lglui
endif


SO = libWCMap.so

standard : main clean

main : $(OBJS)
	g++ $(ALL) $(OBJS) -o runme

%.o : $(INC)/%.cc
	g++ $(OPTIONS) $(INCLUDES) -c $(INC)/$*.cc

%.o : %.cpp
	g++ $(OPTIONS) $(INCLUDES) -c $*.cpp

clean :
	rm -f *.o
	ln -f -s $(WCSIM)/libWCSimRoot.so libWCSimRoot.so