LOG_DIR := log

LIB_NAME := myRayLib
EXEC_NAME := raytracer

COMPILER_FLAGS := -Wall -Werror -std=c++23 -g
LINKER_FLAGS := 
# glsl_preprocess
$(eval $(call reset_target))

TARGET_NAME := glsl_preprocess

TARGET_SRC_DIR := src/glsl_preprocess
$(eval $(call compile_target))

TARGET_TYPE := EXEC
$(eval $(call link_target))

$(eval $(call clean_target))

# mGLu lib
$(eval $(call reset_target))

TARGET_NAME := mGLu

TARGET_SRC_DIR := src/
TARGET_HEADER_DIRS := headers/
$(eval $(call compile_target))

TARGET_TYPE := STATIC
$(eval $(call link_target))

$(eval $(call clean_target))

# mGLu example
$(eval $(call reset_target))

TARGET_OUT_DIR := $(mGLu_OUT_DIR)/mGLu_example/
TARGET_NAME := mGLu_example

TARGET_SRC_DIR := example/
TARGET_HEADER_DIRS := $(TARGET_SRC_DIR) ./

$(eval $(call compile_target))

TARGET_LINKER_FLAGS = $(LINKER_FLAGS) -lGL -lglfw -lGLEW
TARGET_TYPE := EXEC
TARGET_LINK_DEPS := mGLu
TARGET_LINK_FILES := $(mGLu_OUT_FILE)
$(eval $(call link_target))
$(eval $(call clean_target))