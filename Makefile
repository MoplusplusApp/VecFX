BUILDDIR = ./build/
OBJS = SVGObject.o
SOURCE = SVGObject.cpp
LDLIBS += -lpugixml
OUT = VecFX
CC = g++
INC= -I\include
RM = /usr/bin/rm -rf
MKDIR = /usr/bin/mkdir -p

build: $(OBJS)
	$(CC) -static $(BUILDDIR)$(OBJS) $(LDLIBS) -o $(BUILDDIR)$(OUT)
	$(RM) $(BUILDDIR)*.o
	chmod +x $(BUILDDIR)$(OUT)
	@echo Build completed successfully!
	@echo Executable located at $(BUILDDIR)$(OUT)

SVGObject.o: $(SOURCE)
	@$(MKDIR) $(BUILDDIR)
	@echo Build dir created at $(BUILDDIR)
	$(CC) -c -Wall -static $(SOURCE) $(INC) -o $(BUILDDIR)$(OBJS)

clean:
	$(RM) $(BUILDDIR)*.o
	@echo Build cleaned!

purge:
	@echo Are you sure you want to clean everything?
	@read -p "[Enter to confirm, CTRL C to stop]: "
	$(RM) $(BUILDDIR)

guide:
	@echo -e "This is the VecFX compiling help guide:\n"
	@cat BUILD.md
help:
	@echo Project options:
	@echo -e "\t Build objects only: make SVGObject.o"
	@echo -e "\t Build application (default): make build"
	@echo -e "\t Clean build artifacts: make clean"
	@echo -e "\t Clean entire build: make purge"
	@echo -e "\t Show help (you're looking at it right now): make help"
	@echo -e "\t For further help, run: make guide"
