CXX=g++-4.8
BIN=./bin

LOCAL_ROOT := ../third/local
THIRD_INCPATH := -I $(LOCAL_ROOT)/include -I /usr/local/Cellar/protobuf/2.6.1/include
THIRD_LIB := -L $(LOCAL_ROOT)/lib -L /usr/local/Cellar/protobuf/2.6.1/lib
CXXFLAGS := -g -Bstatic -std=c++11 -Bdynamic -pthread -lpthread -lgtest -lgtest_main -lglog -lprotobuf -I./ $(THIRD_INCPATH) $(THIRD_LIB) 

OBJS=utils/vec.o utils/mat.o \
	 layers/NNProjection.o \
	 proto/LayerConfig.pb.o proto/ProjectionConfig.pb.o proto/Argument.pb.o 


$(BIN)/test: unittest/test.cpp $(OBJS) unittest/utils/*.h unittest/proto/*.h
	@mkdir -p $(BIN)
	@echo "compiling" unittest/test.cpp 
	$(CXX) unittest/test.cpp $(CXXFLAGS)  $(OBJS) -o $(BIN)/test

utils/vec.o : utils/vec.h
utils/mat.o : utils/mat.h
proto/LayerConfig.pb.o : proto/LayerConfig.pb.h 
proto/ProjectionConfig.pb.o : proto/ProjectionConfig.pb.h
layers/NNProjection.o : layers/NNProjection.h

%.o: %.cpp
	@echo "compile $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@
%.o: %.cc
	@echo "compile $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm  -f $(BIN)/test 
	rm -f $(OBJS)
