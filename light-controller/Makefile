PIO_USER_LIBS_HOME=.pio/libdeps/universal
PIO_HOME := $(HOME)/.platformio
FRAMEWORK_DIR := $(PIO_HOME)/packages/framework-arduinoavr

CXX := g++
CXXFLAGS := -g -std=gnu++11 -D UNIVERSAL -D SWITCH_SERVICE_VERBOSE -D SWITCH_ROUTER_VERBOSE
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD := build
OBJ_DIR := $(BUILD)/objects
APP_DIR := $(BUILD)/apps
TARGET := program
INCLUDE_FILES  := \
	$(dir $(wildcard lib-universal/*/*.h))
INCLUDE := $(INCLUDE_FILES:%=-I%)

# This should not be part of src for now
# As it includes main program main
# $(wildcard src/*.cpp)

CPP_SRC := \
	$(wildcard test/*.cpp) \
	$(wildcard test/**/*.cpp) \
	$(wildcard lib-universal/*/*.cpp)
CC_SRC := \
	$(wildcard $(PIO_USER_LIBS_HOME)/googletest_ID5976/*/src/*.cc)
OBJECTS := $(CPP_SRC:%.cpp=$(OBJ_DIR)/%.o) $(CC_SRC:%.cc=$(OBJ_DIR)/%.o)

INCLUDE += \
	-I$(PIO_USER_LIBS_HOME)/googletest_ID5976/googlemock/include/ \
	-I$(PIO_USER_LIBS_HOME)/googletest_ID5976/googletest/include/ \
	-I$(PIO_USER_LIBS_HOME)/googletest_ID5976/googletest/

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.c*
	@mkdir -p $(@D)
	@echo Buidling object for $<
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)

build-test: $(APP_DIR)/$(TARGET)

test: build-test
	$(APP_DIR)/$(TARGET)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*