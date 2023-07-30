import matplotlib.pyplot as plt
import numpy as np
from matplotlib import colors

with open("output.txt") as f:
    lines = f.readlines()

xs, ys = [], []
newcolors = []
for line in lines:
    if line.strip() == "":
        # Empty line indicates a new polygon
        plt.fill(xs, ys, edgecolor='black', linewidth=0.5, facecolor=newcolors.pop(0))
        xs, ys = [], []
    else:
        # Check that line has at least two comma-separated values
        if ',' not in line:
            continue
        x, y = map(float, line.split(','))
        xs.append(x)
        ys.append(y)
    
    if len(xs) > 0 and len(newcolors) == 0:
        # Create a new color map when a new polygon is started
        num_colors = 20
        color_list = np.random.choice(range(256), size=(num_colors, 3))
        newcolors = [tuple(x/256.0 for x in color) for color in color_list]

# Fill the last polygon
plt.fill(xs, ys, edgecolor='black', linewidth=0.5, facecolor=newcolors.pop(0))

# Add grid and set style to dotted
plt.grid(linestyle=':')
plt.gca().set_aspect('equal', adjustable='box')

# Save the image as a tight figure without padding
plt.savefig('output.png', dpi=1200, bbox_inches='tight', pad_inches=0.0)

