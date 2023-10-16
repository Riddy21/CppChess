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
LIB_DIR := $(BUILD)/lib
LOG_DIR := $(BUILD)/log
PY_DIR := pysrc
INCLUDE_DIR := include
SRC_DIR := src
UTEST_DIR := utests
SYSTEST_DIR :=systemtests

TARGET   := chesslib
PY_TARGETS := \
	$(wildcard $(PY_DIR)/*.py)
UTEST_TARGETS := $(patsubst $(UTEST_DIR)/test_%.py, test_%, $(wildcard $(UTEST_DIR)/*.py))
SYSTEST_TARGETS := $(patsubst $(SYSTEST_DIR)/test_%.py, test_%, $(wildcard $(SYSTEST_DIR)/*.py))

INCLUDE  := -I $(INCLUDE_DIR) \
			-I $(PYTHON_PACKAGE)

SRC      :=                      \
   $(wildcard $(SRC_DIR)/*.cpp)

OBJECTS  := $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)/%.o)
HEADERS  := $(SRC:$(SRC_DIR)%.cpp=$(INCLUDE_DIR)/%.h)
SWIG_LIB := $(LIB_DIR)/_$(TARGET).so
PY_LIB   := $(PY_TARGETS:$(PY_DIR)/%.py=$(LIB_DIR)/%.py)

DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all: build $(OBJECTS) $(SWIG_LIB) $(PY_LIB) unittest systemtest

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

$(LIB_DIR)/%_wrap.cpp: $(INCLUDE_DIR)/%.i $(HEADERS) $(SRC)
	$(SWIG) $(SWIGFLAGS) -o $@ -l $<

$(OBJ_DIR)/%_wrap.o: $(LIB_DIR)/%_wrap.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(SWIG_LIB): $(LIB_DIR)/_%.so: $(OBJECTS) $(OBJ_DIR)/%_wrap.o
	$(CXX) $(CXXFLAGS) $(SWIG_CXX_SO_FLAGS) -g $^ -o $@ $(LDFLAGS)

$(PY_LIB): $(LIB_DIR)/%.py: $(PY_DIR)/%.py
	cp $< $@

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info unittest $(UTEST_TARGETS) $(SYSTEST_TARGETS)

$(UTEST_TARGETS): test_%: $(UTEST_DIR)/test_%.py $(SWIG_LIB) $(PY_LIB)
	export PYTHONPATH=$(LIB_DIR); python3 -m unittest $< 2>&1 | tee $(LOG_DIR)/$@.py.out

$(SYSTEST_TARGETS): test_%: $(SYSTEST_DIR)/test_%.py $(SWIG_LIB) $(PY_LIB)
	export PYTHONPATH=$(LIB_DIR); python3 -m unittest $< 2>&1 | tee $(LOG_DIR)/$@.py.out

unittest: $(SWIG_LIB) $(PY_LIB)
	export PYTHONPATH=$(LIB_DIR); python3 -m unittest discover -s $(UTEST_DIR) -p "test_*.py" -v 2>&1 | tee $(LOG_DIR)/$@.out

systemtest: $(SWIG_LIB) $(PY_LIB)
	export PYTHONPATH=$(LIB_DIR); python3 -m unittest discover -s $(SYSTEST_DIR) -p "test_*.py" -v 2>&1 | tee $(LOG_DIR)/$@.out

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(LOG_DIR)

debug: CXXFLAGS += -DNDEBUG -g
debug: all

release: CXXFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(LIB_DIR)/*
	-@rm -rvf $(LOG_DIR)/*
	-@rm -rvf */__pycache__

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
