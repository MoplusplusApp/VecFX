INC=-I\home\ravi-arora\workStuff\MoPlusPlus\MoPlusPlus\include
LIB=-L\home\ravi-arora\workStuff\MoPlusPlus\MoPlusPlus\lib

application: SVGObject.o
	g++ -static SVGObject.o -lpugixml -o VecFX && rm *.o && chmod +x VecFX && ./VecFX

SVGObject.o:
	g++ -static -c SVGObject.cpp $(INC) -o  SVGObject.o



