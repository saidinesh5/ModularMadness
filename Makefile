CC  = gcc
CXX = g++
RM  = rm -f
CPPFLAGS = -g -std=c++0x $(shell root-config --cflags) 
LDFLAGS  = -g $(shell root-config --ldflags)
LDLIBS   = $(shell root-config --libs)

SRCS = main.cpp \
       commandparser.cpp \
       engine.cpp \
       module.cpp \
       modules/inputmodule.cpp \
       modules/outputmodule.cpp \
       modules/delaymodule.cpp \
       modules/echomodule.cpp \
       modules/noopmodule.cpp \
       modules/reversemodule.cpp

OBJS = $(subst .cpp,.o,$(SRCS))
TARGET = ModularMadness

all: ModularMadness

ModularMadness: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS) $(TARGET)

dist-clean: clean
	$(RM) *~ .dependtool $(TARGET)

include .depend
