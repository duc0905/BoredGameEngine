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

The Engine project is using Precompiled header (pch.h and pch.cpp). However, there are some small issue with it and here is how to work around.

Each time you create a new .cpp file:
- Need to have #include "path/to/pch.h"
- Right click on the newly created .cpp file and choose Properties
- Go to C/C++ -> Precompiled Headers
- Set "Precompiled Headers" to Use (/Yu), and "Precompiled Header file" to the path exactly like in the #include
- Done!

Sorry for the inconvenience, will find a way to solve this in the future.

# Authors

- [Duc Le](https://github.com/duc0905)
- [Hoang Nguyen](https://github.com/kipiiler)
