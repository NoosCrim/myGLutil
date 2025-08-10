define compile_target
$(call add_target_compile)

$(TARGET_NAME)_OUT_DIR := $(if $(TARGET_OUT_DIR), $(TARGET_OUT_DIR), $(OUT_DIR)/$(TARGET_NAME))/
$(TARGET_NAME)_SRC = $(if TARGET_SRC_DIR, $$(wildcard $(TARGET_SRC_DIR)/*.cpp))

$(TARGET_NAME)_DEP_DIR := $$($(TARGET_NAME)_OUT_DIR)/$(DEP_DIR)/
$(TARGET_NAME)_DEP = $$(patsubst %.cpp, $$($(TARGET_NAME)_DEP_DIR)/%.d, $$(notdir $$($(TARGET_NAME)_SRC)))


$(TARGET_NAME)_OBJ_DIR := $$($(TARGET_NAME)_OUT_DIR)/$(OBJ_DIR)/
$(TARGET_NAME)_OBJ := $$(patsubst %.cpp, $$($(TARGET_NAME)_OBJ_DIR)/%.o, $$(notdir $$($(TARGET_NAME)_SRC)))
$(TARGET_NAME)_HEADER_DIRS := $(if $(TARGET_HEADER_DIRS), $(TARGET_HEADER_DIRS), $(TARGET_SRC_DIR))
$(TARGET_NAME)_COMPILER_FLAGS := $(if $(TARGET_COMPILER_FLAGS), $(TARGET_COMPILER_FLAGS), $(COMPILER_FLAGS)) $$(addprefix -I, $$($(TARGET_NAME)_HEADER_DIRS))

$(TARGET_NAME)_COMPILE_DEPS := $(TARGET_COMPILE_DEPS)

PHONY.: $(TARGET_NAME)_compile $(TARGET_NAME) $(TARGET_NAME)_compile_msg

$(TARGET_NAME): $(TARGET_NAME)_compile
	@:
$(TARGET_NAME)_compile: $$($(TARGET_NAME)_COMPILE_DEPS) $(TARGET_NAME)_compile_msg $$($(TARGET_NAME)_DEP_DIR) $$($(TARGET_NAME)_OBJ_DIR) $$($(TARGET_NAME)_OBJ)
	@:
$(TARGET_NAME)_compile_msg:
	@printf "$(STATUS_MSG_PREFIX) Compiling $(TARGET_NAME)...\n"

$$($(TARGET_NAME)_OBJ_DIR)/%.o: $(TARGET_SRC_DIR)/%.cpp 
	g++ -MMD -MP -MF $$($(TARGET_NAME)_DEP_DIR)/$(notdir $$*).d $$($(TARGET_NAME)_COMPILER_FLAGS) -c $$< -o $$@
-include $$($(TARGET_NAME)_DEP)

endef
