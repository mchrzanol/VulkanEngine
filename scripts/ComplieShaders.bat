cd ..
call external\bin\glslc\glslc.exe shaders\shader.vert -o shaders\vert.spv
call external\bin\glslc\glslc.exe shaders\shader.frag -o shaders\frag.spv
pause