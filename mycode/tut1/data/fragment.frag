R"====(

// NOTE(srp): Fragment shaders run at window-space time, so
// the window-space coordinate of the fragment gets passed to us.

#version 330

// output for the fragment shader
out vec4 outputColor;
void main()
{
    // R G B x, this is just white
    outputColor = vec4(0.1f, 0.9f, 0.4f, 1.0f);
}

)===="
