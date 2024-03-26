@echo off
if not exist Visualizer\vendor\assimp\build call Visualizer\vendor\BuildAssimp.bat

call premake\premake5.exe vs2022
PAUSE