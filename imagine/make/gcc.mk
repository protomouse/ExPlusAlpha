include $(currPath)/common.mk
include $(currPath)/gcc-link.mk
include $(currPath)/gcc-common.mk

ifdef O_LTO
 COMPILE_FLAGS += -flto
 LDFLAGS += $(COMPILE_FLAGS) $(WHOLE_PROGRAM_CFLAGS)
endif

gccVersion := $(shell $(CC) -dumpversion)

#WHOLE_PROGRAM_CFLAGS += -fipa-pta
BASE_CXXFLAGS += -std=gnu++0x
NORMAL_WARNINGS_CFLAGS += $(if $(ccNoStrictAliasing),,-Werror=strict-aliasing) -Wno-unused-parameter
#NORMAL_WARNINGS_CFLAGS += -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn

ifdef RELEASE
 COMPILE_FLAGS += -fno-ident
 BASE_CXXFLAGS += -fno-enforce-eh-specs
endif

ifndef RELEASE
 ifneq ($(ENV), ps3)
  COMPILE_FLAGS += -ggdb
 endif
endif

ifdef cxxExceptions
 BASE_CXXFLAGS += -fnothrow-opt
endif

HIGH_OPTIMIZE_CFLAGS_MISC += -funsafe-loop-optimizations -Wunsafe-loop-optimizations
HIGH_OPTIMIZE_CFLAGS := -O2 $(HIGH_OPTIMIZE_CFLAGS_MISC)
