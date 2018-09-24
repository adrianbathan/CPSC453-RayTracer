LIBS="-lglfw -lpthread -lm -lXi -lXrandr -lX11 -lXxf86vm -lXinerama -lXcursor -lGLU -ldl -lOpenGL "



g++  -std=c++0x middleware/glad/src/glad.c $LIBS -L/usr/lib/nvidia-384 ./boilerplate/*.cpp -Wall -g -Imiddleware/glm-0.9.8.2 -I"./middleware/glad/include" -I"./middleware"  -o "raytracer.o"
