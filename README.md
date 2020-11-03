# Raytracer

## Introduction
Raytracing project first developed for the “Advanced Graphics” course of [Tobias Isenberg](https://tobias.isenberg.cc/) at Polytech Paris-Sud that I continued after its end. The purpose of this project is to discover the raytracing process in graphics and its differences with the classic “shading” graphics programming.

## Requirement

* Windows 10
* Visual Studio 2017 (or above)

## Build

**NOTE : Currently, this project can only be build on Windows 10**

To first build the raytracer, you will need to do the followings : 
1. Clone the  github repository (https://github.com/T4ouf/Raytracer.git)
2. Execute "generateProject.bat" (either with a double-click or with a terminal)
3. Launch the Visual Studio Solution ("Raytracer.sln")
4. Generate the solution preferabily in "Release" mode (Check the mode then press CTRL+SHIFT+B)

=> No error should appear : the build is a success !

## Run

Everytime the solution is generated, the .exe binary is copied to the folder "Raytracer\rendering\". There you will find a lot of scene examples (.yaml) and .bat files to run if you want to see some examples.

If you look at the .bat files, you will see that all they do is calling the .exe binary with a few good arguments. The run command is the following : 
```console
PathOfYourCmd> [PathToExe]/Raytracer.exe [pathToYamlInputScene]/inputScene.yaml (Optional)ImageOutputName
```
**Note : all the textures, models, etc. data path are hardcoded in the yaml scene. Be sure to have them at the right place (relative to the .yaml file) before to run the scene.**

An example for run command can be : 
```console
C:\prog\Raytracer\Raytracer\rendering> Raytracer.exe sceneRainbowHelixGalaxy.yaml
```
 
 ## Documentation
 
 All the documentation can be found in the "doc\" folder. It contains image examples, some comparatives, the scene format explanation and the doxygen documentation of the code. The code is also completely commented.