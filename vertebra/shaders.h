#ifndef SHADERS_H
#define SHADERS_H

/*
 * This file contains the source code of the GLSL shaders
 */

const char* vertexColorShaderSource =
        "const vec3 coolest = vec3(0.0, 1.0, 1.0);"
        "const vec3 hottest = vec3(1.0, 0.0, 0.0);"
        "void main()"
        "{"
        "	vec3 color = mix(coolest, hottest, gl_Color[0]);"
        "	gl_FrontColor = vec4(color, gl_Color[0]);"
        "	gl_BackColor = gl_FrontColor;"
        "	gl_Position = ftransform();"
        "}"
        ;

#endif // SHADERS_H
