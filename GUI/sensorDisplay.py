import tkinter as tk

SIZE = 240
IDLE = "#cccccc"
ACTIVE = "red"

canvas = None
sensors = {}

def sensorInit(parent):
    global canvas
    canvas = tk.Canvas(parent, width = SIZE, height = SIZE, bg = "white",
                       highlightthickness = 1, highlightbackground = "gray")

    canvas.create_oval(40, 40, 200, 200, fill = "#eeeeee", outline = "black")

    layout = {
        "cliffFL" : (90, 55),
        "cliffFR" : (150, 55)
    }
    radius = 14
    for name, (x, y) in layout.items():
        sensors[name] = canvas.create_oval(x - radius, y - radius, x + radius, y + radius,
                                        fill = IDLE, outline = "black")
        canvas.create_text(x, y + radius + 8, text = name, font = ("Helvetica", 7))

    return canvas

def setSensor(name, active):
    canvas.itemconfig(sensors[name], fill = ACTIVE if active else IDLE)

def clearSensors():
    for name in sensors:
        canvas.itemconfig(sensors[name], fill = IDLE)