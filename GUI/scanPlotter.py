import tkinter as tk
from tkinter import filedialog
import numpy as np
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

ANGLE_SIZE = 2

distances = []
line = None
canvas = None


def scanGraphInit(parent):
    global line, canvas

    fig = Figure(figsize=(3.5, 2.7))
    ax = fig.add_subplot(projection="polar")
    line, = ax.plot([], [], color="r", linewidth=3)
    ax.set_rmax(2.5)
    ax.set_thetamax(180)
    ax.set_xticks(np.deg2rad([0, 45, 90, 135, 180]))

    canvas = FigureCanvasTkAgg(fig, master=parent)
    canvas.draw()
    return canvas.get_tk_widget()


#Clears plot and prepares for next scan
def start_scan():
    distances.clear()
    line.set_data([], [])
    canvas.draw_idle()

#plots individual points
def add_point(distance):
    distances.append(distance)
    angles = np.deg2rad(np.arange(len(distances)) * ANGLE_SIZE)
    line.set_data(angles, distances)
    canvas.draw_idle()

#alternative choice plot whole data 
def plot_scan(every_distance):
    start_scan()
    distances.extend(every_distance)
    angles = np.deg2rad(np.arange(len(distances)) * ANGLE_SIZE)
    line.set_data(angles, distances)
    canvas.draw_idle()