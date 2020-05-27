################################################################################
# Automatically-generated file.  Do not edit directly; use project properties.
################################################################################




com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_o_OBJS := \
 $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_C_SRCS:$(ROOT)/%.c=%.o) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_CC_SRCS:$(ROOT)/%.cc=%.o) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_CPP_SRCS:$(ROOT)/%.cpp=%.o) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_CXX_SRCS:$(ROOT)/%.cxx=%.o) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_C_UPPER_SRCS:$(ROOT)/%.C=%.o)

com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_o_DEPS := \
 $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_C_SRCS:$(ROOT)/%.c=%.d) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_CC_SRCS:$(ROOT)/%.cc=%.d) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_CPP_SRCS:$(ROOT)/%.cpp=%.d) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_CXX_SRCS:$(ROOT)/%.cxx=%.d) $(com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_C_UPPER_SRCS:$(ROOT)/%.C=%.d)


MULTISECTION_OBJS := 
MULTISECTION_DEFINITIONS_FILE = 

ifeq ($(MULTISECTION_BUILD), true)
MULTISECTION_ASM_OBJS = multiple_code_sections.o
else
MULTISECTION_ASM_OBJS = 
endif



com.palmsource.eclipse.tools.compiler.palmrc_trc_OBJS := \
 $(com.palmsource.eclipse.tools.compiler.palmrc_XRD_SRCS:$(ROOT)/%.xrd=%.trc) $(com.palmsource.eclipse.tools.compiler.palmrc_XRD_UPPER_SRCS:$(ROOT)/%.XRD=%.trc)

com.palmsource.eclipse.tools.compiler.palmrc_trc_DEPS := \
 $(com.palmsource.eclipse.tools.compiler.palmrc_XRD_SRCS:$(ROOT)/%.xrd=%.d) $(com.palmsource.eclipse.tools.compiler.palmrc_XRD_UPPER_SRCS:$(ROOT)/%.XRD=%.d)


