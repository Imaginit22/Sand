import numpy as np
import matplotlib.pyplot as plt
import random
import sys
import os
sys.setrecursionlimit(100000)
# Grid size (small for visualization)
size = 500
grid = np.zeros((size, size), dtype=int)

# Drop grains in the center
center = size // 2


grid[center, center] = 3 # try different numbers here!

def slope(i, j, grid = grid):
    if i < 0 or i >= size or j < 0 or j >= size:
        return 0
    if grid[i, j] == 3:
        toRet = 0
        grid[i, j] = 0
        toRet += slope(i+1, j)
        toRet += slope(i-1, j)
        toRet += slope(i, j+1)
        toRet += slope(i, j-1)
        return toRet + 1
    else:
        grid[i, j] = grid[i, j] + 1
        return 0
        
# plt.imshow(grid, cmap='viridis', interpolation='nearest')
# plt.colorbar(label="Grains", ticks=range(0,4))
# plt.title("Abelian Sandpile")
# plt.axis('off')
# plt.show(block=False)
topplecounter = []
inputted = input("Should this run automatically? (y/n): ")
if inputted == "n":
    inputted = True
else:
    inputted = False
for i in range(100000):
    inpu = ""
    if inputted:# or i % 1000 == 0:
        inpu = input("Press Enter to drop a grain, or specify a location x y: ")
        plt.imshow(grid, cmap='viridis', interpolation='nearest')
        #plt.colorbar(label="Grains", ticks=range(0,4))
        plt.title("Abelian Sandpile")
        plt.axis('off')
        plt.show(block=False)
        # Show the result
    
    if len(inpu) == 0:
        x = random.randint(0, size - 1)
        y = random.randint(0, size - 1)
    else:
        x,y = inpu.split(" ")
        x = int(x)
        y = int(y)
    topples = slope(x, y)
    topplecounter.append(topples)
    if i%1000 == 0:
        print("worked on ", i)
        print("topples was", topples)
print("Doneiterating")
plt.imshow(grid, cmap='viridis', interpolation='nearest')
plt.savefig("plot.png")
file = open("topplecounter.txt", "w")
file.write(str(topplecounter))
print("done")

