
# Bored game engine

A simple game engine as a learning project to discover different areas of Computer Science such as Computer Graphics, Optimization, Audio, etc., and to practice different techniques in Software engineering.



## 💡 Tech Stack

- Language: C++
- Build tools: Clang, CMake, vcpkg, Doxygen
- Libraries: see `vcpkg.json`
- Testing library: Googletest
## 🚀 Features and roadmap

- Scene \& node
  - Each scene is a seperate map/level
  - Each scene has a Node-tree
  - Each scene has its own ECS system
- Rendering
  - [x] Orthographic \& perspective cameras
  - [x] Importing meshes
  - Material system
    - [x] Phong model
    - [ ] PBR
    - [ ] Texture mapping
    - [ ] Normal mapping
  - Lighting
    - [x] Directional light
    - [x] Point light
    - [ ] Spot light
  - [ ] Shadow mapping
- [ ] Audio
- [ ] Physics
- [ ] Interactive editor
- [ ] Serialize \& deserialize
- [x] Scripting
- [x] Input handling
- [ ] Networking
## ⚙️ Installation

**📋 Prerequisites**

- CMake >= 3.25.0
- Ninja build system
- Git
- Clang compiler

**🛠️ Build**

Clone the project

```bash
  git clone --recurse-submodules https://github.com/duc0905/BoredGameEngine.git
```

Go to the project directory

```bash
  cd ./BoredGameEngine
```

Configure the build system. Vcpkg will automatically download the dependencies in this step, so it might take a few minutes.

```bash
  cmake --preset base
```

Building CMake targets

```bash
  cmake --build --preset build
```

Currently, there are 3 targets:

- Engine - the static library containing core engine code
  - Directory: build/base/Engine/Engine.lib
- demo1 - the simple demo using the core functions of the engine
  - Directory: build/base/demo/demo1/demo1.exe
- demo2 - user-defined components and services demo
  - Directory: build/base/demo/demo2/demo2.exe
## 📖 Documentation

Use the custom CMake target to generate documentation using Doxygen:

```bash
  cmake --build --preset build --target gen_doc
```

The html documentation will be at `./build/base/docs/html/index.html`.


## 🎮 Usage

Once built, you can link the static library and use the header files in the Engine to your libraries and/or executables.


## 🤝 Contributing

Contributions are always welcome! Since this is my personal project and I would prefer implementing myself, I would love to receive suggestions and/or guidance on new ideas, feature, or bugs for the project. However, you are free to work on a copy of the project for your own.
## 📄 License

[MIT](https://choosealicense.com/licenses/mit/)

