# The-Game-of-Life
Simulation of the Game of Life in Python and C.

The Game of Life is a cellular automaton whose evolution is determined just by an initial state of living and dead cells.

The game rules:

1) Each cell has 8 neighbors: 1 top, 1 bottom, 1 left, 1 right, and 4 in the diagonal corners.
2) Any living cell with less than 2 or more than 3 neighbors dies. Other living cells survive.
3) Any dead cell with exactly 3 living neighbors comes alive.

These simple rules give rise to many interesting and complicated patterns. This repository contains the source code in C and Python. For animations in C, I used raylib (https://github.com/raysan5/raylib). For Python, I used matplotlib.

Below are some interesting patterns. Feel free to explore more of your own!

In order to build (on MacOS), cd to the directory 'C' and then enter 'make; ./game_of_life_raylib' in your terminal.

C simulations:

1) Queen bee shuttle

![Queen_bee_shuttle](https://github.com/shubham-93/The-Game-of-Life/assets/47306775/fc6e8548-8088-4422-9e5b-ffb6664dd148)

2) Two-glider octomino

![Two-glider octomino](https://github.com/shubham-93/The-Game-of-Life/assets/47306775/f64f300a-2d9d-4541-8701-829e860fa91e)

3) Pi-heptomino

![Pi-heptomino](https://github.com/shubham-93/The-Game-of-Life/assets/47306775/6ad3c351-322b-4bf2-a669-643ab48c684d)

4) Pentadecathlon
   
![Pentadecathlon](https://github.com/shubham-93/The-Game-of-Life/assets/47306775/bdadfa63-4db5-4439-8f59-b02e65e47427)

5) Aries betwixt two blocks

![Aries betwixt two blocks](https://github.com/shubham-93/The-Game-of-Life/assets/47306775/73cb9c83-3f16-42f4-9b91-927106fc85c0)




The blinker in Python:

![game_of_life](https://github.com/shubham-93/The-Game-of-life/assets/47306775/49c3246e-6e14-4460-8847-4c497b705cb7)


References:
1. https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
2. https://conwaylife.com/wiki/Conway's_Game_of_Life
