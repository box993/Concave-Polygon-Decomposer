import os
import matplotlib.pyplot as plt



times = []


    #/*************************************************************************************************/
    #/*                                      n currently set to 4 to 250                              */
    #/*************************************************************************************************/

for n in range(4, 251):
    for i in range(1, 11):
        filename = f"decomposed/{n}/{n}_{i}.txt"
        if os.path.exists(filename):
            with open(filename, "r") as f:
                lines = f.readlines()
                time = float(lines[-1])
                times.append((n, time))

plt.plot([n for n, t in times], [t for n, t in times], linewidth=0.5)
plt.xlabel("Number of Vertices (n)")
plt.ylabel("Time (s)")
plt.grid(linestyle=':')
plt.savefig("Time_vs_n.png", dpi=1200, bbox_inches='tight', pad_inches=0.1)
