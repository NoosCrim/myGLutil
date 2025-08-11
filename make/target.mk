# global input variables:
#	COMPILER_FLAGS
#	LINKER_FLAGS
#	OBJ_DIR
#	DEP_DIR
#	OUT_DIR
#	LOG_DIR - if empty logs will be written to console

# general target optional input vars:
#	TARGET_OUT_DIR
#	TARGET_NAME

# compile input vars:
# 	TARGET_SRC_DIR
#	TARGET_COMPILE_DEPS
#	TARGET_COMPILER_FLAGS
#	TARGET_HEADER_DIRS

# link input vars:
#	TARGET_LINK_FILES or TARGET_LINK_DIR
#	TARGET_LINKER_FLAGS
#	TARGET_LINK_DEPS

# test input vars:
#	TARGET_NAME
#	TARGET_LINKS_FILES
#	TESTED_DIR (the directory where code to test is located. Test code should be located in it's test subdirectory)
#	and all the ones for compile and link except ones that specify SRCs 
#		because you need to compile a test and link againist the file with what you're testing, but SRC should be only one and match test name

# the test src file should be <target_name>.cpp
#	it should contain test_function function, that will be used for testing, and use resources provided in test.hpp for checking if everything works
# they should also be located in $(TESTED_DIR)/test

# test .o files will be placed in $(OUT_DIR)/$(OBJ_DIR)/$(TEST_DIR) with name being <target_name>.o
# test executables will be placed in $(OUT_DIR)/$(TEST_DIR)
# generally tests should consist of only one .cpp file, but if necessary, you can create a compile target for more and link againist it's output

STATUS_MSG_PREFIX = [\033[33mSTATUS\033[0m]
SUCCESS_MSG_PREFIX = [\033[32mSUCCESS\033[0m]
ERROR_MSG_PREFIX = [\033[31mERROR\033[0m]
FAILED_MSG_PREFIX = [\033[31mFAILED\033[0m]

OBJ_DIR ?= obj
DEP_DIR ?= deps
OUT_DIR ?= out
TEST_DIR ?= test
COMPILER_FLAGS ?=
LINKER_FLAGS ?= 
DEFAULT_TARGET_TYPE ?= OBJECT # OBJECT | STATIC | SHARED | EXEC

# general dir recipe
%/:
	mkdir -p $@

TARGET_SUFFIX = $(strip\
$(if $(filter $(TARGET_TYPE),OBJECT),.o,\
$(if $(filter $(TARGET_TYPE),STATIC),.a,\
$(if $(filter $(TARGET_TYPE),SHARED),.so,\
$(if $(filter $(TARGET_TYPE),EXEC),,\
$(error Unknown TARGET_TYPE: $(TARGET_TYPE)))))))

TARGET_PREFIX = $(strip\
$(if $(filter $(TARGET_TYPE),OBJECT),,\
$(if $(filter $(TARGET_TYPE),STATIC),lib,\
$(if $(filter $(TARGET_TYPE),SHARED),lib,\
$(if $(filter $(TARGET_TYPE),EXEC),,\
$(error Unknown TARGET_TYPE: $(TARGET_TYPE)))))))


COMPILE_TARGETS := 

define add_target_compile
$(if $(TARGET_NAME), , $(error TARGET_NAME empty in compile target definition))
COMPILE_TARGETS := $(COMPILE_TARGETS) $(if $(filter $(COMPILE_TARGETS), $(TARGET_NAME)), $(error More than one definition of compile target $(TARGET_NAME)), $(TARGET_NAME))
endef

LINK_TARGETS := 

define add_target_link
$(if $(TARGET_NAME), , $(error TARGET_NAME empty in link target definition))
LINK_TARGETS := $(LINK_TARGETS) $(if $(filter $(LINK_TARGETS), $(TARGET_NAME)), $(error More than one definition of link target $(TARGET_NAME)), $(TARGET_NAME))
endef

TEST_TARGETS := 

define add_target_test
$(if $(TARGET_NAME), , $(error TARGET_NAME empty in link target definition))
TEST_TARGETS := $(TEST_TARGETS) $(if $(filter $(TEST_TARGETS), $(TARGET_NAME)), $(error More than one definition of test target $(TARGET_NAME)), $(TARGET_NAME))
endef

define reset_target
TARGET_NAME :=
TARGET_SRC_DIR :=
TARGET_LINK_DIR :=
TARGET_LINK_FILES :=
TARGET_OBJ_DIR := $(OBJ_DIR)
TARGET_DEP_DIR := $(DEP_DIR)
TARGET_OUT_DIR := 
TARGET_COMPILER_FLAGS := $(COMPILER_FLAGS)
TARGET_LINKER_FLAGS := $(LINKER_FLAGS)
TARGET_TYPE := $(DEFAULT_TARGET_TYPE)
TARGET_LINK_DEPS :=
TARGET_COMPILE_DEPS :=
endef

$(eval $(call reset_target))

define clean_target
$(if $(filter $(COMPILE_TARGETS), $(TARGET_NAME))$(filter $(LINK_TARGETS), $(TARGET_NAME)), , $(error Tried to create clean target for a target that does not exist: $(TARGET_NAME)))
PHONY.: $(TARGET_NAME)_clean $(TARGET_NAME)_clean_msg
$(TARGET_NAME)_clean: $(TARGET_NAME)_clean_msg
	rm -rf ./$$($(TARGET_NAME)_OUT_DIR)/$(TARGET_NAME)
$(TARGET_NAME)_clean_msg:
	@printf "$(STATUS_MSG_PREFIX) Cleaning $(TARGET_NAME)...\n"
endef

all:
tests:

define tests_target
tests: tests_msg $(TEST_TARGETS)
tests_msg:
	@printf "$(STATUS_MSG_PREFIX) Running all tests...\n"
endef

define all_target
all: all_msg $(COMPILE_TARGETS) $(LINK_TARGETS) tests
all_msg:
	@printf "$(STATUS_MSG_PREFIX) Building all targets...\n"
endef

define compile_all_target
compile_all: compile_all_msg $(addsuffix _compile, $(COMPILE_TARGETS))
compile_all_msg:
	@printf "$(STATUS_MSG_PREFIX) Compiling all targets...\n"
endef

define link_all_target
link_all: link_all_msg $(addsuffix _link, $(LINK_TARGETS))
link_all_msg:
	@printf "$(STATUS_MSG_PREFIX) Linking all targets...\n"
endef


PHONY.: all tests all_msg tests_msg compile_all_target compile_all_msg


include make/compile_target.mk
include make/link_target.mk
include make/test_target.mk
