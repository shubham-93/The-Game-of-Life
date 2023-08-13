/*
The Game of Life
Made with raylib

Author: Shubham Maheshwari
email: shubham.93@gmail.com
GitHub: https://github.com/shubham-93


Some interesting patterns to try:

Oscillators:
Queen bee shuttle: https://conwaylife.com/wiki/Queen_bee_shuttle
Pentadecathlon: https://conwaylife.com/wiki/Pentadecathlon

Methuselahs:
Two-glider octomino: https://conwaylife.com/wiki/Two-glider_octomino
Pi-heptomino: https://conwaylife.com/wiki/Pi-heptomino

Still lifes:
Aries betwixt two blocks: https://conwaylife.com/wiki/Aries_betwixt_two_blocks
Moose antlers: https://conwaylife.com/wiki/Moose_antlers
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"

// Each cell has 8 neighbors
#define NUM_NEIGHBORS 8


// Cell struct
typedef struct {
    int label;
    Rectangle rect_dim;
    int state;
    int new_state;
} cell;


// Struct for neighbor cells info
typedef struct {
    cell *neighbor_cells;
    int sum_neighbor_states;
    bool out_of_bounds_access; // to check if we try to access a cell out of the array bounds
} neighbor_info;


// Information about the 8 neighbors of an input cell
neighbor_info get_neighbor_info(cell input_cell, cell *all_cells, int num_rows, int num_columns, int num_cells) {

    neighbor_info neighbor_info;

    // The input cell whose 8 neighbors we want to find
    int input_cell_label = input_cell.label;

    // The labels (or indices, for accessing array elements) of the 8 neighbors
    int neighbor_labels[8] = {  input_cell_label - 1,
                                input_cell_label + 1,
                                input_cell_label + num_columns,
                                input_cell_label + num_columns - 1,
                                input_cell_label + num_columns + 1,
                                input_cell_label - num_columns,
                                input_cell_label - num_columns - 1,
                                input_cell_label - num_columns + 1 };

    // out_of_bounds = 0 if there was no out_of_bounds access. 1 otherwise.
    bool out_of_bounds_access = 0;

    // Check for out-of-bounds access. Return upon detection.
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        if ((neighbor_labels[i] < 0) || (neighbor_labels[i] >= num_cells)) {
            out_of_bounds_access = 1;
            neighbor_info.out_of_bounds_access = out_of_bounds_access;
            return neighbor_info;
        }
    }

    // Dynamically allocate memory for the 8 neighbors   
    cell* neighbor_cells = (cell*)malloc(sizeof(cell)*NUM_NEIGHBORS);

    // Assign values to neighbor_cells
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        neighbor_cells[i] = all_cells[neighbor_labels[i]];
    }

    // Find the sum of the 8 neighbor states
    int sum_neighbor_states = 0;
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        sum_neighbor_states += neighbor_cells[i].state;
    }

    neighbor_info.neighbor_cells = neighbor_cells;
    neighbor_info.sum_neighbor_states = sum_neighbor_states;
    neighbor_info.out_of_bounds_access = out_of_bounds_access; // out_of_bounds_access = 0 if the program arrives at this line anyway

    return neighbor_info;
}

// Update the states of each cell. Check if living cells may die, or if dead cells may come to life
bool update_cells(cell* all_cells, int num_cells, int num_rows, int num_columns) {

    for (int i = 0; i < num_cells; i++) {
        // Check if a cell is alive
        if (all_cells[i].state == 1) {
            neighbor_info neighbors;
            neighbors = get_neighbor_info(all_cells[i], all_cells, num_rows, num_columns, num_cells);
            if (neighbors.out_of_bounds_access == 1) return 1; // Return immediately if an out-of-bounds access of a cell was attempted

            // Check if a living cell will die
            if ((neighbors.sum_neighbor_states < 2) || (neighbors.sum_neighbor_states > 3)) {
                all_cells[i].new_state = 0;
            }

            // For each of the 8 neighbors above, find its 8 neighbors. This is to check if any dead cell must come to life.
            for (int j = 0; j < NUM_NEIGHBORS; j++) {  
                cell neighbor = neighbors.neighbor_cells[j];
                // 'nneighbors' are the second-order neighbors. 'neighbors' (and 'neighbor') are first-order.
                neighbor_info nneighbors;
                nneighbors = get_neighbor_info(all_cells[neighbor.label], all_cells, num_rows, num_columns, num_cells);

                if (nneighbors.out_of_bounds_access == 1) return 1; // Return immediately if an out-of-bounds access of a cell was attempted

                if (nneighbors.sum_neighbor_states == 3) {
                    all_cells[neighbor.label].new_state = 1;
                }

                free(nneighbors.neighbor_cells); // forgetting this leaks more memory, since it's 8 times (because of the loop) the size of the allocated memory from malloc (sizeof(cell))
            }
            free(neighbors.neighbor_cells); // forgetting this also leaks memory, but just 1 unit of sizeof(cell).
        }
    }

    // Refresh to update all cell states parallely
    for (int i = 0; i < num_cells; i++) {
        all_cells[i].state = all_cells[i].new_state;
    }

    // Return success (0). If there was an out-of-bounds access of a cell somewhere, 1 was returned before
    return 0;
}




int main(void) {

    // Initialize screen width and height
    const int screenWidth = 1000;       //default 1000, max 1400
    const int screenHeight = 600;       //default 600, max 750
    //ToggleFullscreen();

    const int cell_length = 20;                 // 10 is also a good choice to get more cells
    const int dist_bw_cells = cell_length/2;    // Distance between adjacent cells
    const int horizontal_padding = 20;          // Horizontal space from the edge of the window
    const int vertical_padding = 20;            // Vertical space from the edge of the window

    bool pause = 0;
    bool start = 0;
    bool reset = 0;
    bool out_of_bounds_access = 0; // Initialized to 0. See the functions get_neighbor_info() and update_cells() for more info.
    int frames_counter = 0;

    // Target FPS. This can be changed later in the game using left or right arrow buttons. See below.
    SetTargetFPS(40);

    // Initialize window and OpenGL context
    InitWindow(screenWidth, screenHeight, "The Game of Life");

    // Calculate the number of rows, columns, and cells from some of the variables initialized above
    const int num_columns = (2*(screenWidth - 2*horizontal_padding) + cell_length)/(3*cell_length);
    const int num_rows = (2*(screenHeight - 2*vertical_padding) + cell_length)/(3*cell_length);
    const int num_cells = num_columns*num_rows;

    // Create an array of cells and assign them their dimensions and states
    cell cells[num_cells];

    for (int i = 0; i < num_cells; i++) {
        cells[i].label = i;

        cells[i].rect_dim = (Rectangle){horizontal_padding + cell_length *(i%num_columns) + dist_bw_cells *(i%num_columns),
                                        vertical_padding + cell_length *(i/num_columns) + dist_bw_cells *(i/num_columns),
                                        cell_length,
                                        cell_length};
        
        cells[i].state = 0;
        cells[i].new_state = 0;
    }


    Vector2 mousePoint = {0.0f, 0.0f};



    // Main animation loop. One frame per loop
    // Detect window close button or ESC key
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) pause = !pause;
        if (IsKeyPressed(KEY_R)) reset = 1;
        if (IsKeyPressed(KEY_ENTER)) start = 1;

        if (IsKeyPressed(KEY_LEFT)) SetTargetFPS(GetFPS() - 5);
        else if (IsKeyPressed(KEY_RIGHT)) SetTargetFPS(GetFPS() + 5);

        if (!pause) {

            // Click on a cell to select (to make it alive). A selected cell becomes RED. Click again to deselect.
            // All cells are deselected at first (they are all dead) and beige in color.
            if (!start) {
                mousePoint = GetMousePosition();
                for (int i = 0; i < num_cells; i++) {
                    if (CheckCollisionPointRec(mousePoint, cells[i].rect_dim) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        cells[i].state = !cells[i].state;
                        cells[i].new_state = !cells[i].new_state;
                    }
                }
            }

            else {
                // The Game of Life starts here

                // Update cell states
                out_of_bounds_access = update_cells(cells, num_cells, num_rows, num_columns);
                if (out_of_bounds_access == 1) {
                    printf("Out-of-bounds access of a cell! Choose a different initial configuration or reduce the cell length to get more cells in the window.\n");
                    reset = 1;
                }
            }
        }
        else frames_counter++;


        BeginDrawing();

        ClearBackground(RAYWHITE);

        // If reset == 1, put all cells to dead (beige color) and restart the game. User can now select cells afresh.
        if (reset) {
            for (int i = 0; i < num_cells; i++) {
                cells[i].state = 0;
                cells[i].new_state = 0;
            }
            printf("Game reset!\n");
            reset = 0;
            start = 0;
        }

        // If the game is running, draw only the living (RED) cells.
        else if (start) {
            // Draw all living (RED) cells
            for (int i = 0; i < num_cells; i++) {
                if (cells[i].state == 1) DrawRectangleRec(cells[i].rect_dim, RED);
            }
        }
        
        // The window and cell configuration at game start (or restart). All cells are dead, and beige in color.
        else {
            for (int i = 0; i < num_cells; i++) {
                DrawRectangleRec(cells[i].rect_dim, cells[i].state? RED : (Color){ 0xD1, 0xB3, 0x8D, 0xFF }); // Shade of BEIGE
            }
        }

        // Draw text to show how to quit, reset, pause, or decrease/increase FPS
        DrawText("Quit: Esc | Reset: R | Pause: Space | FPS: Left/Right", 10, GetScreenHeight()-25, 20, LIGHTGRAY);

        // On pause, we draw a blinking message
        if (pause && ((frames_counter/30)%2)) DrawText("Paused", GetScreenWidth()/2, GetScreenHeight()/2, 30, GRAY);

        // Show FPS
        DrawFPS(10, 10);
    
        EndDrawing();
        // End of animation loop
        // Go back to beginning of loop, and repeat for next frame
    }
    // Close window and unload OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
}