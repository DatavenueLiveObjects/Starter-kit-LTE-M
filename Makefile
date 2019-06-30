TARGETS := ar7 wp76xx wp77xx ar86 wp85 localhost

.PHONY: all $(TARGETS)
all: $(TARGETS)

$(TARGETS):
	export MANGOH_BOARD=red ; \
	export TARGET=$@ ; \
	mksys -v -t $@ \
		-i $(LEGATO_ROOT)/interfaces/dataConnectionService \
		-i $(LEGATO_ROOT)/interfaces/modemServices \
		-i $(LEGATO_ROOT)/interfaces/positioning \
		Orange.sdef

clean:
	rm -rf _build_* *.ar7 *.wp7 *.ar86 *.wp85 *.localhost *.update

