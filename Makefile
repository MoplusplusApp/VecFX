BUILDDIR = ./build/
OBJS = build/SVGObject.o build/main.o
SOURCE = src/SVGObject.cpp src/main.cpp
HEADER	= src/SVGObject.hpp
LDLIBS += -lpugixml
FLAGS = -c -Wall -static
# -g option enables debugging mode 
# -c flag generates object code for separate files
# -Wall adds more warnings for debugging
OUT = VecFX
CC = g++
INCLUDE = -I./include/
RM = /usr/bin/rm -rf
MKDIR = /usr/bin/mkdir -p

build: $(OBJS)
	$(CC) -static -g $(OBJS) $(LDLIBS) -o $(BUILDDIR)$(OUT)
	$(RM) $(BUILDDIR)*.o
	chmod +x $(BUILDDIR)$(OUT)
	@echo Build completed successfully!
	@echo Executable located at $(BUILDDIR)$(OUT)

build/SVGObject.o: src/SVGObject.cpp
	@$(MKDIR) $(BUILDDIR)
	@echo Build dir located at $(BUILDDIR)
	$(CC) $(FLAGS) $(INC) src/SVGObject.cpp -o build/SVGObject.o

build/main.o: src/main.cpp
	@$(MKDIR) $(BUILDDIR)
	@echo Build dir created at $(BUILDDIR)
	$(CC) $(FLAGS) $(INC) src/main.cpp -o build/main.o

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
