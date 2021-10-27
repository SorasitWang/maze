#version 330 core

uniform bool draw;
void main()
{          
   
        gl_FragDepth = gl_FragCoord.z;
    }