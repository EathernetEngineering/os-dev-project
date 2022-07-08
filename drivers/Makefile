all: portio graphics terminal
.PHONY: all

.PHONY: portio
portio:
	$(MAKE) -C portio/

.PHONY: terminal
terminal:
	$(MAKE) -C terminal/

.PHONY: graphics
graphics:
	$(MAKE) -C graphics/

.PHONY: cleanall cleanportio cleanterminal cleangraphics

cleanall: cleanportio cleanterminal cleangraphics

cleanportio:
	$(MAKE) -C portio/ clean

cleanterminal:
	$(MAKE) -C terminal/ clean

cleangraphics:
	$(MAKE) -C graphics/ clean

