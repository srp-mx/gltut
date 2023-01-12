R"====(

// NOTE(srp): Fragment shaders run at clip-space time
#version 330

// Remember glVertexAttribPointer? Here it is at play!!
// Note that there's usually a limit of 16 attribute indices
layout(location = 0) in vec4 position;
void main()
{
    // gl_Position: clip space position of the vertex
    gl_Position = position;
}

)===="
