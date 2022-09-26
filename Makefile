FEATURETOOL = opp_featuretool
FEATURES_H = src/iblock/features.h

.PHONY: all clean cleanall makefiles makefiles-so makefiles-lib makefiles-exe checkmakefiles

all: checkmakefiles $(FEATURES_H)
	@cd src && $(MAKE)

clean: checkmakefiles
	@cd src && $(MAKE) clean

cleanall: checkmakefiles
	@cd src && $(MAKE) MODE=release clean
	@cd src && $(MAKE) MODE=debug clean
	@rm -f src/Makefile $(FEATURES_H)

MAKEMAKE_OPTIONS := -f --deep -o IBLOCK -O out -pINET -I.

makefiles: makefiles-so

makefiles-so: $(FEATURES_H)
	@FEATURE_OPTIONS=$$($(FEATURETOOL) options --folder-options --linker-options) && cd src && opp_makemake --make-so $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS

makefiles-lib: $(FEATURES_H)
	@FEATURE_OPTIONS=$$($(FEATURETOOL) options --folder-options --linker-options) && cd src && opp_makemake --make-lib $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS

makefiles-exe: $(FEATURES_H)
	@FEATURE_OPTIONS=$$($(FEATURETOOL) options --folder-options --linker-options) && cd src && opp_makemake $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '================================================================='; \
	echo 'Error: src/Makefile not found, please run 'make makefiles' first!'; \
	echo '================================================================='; \
	echo; \
	exit 1; \
	fi

$(FEATURES_H): $(wildcard .oppfeaturestate) .oppfeatures
	@$(FEATURETOOL) defines >$(FEATURES_H)

doc:
	@cd doc && $(MAKE)
