"""
Conway's Game of Life

There are 2 example configurations in this script: A still-life called 'Block', and an oscillator called 'Blinker'

Author: Shubham Maheshwari
email: shubham.93@gmail.com
GitHub: https://github.com/shubham-93
"""

import matplotlib.pyplot as plt
import numpy as np
from math import sqrt
from matplotlib.animation import FuncAnimation


class Cell:
    def __init__(self, label, status, coords):        
        self.label = label              # Unique identifier number from 0 to total number of cells
        self.status = status            # status = True/False (Alive/Dead)
        self.coords = coords            # coords = list of coordinates [x,y]
        self.new_status = self.status   # To make sure that new_status is initialized to be the same as status. new_status may change every a neighbor_checking process below

        # Indices of the 8 neighbors around a cell
        self.neighbors_indices = [
            self.label - int(sqrt(num_cells)),
            self.label - int(sqrt(num_cells)) - 1,
            self.label - int(sqrt(num_cells)) + 1,
            self.label - 1,
            self.label + 1,
            self.label + int(sqrt(num_cells)),
            self.label + int(sqrt(num_cells)) - 1,
            self.label + int(sqrt(num_cells)) + 1
        ]

    # Return the sum of statuses of 8 neighbors around a cell
    def sum_neighboring_statuses(self):
        return sum([cells[neighbor_index].status for neighbor_index in self.neighbors_indices])
    
    # Put status = new_status. Used when all neighbor-checking processes have completed in a frame
    def refresh_status(self):
        self.status = self.new_status

    # Check if an alive cell should die, or if a dead cell should come alive.
    # Note that 'cells' is a list defined in the main body of the program, which contains instances of the Cell class    
    def neighbor_checker(self):
        if self.status==True:
            if self.sum_neighboring_statuses()<2 or self.sum_neighboring_statuses()>3:
                self.new_status=False
            # status change must happen parallely for all cells, not sequentially, so we created a new attribute new_status above.
            # In the main body of the program, after all cells have had their statuses updated inside a list comprehension,
            # we refresh the statuses by using self.refresh_status method
            # Now, cell[index].status represents the true status of each cell (after a parallel change of all cells' statuses)
            # And this is what we can use to plot the next frame in the animation.
            for neighbor_index in self.neighbors_indices:
                if cells[neighbor_index].sum_neighboring_statuses()==3:
                    cells[neighbor_index].new_status=True


# How to plot a mesh of cells
num_rows = 50                                           # Number of rows of cells
num_cells = num_rows**2                                 # Number of cells is just the square of the number of rows
coords = np.meshgrid(range(num_rows), range(num_rows))  # Create a mesh

# Get the list of coordinates [x,y] for each cell
cell_coords = np.array(coords).T.reshape(-1,2)

# Create instances of the Cell class. Initially, all of them are False/Dead
cells = [Cell(label=label, status=False, coords=cell_coords[label]) for label in range(len(cell_coords))]


# We now define 2 simple patterns often seen in the game of life. This is done by finding the cell labels whose initial statuses must be set to true.

# Blinker, a simple oscillator (https://conwaylife.com/wiki/Blinker)
initial_true_labels = [(num_rows/2*num_rows) + num_rows/2,
                       (num_rows/2*num_rows) + num_rows/2 + 1,
                       (num_rows/2*num_rows) + num_rows/2 + 2
                       ]


# Block, a still-life (https://conwaylife.com/wiki/Block)
# initial_true_labels = [(num_rows/2*num_rows) + num_rows/2,
#                        (num_rows/2*num_rows) + num_rows/2 + 1,
#                        ((num_rows/2+1)*num_rows) + num_rows/2,
#                        ((num_rows/2+1)*num_rows) + num_rows/2 + 1,
#                        (10*50)+15]

initial_true_labels = map(int,initial_true_labels)


# Set the status of each cell chosen above to true/alive
for true_label in initial_true_labels:
    cells[true_label].status = True
    cells[true_label].new_status = True


# Create the figure
fig, ax = plt.subplots()
xdata, ydata = [], []
#ax.grid()
ln, = ax.plot([], [], marker='s', markersize=20)
ln.set_markerfacecolor('r')
ln.set_markeredgecolor('k')
ln.set_linestyle('')

# Figure initialization function
def init_fig():
    ax.set_xlim(20, 30)
    ax.set_ylim(20, 30)    
    return ln,


# Evolve the system of particles in time
def evolution(frame):
    # Create new statuses based on neighbor statuses
    [cells[label].neighbor_checker() for label in range(num_cells)]
    # Refresh statuses
    [cells[label].refresh_status() for label in range(num_cells)]

    # Get the coordinates of cells which are alive after checking neighbors and refreshing
    true_cells_coords = [cell.coords for cell in cells if cell.status==True]

    # Unpack the x and y coordinates in order to plot
    if true_cells_coords:
        true_cells_x, true_cells_y = np.array(true_cells_coords).T
    else:
        print(f'---Frame number {frame} ends here, and animation stops. No true cells remaining---')
        anim.event_source.stop()
        return
    
    # false_cells_coords = [cell.coords for cell in cells if cell.status==False]
    # if false_cells_coords:
    #     false_cells_x, false_cells_y = np.array(false_cells_coords).T
    

    # Plot the living cells
    ln.set_data(true_cells_x, true_cells_y)
    return ln,


# Animation
anim = FuncAnimation(fig, evolution, init_func=init_fig, blit=True, interval=200, frames=20, repeat=False, cache_frame_data=False)
#anim.save(filename='./game_of_life.gif')
plt.show()