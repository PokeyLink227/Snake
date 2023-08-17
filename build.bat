@echo off
C:\raylib\mingw\bin\gcc -o snake.exe snake.c C:\raylib\raylib\src\raylib.rc.data -s -static -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
