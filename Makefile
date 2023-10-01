SWIG 	 := -swig
SWIGFLAGS:= -c++ -python
CXX      := -gcc
CXXFLAGS := -Wno-unused-result -Wall -Wno-deprecated-declarations \
			-Wsign-compare -Wunreachable-code -fno-common -dynamic -fwrapv \
			-arch x86_64 -g
SWIG_CXX_SO_FLAGS := -bundle -undefined dynamic_lookup -arch x86_64
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
SWIG_DIR := $(BUILD)/swig
INCLUDE_DIR := include
SRC_DIR := src
TARGET   := chess
INCLUDE  := -I $(INCLUDE_DIR) \
			-I /Library/Frameworks/Python.framework/Versions/3.8/include/python3.8

SRC      :=                      \
   $(wildcard $(SRC_DIR)/*.cpp)

SWIG_MODULES := board

OBJECTS  := $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)/%.o)
HEADERS  := $(SRC:$(SRC_DIR)%.cpp=$(INCLUDE_DIR)/%.h)
SWIG_CPP_MODULES := $(SWIG_MODULES:%=$(SWIG_DIR)/%_wrap.cpp)
SWIG_OBJ_MODULES := $(SWIG_MODULES:%=$(SWIG_DIR)/%_wrap.o)
SWIG_SO_MODULES := $(SWIG_MODULES:%=$(SWIG_DIR)/_%.so)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET) $(SWIG_SO_MODULES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

$(SWIG_DIR)/%_wrap.cpp: $(INCLUDE_DIR)/%.h
	$(SWIG) $(SWIGFLAGS) -o $@ $<

$(SWIG_DIR)/%_wrap.o: $(SWIG_DIR)/%_wrap.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(SWIG_DIR)/_%.so: $(SWIG_DIR)/%_wrap.o $(OBJ_DIR)/%.o
	$(CXX) $(SWIG_CXX_SO_FLAGS) -g $^ -o $@


-include $(DEPENDENCIES)

.PHONY: all build clean debug release info test

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(SWIG_DIR)

debug: CXXFLAGS += -DNDEBUG -g
debug: all

release: CXXFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(SWIG_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
