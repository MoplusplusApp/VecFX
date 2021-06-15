INC=-I\include

application: SVGObject.o
	g++ -static SVGObject.o -lpugixml -o VecFX && rm *.o && chmod +x VecFX && ./VecFX

SVGObject.o:
	g++ -static -c SVGObject.cpp $(INC) -o  SVGObject.o



