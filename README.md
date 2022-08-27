# Bored Game engine

A simple game engine designed to build board games. Started as a learning project and should remain as is in the future.

## Documentations

Well, still new and have not finished the 0.0 version, so focusing on building.
Also, does not have an editor YET, so the development process is a bit tidious. Just saying.

## Installation

Download [Visual Studio](https://visualstudio.microsoft.com/downloads/), latest version is recommended.

Clone this repo

```bash
git clone https://github.com/duc0905/BoredGameEngine.git MyGame
```

Open the "BoredEngine.sln". There are a couples of projects in the soluton.

### The Client project

The Client contains the code to build the game. 

### The ClientUnitTest project

The ClientUnitTest is used to test your Client project, but does not mandatory.

# Quick start

// TODO

# Contribution notes

The Engine project is using Precompiled header (pch.h and pch.cpp). However, there are some small issue with it.

Each time you create a new .cpp file:
- Need to have #include "pch.h" even if the pch.h file is not in the same directory
- The IDE will show an error of cannot include "pch.h", file not found
- No need to worry, if you need intellisense, you can include pch.h file in your .h file

# Authors

- [Duc Le](https://github.com/duc0905)
- [Hoang Nguyen](https://github.com/kipiiler)
