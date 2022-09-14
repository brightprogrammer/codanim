# MazeSolver Explanation

This program creates a 2D array of sprites (representing a single cell) and another array of integer values storing
the states of each cell. There are four states for any cell :  
- **EMPTY** : Means the cell hasn't been visited yet, is neither a path cell and nor a blocking cell.
- **BLOCK** : Means you can't step over to this cell, find some other way around
- **PATH**  : This cell is in solution (path)
- **VISITED** : We reached this cell but there was a deadlock here, so we're never gonna step on it again.

Then there are 8 directons in which the solver can move from any cell:
- **LEFT**
- **RIGHT**
- **UP**
- **DOWN**
- **UP_LEFT**
- **DOWN_LEFT**
- **UP_RIGHT**
- **DOWN_RIGHT**

From each cell, we call a helper function to give us direction in which we can move in. If no such direction exists
then we return a deadlock and try to backtrack our path using stack. The stack stores the path. If we pop the top
element we get the direction in which we moved and from that we can go to previous cell to select another direction.
Now the cell which had a deadlock is marked as **CELL_VISITED**. Each cell which had a solution (direction) is marked
as **CELL_PATH**. **CELL_BLOCK** and **CELL_EMPTY** are marked automatically during maze generation phase.

This program generates maze randomly so sometimes it is possible that there is no solution for maze. In that case the program
backtracks the whole path and marks all empty cell as **CELL_VISITED**! This is some awesome visual!

For detailed idea of how this whole thing works, take a look at how **Depth First Search** works!
