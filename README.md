# LLGW

This is a cross-platform header-only C++ wrapper library for modern low level graphics APIs.

Warning: LLGW is under development and doesn't provide final product quality.

## Features
* OpenGL Shader Language abstraction class

## Usage

LLGW is a header-only library. However, depending on your compiler and the size of your program, you can choose to build in LLGW as a source file. You may want to do this to decrease the compilation times. This can be done in  the following steps:
* In only one of your files add #include <llgw/impl/src.hpp>
* Define LLGW_SEPARATE_COMPILATION macros in all your source files


The source code is published under BSD 2-clause, the license is available [here][license].

## Third-party
* GLM ([MIT License](https://github.com/g-truc/glm/blob/master/copying.txt))


[//]: # (LINKS)
[license]: LICENSE
