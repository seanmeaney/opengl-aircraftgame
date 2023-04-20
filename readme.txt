Compile & execution instructions
(requires opengl, GLFW & SOIL): 
    mkdir build
    cd build
    cmake ..
    make
    ./fp

Controls:
    W to move forward
    S to move backward
    A to rotate counter-clockwise
    D to rotate clockwise
    E to fire a bullet
    Left mouse button to fire a rocket 
        (if you have collected any and an ememy is targeted with cursor)

Weapons:
    bullets
    rockets

Collectibles:
    rockets
    parts

End-game:
    collect the 4 rocket parts
    they are located at +-3 game coordinates because i didnt have time for a better hud and would be hard to find
