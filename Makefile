CXX=g++
BIN=./bin

LOCAL_ROOT := ../hasky/third/local
THIRD_INCPATH := -I $(LOCAL_ROOT)/include -I /usr/local/Cellar/protobuf/2.6.1/include
THIRD_LIB := -L $(LOCAL_ROOT)/lib -L /usr/local/Cellar/protobuf/2.6.1/lib
CXXFLAGS := -g -std=c++11 -pthread -lpthread -lgtest -lgtest_main -lglog -lprotobuf -I./ $(THIRD_INCPATH) $(THIRD_LIB) 

OBJS=utils/vec.o proto/LayerConfig.pb.o
$(BIN)/test: unittest/test.cpp $(OBJS) unittest/utils/*.h unittest/proto/*.h
	@mkdir -p $(BIN)
	@echo "compiling" unittest/test.cpp 
	$(CXX) unittest/test.cpp $(CXXFLAGS) -o $(BIN)/test $(OBJS)

utils/vec.o : utils/vec.h
proto/LayerConfig.pb.o : proto/LayerConfig.pb.h 

%.o: %.cpp
	@echo "compile $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm  $(BIN)/test $(OBJS)
