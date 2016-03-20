.PHONY : submake clean

BIN = server
INSTALL_PATH = ./bin
TARGET = $(INSTALL_PATH)/$(BIN)
VPATH = :src
SRC_PATH = $(foreach dir, $(subst :, ,$(VPATH)), $(wildcard $(dir)/*.cc))
OBJ_DIR = ./obj
OBJ_PATH = $(addprefix $(OBJ_DIR)/, $(subst :, ,$(VPATH)))
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .cc,.o,$(SRC_PATH)))
MAKE_OBJECT_DIR := $(shell mkdir -p $(OBJ_DIR) $(OBJ_PATH))
INCLUDES = -I./include/ -I./rtp/

LIBS = -lpthread -lrtp
LIBDIRS = -L./lib
CXX = g++
CXXFLAGES = -Wall -g -std=c++11
RM = rm -rf


$(TARGET):$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) $(LIBDIRS)

$(OBJ_PATH)/%.o:%.cc
	$(CXX) $(CXXFLAGES) $(INCLUDES) -c $^ -o $@


submake:
	make -C ./rtp/
	make -C ./dict/

clean:
	$(RM) $(INSTALL_PATH)/$(BIN)
	$(RM) $(wildcard $(OBJ_DIR)/*)
	make -C ./rtp/ clean
	make -C ./dict/ clean
