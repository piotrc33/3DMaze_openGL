default: labirynt3D

%: %.cpp
	g++ -I. $< -o $@ AGL3Window.cpp  -lepoxy -lGL -lglfw 
clean:
	rm a.out *.o *~ labirynt3D
