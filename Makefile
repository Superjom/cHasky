CXX = clang++
BIN = bin
THIRD_PATH := ../hasky/third/local
ifeq ($(STATIC_THIRD_LIB), 1)
THIRD_LIB=$(addprefix $(THIRD_PATH)/lib/, libgflags.a libprotobuf.a libglog.a)
else
THIRD_LIB=-L$(THIRD_PATH)/lib -lgflags -lglog -lgtest -lgtest_main -lprotobuf
endif

INCPATH = -I./ -I$(THIRD_PATH)/include
CXXFLAGS = -std=c++11 $(INCPATH) 
LDFLAGS = $(THIRD_LIB) -lpthread

OBJS := Activation.o Layer.o Repository.o Projection.o \
	 utils/str.o utils/vec.o utils/mat.o \
	 proto/LayerConfig.pb.o proto/ProjectionConfig.pb.o proto/Argument.pb.o \
	 layers/NNProjection.o 

bin/test: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o bin/test

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
