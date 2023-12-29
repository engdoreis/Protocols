
DOXYGEN_DIR              ?= 
DOXYGEN                  := $(DOXYGEN_DIR)doxygen

.PHONY: doc all transport command


all: transport command doc

transport:
	
	@printf "\nBuilding TransportProtocol...\n"
	@cd TransportProtocol && $(MAKE) clean static PROFILE=release CPU=x86

command:
	@printf "\nBuilding TemplateCommand...\n"
	@cd TemplateCommand && $(MAKE) clean static PROFILE=release CPU=x86
doc:
	@printf "\nBuilding documentation...\n"
	@cd doxygen && $(DOXYGEN) Doxyfile