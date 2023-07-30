#python3 polygenerator.py
#pip install polygenerator
#pip install shapely

from polygenerator import random_polygon
from shapely.geometry import Polygon
import os
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon as pltPolygon


    #/*************************************************************************************************/
    #/*                                      n currently set to 8 to 31                               */
    #/*************************************************************************************************/
# define range of num_points for polygons
num_points_list = list(range(8, 31))

# create directories for polygons and images
os.makedirs("polygons_input", exist_ok=True)
os.makedirs("images_input", exist_ok=True)

# create directory for each num_points
for num_points in num_points_list:
    dir_name = f"polygons_input/{num_points}"
    os.makedirs(dir_name, exist_ok=True)
    os.makedirs(f"images_input/{num_points}", exist_ok=True)

    # generate 10 valid polygons for current num_points and save to directory
    for i in range(1, 11):
        while True:
            polygon = random_polygon(num_points=num_points)
            poly = Polygon(polygon)
            if poly.is_valid:
                break

        file_name = f"{dir_name}/{num_points}_{i}.txt"
        with open(file_name, "w") as f:
            f.write(f"{num_points}\n")
            for j, coord in enumerate(polygon):
                f.write(f"{coord[0]} {coord[1]}\n")

                # plot the polygon and label coordinates 
                                    # !!!!!!!!!!!!!!!!!!!!UNCOMMENT IT IF YOU WANT TO SAVE IMAGES!!!!!!!!!!!!!!!!!!!
                #if j == 0:
                    #fig, ax = plt.subplots()
                    #ax.set_aspect("equal")
                    #ax.grid(linestyle=':')
                #ax.add_patch(pltPolygon(polygon, closed=True, fill=False, edgecolor="red", lw=0.5))
                #ax.annotate(j + 1, xy=coord, fontsize=8)


        # save the image
        #fig.savefig(f"images_input/{num_points}/{num_points}_{i}.png", dpi=600, bbox_inches="tight", pad_inches=0)

        #plt.close(fig)

    print(f"Polygon with {num_points} saved")
