import tkinter as tk
import scanPlotter
import numpy as np
import buttons
import mapView
import console
import sensorDisplay

root = tk.Tk()
root.title("CyBot")

#let graph resize with window
root.columnconfigure(0, weight = 0)
root.columnconfigure(1, weight = 1)
root.columnconfigure(2, weight = 0)
root.rowconfigure(0, weight = 1)
root.rowconfigure(1, weight = 1)

#Buttons
control_frame = tk.Frame(root)
control_frame.grid(row = 1, column = 0, sticky = "n", padx = 10, pady = 10)
buttons.buttonInit(control_frame).grid(row = 1, column = 0)

#Keys
buttons.keyInit(root)

#Map
map_frame = tk.Frame(root)
map_frame.grid(row = 0, column = 1, rowspan = 2, sticky = "nsew")
mapView.mapInit(map_frame).pack(fill = "both", expand = True)

#Graph
graph_frame = tk.Frame(root)
graph_frame.grid(row=0, column=2, sticky="nsew")
scanPlotter.scanGraphInit(graph_frame).grid(row=0, column=0, sticky="nsew")

#Console
console_frame = tk.Frame(root)
console_frame.grid(row = 0, column = 0, sticky = "nsew")
console_frame.columnconfigure(0, weight = 1)
console_frame.columnconfigure(0, weight = 1)
console.consoleInit(console_frame).grid(row = 0, column = 0, sticky = "nsew")

#Sensor Display
sensorDisplay.sensorInit(root).grid(row = 1, column = 2, padx = 10, pady = 10)

#scanPlotter.start_scan()

test_scan = np.array([2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,2,2,2,2,
                      2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                      2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                      2,2,2,2,2,2,2,2,2,2], dtype=float)
scanPlotter.plot_scan(test_scan)
# 
assert len(test_scan) == 90, f"Expected 90 values, got {len(test_scan)}"
sensorDisplay.setSensor("cliffFL", True)

root.mainloop()


#W .1 meters
#w .05 meters
#L 30 degrees
#l 15 degrees
# add standardized move bytes


#Add only one button at a time logic