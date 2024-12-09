# Rects Battle
![Language (C++)](https://img.shields.io/badge/powered_by-C++-brightgreen.svg?style=flat-square) 

This is game project made by me during COMP lessons of SFML and ImGui on C++.

## Controls

Use **WASD** to control player's. Left mouse button to shoot. Every few seconds you will shoot with black
hole. Shoot enemies, get points, when enemy dies it spawns smaller enmies that last few seconds, destory
them too to get points.  

Use ImGUI panel to control game states, entities.  

![Screenshot](Screenshots/ImGUIPanel.png?raw=true "Start of game")
- **Systems** : turn on/off collisions, player and enemies movement, and enemy spawner.
Also you can manualy spawn an enemy, and change enemy spawn rate
- **All Ent** : Is list of all entites, you can delete them by pressing their ID button
- **Grouped** : Is all entites grouped by their tag

## Screenshots
![Screenshot](Screenshots/g_start.png?raw=true "Start of game")

![Screenshot](Screenshots/g_inProgress.png?raw=true "Game in progress")

### Must Have (dependencies)
- A valid C++ compiler - clang, gcc, msvc that supports c++17
- cmake >= v3.15

## Building

The CMAKE is set up to automatically download the dependencies and link against it. It will be done when configuring cmake.

```
git clone https://github.com/FroGGog/RectsBattle.git
cd RectsBattle
cmake build .
```

Feel free to fork, fix the issues and contribute. Help is always welcome.
