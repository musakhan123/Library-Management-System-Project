@echo off
g++ src/main.cpp include/*.cpp imgui/*.cpp ^
-o LMS.exe ^
-Iinclude -Iimgui -IC:\libs\glfw\include ^
-LC:\libs\glfw\lib-mingw-w64 ^
-lglfw3 -lopengl32 -lgdi32 -mwindows
pause
