import tkinter as tk

MAP_START = 400

canvas = None

def mapInit(parent):
    global canvas
    container = tk.Frame(parent, width = MAP_START, height = MAP_START)
    canvas = tk.Canvas(container, width = MAP_START, height = MAP_START, bg = "white",
                       highlightthickness = 1, highlightbackground = "gray")
    canvas.place(relx = 0.5, rely = 0.5, anchor = "center",
                 width = MAP_START, height = MAP_START)

    def keepMapSquare(event):
        side = min(event.width, event.height)
        canvas.place_configure(width = side, height = side)

    container.bind("<Configure>", keepMapSquare)
    return container


#Short object radius 6.5
#cliff dots trace between closest ones