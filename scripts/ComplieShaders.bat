@echo off
set /p NAME="Podaj dopisek do nazwy: "

cd ..
call external\bin\glslc\glslc.exe shaders\%NAME%Shader.vert -o shaders\%NAME%Vert.spv
call external\bin\glslc\glslc.exe shaders\%NAME%Shader.frag -o shaders\%NAME%Frag.spv
pause