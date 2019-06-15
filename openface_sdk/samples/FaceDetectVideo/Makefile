ifeq ($(RELEASE_DIR),)
RELEASE_DIR := ../..
endif 

#############################################################
INCS += -I$(RELEASE_DIR)/include
INCS += -I../common

#############################################################
LIBS := -L$(RELEASE_DIR)/lib -lrr_facedetect_3_0 -lcaffe -lopenblas
LIBS += -lopencv_highgui -lopencv_core -lopencv_calib3d 
LIBS += -ldrmservice -lfirefly_faceverify
LIBS += -pthread

#############################################################
CXX := g++
CXXFLAGS := -O2 -Wall -s -std=c++11 
CXXFLAGS += $(INCS)
LDFLAGS += -Wl,-rpath,$(RELEASE_DIR)/lib

#############################################################
all :  FaceDetectVideoSample

FaceDetectVideoSample : FaceDetectVideoSample.cpp  
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f FaceDetectVideoSample *.o

.PHONY : all clean FaceDetectVideoSample

