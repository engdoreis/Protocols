#*********************************************************************************************
# @file     
# @project  
# @platform 
# @device   
# @author   Douglas Reis
# @version  0.1
# @date     06-Feb-2019
# @brief    Configure the source to be use during compilation of the Project    
#
# @history
#
# When         Who               What
# -----------  ----------------  -------------------------------------------------------------
# 06-Feb-2019  Douglas Reis     - Initial Version. 
# ********************************************************************************************

MKFILE_DIR      := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
DIR_NAME        := $(notdir $(patsubst %/,%,$(MKFILE_DIR)))

CPU             ?= x86
PROFILE         ?= release

include  $(MKFILE_DIR)/make/functions.mk
-include $(MKFILE_DIR)/make/help.mk
include  $(MKFILE_DIR)/make/config.mk
include  $(MKFILE_DIR)/make/cpu/$(CPU).mk
include  $(MKFILE_DIR)/make/profile/$(PROFILE).mk
include  $(MKFILE_DIR)/make/dependencies.mk


SOURCE_HOME   ?= src

TARGET_NAME   ?= $(DIR_NAME)
BUILD_DIR     ?= ~build/$(CPU)/$(PROFILE)


DEPENDENCIES_LIB_OBJ      := $(wildcard $(LIB_DIR)/lib*.a)
EXTRACTED_OBJ_DIR         := $(BUILD_DIR)/$(DEPENDENCIES_NAME)


# INCDIRS       += $(shell find $(SOURCE_HOME) -type d)
INCDIRS       += $(call rdwildcard,$(SOURCE_HOME)/,*)

# SRC           := $(shell find $(SOURCE_HOME) -type f -name "*.c")
SRC           := $(call rfwildcard,$(SOURCE_HOME)/,*.c)

OBJ           := $(addprefix $(BUILD_DIR)/, $(SRC))
OBJ           := $(OBJ:.c=.o)

INCFLAGS      := $(addprefix -I, $(INCDIRS)) 

CFLAGS        += -Wall -Werror -fdata-sections -ffunction-sections

.PHONY: clean static help

static: prerequisites log extract $(BUILD_DIR)/lib$(TARGET_NAME).a	

$(BUILD_DIR)/lib$(TARGET_NAME).a: $(OBJ)
	@mkdir -p $(dir $@)	
	@echo "Generating $@..."
	@$(eval EXTRACTED_OBJ := $(wildcard $(EXTRACTED_OBJ_DIR)/*))
	@$(AR) -rcs $@ $^ $(EXTRACTED_OBJ)
	@printf "Done! \n"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)	
	
#Extract objects from lib
extract: $(subst $(LIB_DIR),$(EXTRACTED_OBJ_DIR),$(DEPENDENCIES_LIB_OBJ))


$(EXTRACTED_OBJ_DIR)%.a: $(LIB_DIR)%.a
	@mkdir -p $(dir $@)
	cp $< $@ 2>/dev/null || :
	cd $(@D);$(AR) -x $(@F);mv *.o -t 2>/dev/null || :
	rm -f $@


test: static servertest clienttest
	python3 test/AutoTest.py $(BUILD_DIR)/servertest.exe $(BUILD_DIR)/clienttest.exe

servertest: test/src/Server.c test/src/Porting.c test/src/circular_buffer.c
	@echo "Generating $@..."
	@$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@.exe $(INCFLAGS) $(BUILD_DIR)/lib$(TARGET_NAME).a

clienttest: test/src/Client.c test/src/Porting.c test/src/circular_buffer.c
	@echo "Generating $@..."
	@$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@.exe $(INCFLAGS) $(BUILD_DIR)/lib$(TARGET_NAME).a
	
clean:
	$(RM) $(BUILD_DIR)/
	
log:
	@printf "\nBuilding $(TARGET_NAME) whith the folowing setup:\n"
	@printf "CPU      : $(CPU)\n"
	@printf "PROFILE  : $(PROFILE)\n"
	@printf "CC       : $(CC)\n"
	@printf "CFLAGS   : $(CFLAGS)\n"
	@printf "INCFLAGS : $(INCFLAGS)\n\n"

prerequisites: dependenciesRequisites cpuRequisites profileRequisites
