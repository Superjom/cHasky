CXX=g++-4.8
BIN=./bin

LOCAL_ROOT := ../third/local
THIRD_INCPATH := -I $(LOCAL_ROOT)/include -I /usr/local/Cellar/protobuf/2.6.1/include
THIRD_LIB := -L $(LOCAL_ROOT)/lib 
THIRD_STATIC_LIBRARY := $(LOCAL_ROOT)/lib/libgtest.a $(LOCAL_ROOT)/lib/libprotobuf.a 
CXXFLAGS := -g -Bstatic -std=c++11 -Bdynamic -lglog -pthread -lpthread -I./ $(THIRD_INCPATH) $(THIRD_LIB) 

OBJS=Repository.o Projection.o \
	 utils/str.o utils/vec.o utils/mat.o \
	 proto/LayerConfig.pb.o proto/ProjectionConfig.pb.o proto/Argument.pb.o \
	 layers/NNProjection.o 


$(BIN)/test: unittest/test.cpp $(OBJS) unittest/utils/*.h unittest/proto/*.h \
		Repository.h \
		unittest/layers/test_NNProjection.h
	@mkdir -p $(BIN)
	@echo "compiling" unittest/test.cpp 
	export PKG_CONFIG_PATH=$(LOCAL_ROOT)/lib/pkgconfig; $(CXX) unittest/test.cpp $(CXXFLAGS) $(THIRD_STATIC_LIBRARY) $(OBJS) -o $(BIN)/test `pkg-config --cflags --libs protobuf`

#Edge.o : Edge.h
Activation.o : Activation.h
Layer.o : Layer.h
Projection.o : Projection.h
Repository.o : Repository.h
utils/vec.o : utils/vec.h
utils/mat.o : utils/mat.h
utils/str.o : utils/str.h
proto/LayerConfig.pb.o : proto/LayerConfig.pb.h 
proto/ProjectionConfig.pb.o : proto/ProjectionConfig.pb.h
proto/LayerConfig.pb.o: proto/LayerConfig.pb.h
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
shallow_clean:
	rm  -f $(BIN)/test 
test:
	./bin/test
