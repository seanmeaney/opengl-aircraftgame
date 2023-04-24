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

Enemies:
    red and green aircraft that shoot at you if you get to close

End-game:
    collect the 4 rocket parts
    they are located at +-3 game coordinates because i didnt have time for a better hud and would be hard to find
