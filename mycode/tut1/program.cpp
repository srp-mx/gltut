#include <glload/gl_3_3.h>
#include <GL/freeglut.h>

#include <string>
#include <algorithm>
#include <vector>

// The program object we're using
static GLuint theProgram;

// Vertex position as 'buffer object', ie GPU memory
static GLuint positionBufferObject;
static GLuint vertexArrayObject;


// Triangle already in clip space
const float vertexPositions[] =
{
    0.75f, 0.0f, 0.0f, 1.0f,
    0.0f, -0.75f, 0.0f, 1.0f,
    -0.75f, 0.0f, 0.0f, 1.0f,


    0.75f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    0.375f, 0.375f, 0.0f, 1.0f,

    -0.75f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    -0.375f, 0.375f, 0.0f, 1.0f
};
// NOTE(srp): we could also calculate this, but this is a bit more explicit
static int triangleCount = 3;

// Passes main memory to video memory as 'buffer objects'
// Note that OpenGL still doesn't know what this IS, it's just memory.
static void
InitializeVertexBuffer()
{
    // Creates the buffer object, but needs binding and memory
    glGenBuffers(1, &positionBufferObject);

    // Binds to The Context (TM)
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject); 
    // Allocates and fills GPU memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    // Unbinds from GL_ARRAY_BUFFER. Not necessary.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Shaders at vertex processing (vertex shaders)
//  They output, among user-defined stuff, a clip-space
//  position for the passed in vertex.
static const std::string strVertexShader = 
    #include "data/vertex.vert"
;
static const std::string strFragmentShader =
    #include "data/fragment.frag"
;

GLuint
CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
    // Creates the object, a shader of a particular type
    GLuint shader = glCreateShader(eShaderType);
    const char *strFileData = strShaderFile.c_str();

    // A shader "source code object"
    // Params: shader object to put string into, number of strings, 
    //  strings, array of length of strings (NULL for null termination)
    glShaderSource(shader, 1, &strFileData, NULL);

    // Compiles the shader into the object
    glCompileShader(shader);

    GLint status;
    // Checks whether compilation was successful (GL_COMPILE_STATUS)
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;

        switch (eShaderType)
        {
            case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
            case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
            case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }

    return shader;
}

// Links shader objects into a program object
GLuint
CreateProgram(const std::vector<GLuint> &shaderList)
{
    // Creates empty program
    GLuint program = glCreateProgram();

    for (size_t i = 0; i < shaderList.size(); i++)
    {
        // Attaches all shaders
        glAttachShader(program, shaderList[i]);
    }

    // Links the program
    glLinkProgram(program);

    GLint status;
    // Checks if linkage was successful, compare with CreateShader function
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        delete[] strInfoLog;
    }

    for (size_t i = 0; i < shaderList.size(); i++)
    {
        // Once linked, we can cleanup
        glDetachShader(program, shaderList[i]);
    }

    return program;
}

// The 'program' in this case has to do with how glsl compilation works,
// it compiles each shader into a 'shader object' (like C's .o files)
// and then links them together into a single 'program object'.
void 
InitializeProgramObject()
{
    // List of shaders to link together
    std::vector<GLuint> shaderList;
    // Shader compilation here
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

    theProgram = CreateProgram(shaderList);

    // Once all is done, cleanup the shader objects since they're no
    // longer needed once we have a program object.
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}


// Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void
init()
{
    // The shaders get compiled into a program
    InitializeProgramObject();
    // The triangle we're drawing gets passed to GPU memory
    InitializeVertexBuffer();

    // Generates and binds the vertices to The Context
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
}

// Called to update the display.
// You should call glutSwapBuffers after all of your rendering to display what you rendered.
// If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void
display()
{
    glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // This actually uses the program object we generated earlier
    glUseProgram(theProgram);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    // Enables attribute index 0
    glEnableVertexAttribArray(0);
    // This tells OpenGL how to interpret our 'buffer object'
    //  index 0, size 4, type float, normalized fixed point false, stride 0 (tightly packed), first element offset: 0
    // Note that it implicitly refers to whatever at 'GL_ARRAY_BUFFER'
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Draws a triangle from our 'glVertexAttribPointer'
    //  draw triangle, start index 0, elements 3 * triangleCount
    //      ^every 3 vertices makes a triangle
    glDrawArrays(GL_TRIANGLES, 0, 3 * triangleCount);

    // Disables attribute index 0
    glDisableVertexAttribArray(0);
    
    // Because double-buffering.. duh
	glutSwapBuffers();
}

// Called whenever the window is resized. The new window size is given, in pixels.
// This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void
reshape (int w, int h)
{
    int size = 300;
    // Viewport transform, Normalized Display Coordinates -> Window coordinates
    glViewport(w/2 - size/2, h/2 - size/2, (GLsizei) size, (GLsizei) size);
}

// Called whenever a key on the keyboard was pressed.
// The key is given by the ''key'' parameter, which is in ASCII.
// It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
// exit the program.
void 
keyboard(unsigned char key, int x, int y)
{

}

// Called before FreeGLUT is initialized. It should return the FreeGLUT
// display mode flags that you want to use. The initial value are the standard ones
// used by the framework. You can modify it or just return you own set.
// This function can also set the width/height of the window. The initial
// value of these variables is the default, but you can change it.
unsigned int 
defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}
