################################################################################
# Automatically-generated file.  Do not edit directly; use project properties.
################################################################################


com.palmsource.eclipse.palmos4dev.compiler.m68k_palmos_C_SRCS += \
${addprefix $(ROOT)/src/, \
AppMain.c \
AppUtilities.c \
DbUtilities.c \
MusicLibUtilities.c \
TableUtilities.c \
}

src/%.o: $(ROOT)/src/%.c
	@echo
	m68k-palmos-gcc -c -D__PALMOS__=0x05000000 -I"$(TOOLS_DIR)include/stlport" -O0 -g -fexceptions -UNDEBUG -Wall $(subst sdk-,-palmos,$(SDK_VERSION)) -o $@  $<
	echo -n $(@:%.o=%.d) 'src/' > $(@:%.o=%.d) && \
	m68k-palmos-gcc -c -MM -MG -P -w -D__PALMOS__=0x05000000 -I"$(TOOLS_DIR)include/stlport" -O0 -g -fexceptions -UNDEBUG -Wall $(subst sdk-,-palmos,$(SDK_VERSION))  $< >> $(@:%.o=%.d)


