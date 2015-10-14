CXX=g++
BIN=./bin

CXXFLAGS := -g -std=c++11 -pthread -lpthread -lgtest -lgtest_main -lglog 
LOCAL_ROOT := ../hasky/third/local
THIRD_INCPATH := -I $(LOCAL_ROOT)/include
THIRD_LIB := -L $(LOCAL_ROOT)/lib

OBJS=utils/vec.o

$(BIN)/test: unittest/test.cpp $(OBJS)
	mkdir -p $(BIN)
	$(CXX) unittest/test.cpp $(CXXFLAGS) -I./ $(THIRD_INCPATH) -Xlinker $(THIRD_LIB) -o $(BIN)/test

utils/vec.o : utils/vec.h

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(THIRD_INCPATH) $(THIRD_LIB) -o $@ $<

.PHONY : clean
clean:
	rm  $(BIN)/test $(OBJS)
