/* VERTEBRA - Volumetric Examiner for Radiological/Tomographical Experimental Basic Realtime Analysis
   Copyright (C) 2010 Uli Koehler

   VERTEBRA is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   VERTEBRA is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with VERTEBRA; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301 USA */

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
