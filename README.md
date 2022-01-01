# GTEngine
## _OpenGL game engine written in C_

## What works
- Loading models with textures

## Building
At the moment Windows/Wine build is not supported

Install [glfw](https://github.com/glfw/glfw) and [assimp](https://github.com/assimp/assimp)

- Debian/Ubuntu: libglfw3-dev libassimp-dev
- Alpine/PostmarketOS: glfw-dev assimp-dev musl-dev
- Arch: glfw assimp

Clone the repository
```
git clone --recurse-submodules --remote-submodules https://github.com/michaelskyf/GTEngine.git
```
If cloned withuout `--recurse-submodules --remote-submodules` run
```
git submodule update --init
```

Compile and run
```
make
make run
```

## Supported languages
 - C
 - C++

## Supported graphics APIs
- GLES 2.0
