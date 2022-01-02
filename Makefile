
DOXYGEN_DIR              := d:/SW/doxygen/bin/
DOXYGEN                  := $(DOXYGEN_DIR)doxygen.exe

.PHONY: doc all transport command


all: transport command doc

transport:
	
	@printf "\nBuilding TransportProtocol...\n"
	@cd TransportProtocol && $(MAKE) clean static PROFILE=debug CPU=x86

command:
	@printf "\nBuilding TemplateCommand...\n"
	@cd TemplateCommand && $(MAKE) clean static PROFILE=debug CPU=x86
doc:
	@printf "\nBuilding documentation...\n"
	@cd doxygen && $(DOXYGEN) Doxyfile