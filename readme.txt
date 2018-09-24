////////////////
// HOW TO RUN //
////////////////

1. EXTRACT the project folder 'CPSC453Boilerplate' 
2. COMPILE program using the command line 'sh compile.sh'
    // had to part from using the make file due to the usage opengl
    // version 4.3 instead of 4.1. this caused some problems with
    // the provided make file.
    // this command line makes a .o file called 'raytracer.o'
3. RUN the program using the command line './raytracer.o'

////////////////
// HOW TO USE //
////////////////

P   :   take screen shot at any scene

SCENE 1 AND 2 ARE ASSIGNMENT SCENES
ASDW : MOVE CONTROLS
R/F  : ELEVATION CONTROL
CLICK AND DRAG TO LOOK AROUND
SCROLL WHEEL TO CHANGE FIELD OF VIEW
G TO RESET TO DEFAULT FIELD OF VIEW

SCENE 3 IS DEPTH OF FIELD BONUS
UP/DOWN ARROW adjust aperture size

SCENE 4 IS CREATIVE SCENE

SCENES 1 AND 2 RENDER IN REAL TIME
SCENE 3 MAY BE SLOW DUE TO SEVERAL SAMPLING
SCENE 4 NOT REAL TIME CAUSE RAYTRACING 36000+ TRIANGLES IS NUTS

/////////////////
// IMAGE FILES //
/////////////////

scene0.tga  assignment scene
scene1.tga  assignment scene
scene2.tga  depth of field scene
scene3.tga  CREATIVE SCENE
