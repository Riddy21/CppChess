UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXX      := clang++
	CXXFLAGS := -pthread -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O2 -Wall -g -fstack-protector-strong \
				-Wformat -Werror=format-security -g -fwrapv -O2 -g -fstack-protector-strong -Wformat -Werror=format-security \
				-Wdate-time -D_FORTIFY_SOURCE=2 -fPIC -std=c++20 -Wno-deprecated-declarations
	SWIG_CXX_SO_FLAGS := -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -Wl,-z,relro -g -fwrapv -O2 -Wl,-Bsymbolic-functions -Wl,-z,relro -g
	PYTHON_PACKAGE := /usr/include/python3.8
endif
ifeq ($(UNAME_S),Darwin)
	CXX      := clang++
	CXXFLAGS := -Wno-unused-result -Wall -Wno-deprecated-declarations \
				-Wsign-compare -Wunreachable-code -fno-common -dynamic -fwrapv \
				-arch x86_64 -g -MD -MP -std=c++20 -stdlib=libc++
	SWIG_CXX_SO_FLAGS := -bundle -undefined dynamic_lookup -arch x86_64
	PYTHON_PACKAGE := /Library/Frameworks/Python.framework/Versions/3.8/include/python3.8
endif

SWIG 	 := swig
SWIGFLAGS:= -c++ -python
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
SWIG_DIR := $(BUILD)/swig
LOG_DIR := $(BUILD)/log
INCLUDE_DIR := include
SRC_DIR := src
TEST_DIR := utests
TARGET   := chess
INCLUDE  := -I $(INCLUDE_DIR) \
			-I $(PYTHON_PACKAGE)

SRC      :=                      \
   $(wildcard $(SRC_DIR)/*.cpp)

SWIG_MODULES := board rules piece

TEST_TARGETS := $(SWIG_MODULES:%=test_%)
OBJECTS  := $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)/%.o)
HEADERS  := $(SRC:$(SRC_DIR)%.cpp=$(INCLUDE_DIR)/%.h)
SWIG_CPP_MODULES := $(SWIG_MODULES:%=$(SWIG_DIR)/%_wrap.cpp)
SWIG_OBJ_MODULES := $(SWIG_MODULES:%=$(SWIG_DIR)/%_wrap.o)
SWIG_SO_MODULES := $(SWIG_MODULES:%=$(SWIG_DIR)/_%.so)
UNITTEST := $(SWIG_MODULES:%=$(LOG_DIR)/test_%.py.out)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all: build $(OBJECTS) unittest $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

$(SWIG_DIR)/%_wrap.cpp: $(INCLUDE_DIR)/%.h
	$(SWIG) $(SWIGFLAGS) -o $@ -l $<

$(SWIG_DIR)/%_wrap.o: $(SWIG_DIR)/%_wrap.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(SWIG_DIR)/_%.so: $(OBJECTS) $(SWIG_OBJ_MODULES)
	$(CXX) $(CXXFLAGS) $(SWIG_CXX_SO_FLAGS) -g $^ -o $@ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info test $(TEST_TARGETS) $(UNITTEST)

$(UNITTEST): $(LOG_DIR)/test_%.py.out: $(TEST_DIR)/test_%.py $(SWIG_SO_MODULES)
	export PYTHONPATH=$(SWIG_DIR); python3 -m unittest $< 2>&1 | tee -a $@

$(TEST_TARGETS): test_%: $(LOG_DIR)/test_%.py.out

unittest: $(UNITTEST)

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(SWIG_DIR)
	@mkdir -p $(LOG_DIR)

debug: CXXFLAGS += -DNDEBUG -g
debug: all

release: CXXFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(SWIG_DIR)/*
	-@rm -rvf $(LOG_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
