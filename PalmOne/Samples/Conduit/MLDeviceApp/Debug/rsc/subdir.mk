################################################################################
# Automatically-generated file.  Do not edit directly; use project properties.
################################################################################


com.palmsource.eclipse.tools.compiler.palmrc_XRD_SRCS += \
${addprefix $(ROOT)/rsc/, \
AppResources.xrd \
}

rsc/%.trc: $(ROOT)/rsc/%.xrd
	@echo
	"$(TOOLS_DIR)PalmRC" -locale enUS -target 4.0 -strictTextEncoding -quiet -p PalmOS4 -o $@  $<
	echo -n $(@:%.trc=%.d) $@ : ' ' > $(@:%.trc=%.d) && \
	"$(TOOLS_DIR)PalmRC" -quiet -makedeps $(@:%.trc=%.d).tmp $< && \
	/usr/bin/cygpath -m -f $(@:%.trc=%.d).tmp | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.trc=%.d) && \
	rm -rf $(@:%.trc=%.d).tmp



