import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

file_path = 'output.txt'  # path to the constantly updating file


# Load initial data
initial_data = np.loadtxt(file_path)
fig, ax = plt.subplots()
im = ax.imshow(initial_data, cmap='viridis', interpolation='nearest', vmin=0, vmax=3)
plt.colorbar(im, ax=ax, label="Grains", ticks=range(0, 4))
ax.set_title("Abelian Sandpile")
ax.axis('off')

def update(frame):
    try:
        new_data = np.loadtxt(file_path)
        if new_data.shape == im.get_array().shape:
            im.set_data(new_data)
        else:
            print("Shape changed, skipping frame.")
    except Exception as e:
        print(f"Error reading file: {e}")

    return [im]

ani = FuncAnimation(fig, update, interval=100, blit=False)
plt.show()
