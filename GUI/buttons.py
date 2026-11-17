import tkinter as tk
import console

BTN_SIZE = 60
REPEAT_MS = 100
COOLDOWN_MS = 800
RELEASE_DELAY_MS = 60

held = {}
locked = set()
pendingRelease = {}

#Flicker Fix
def keyPress(root, key):
    instruction = pendingRelease.pop(key, None)
    if instruction is not None:
        root.after_cancel(instruction)
        return
    press(root, key)

def keyRelease(root, key):
    if key in pendingRelease:
        return
    pendingRelease[key] = root.after(RELEASE_DELAY_MS, lambda: finishRelease(root, key))

def finishRelease(root, key):
    pendingRelease.pop(key, None)
    release(root, key)

#-----Hold----
def sendInstruction(key):
    console.logSent(key)

def sendStop():
    console.logSent("STOP")

def press(widget, key):
    if key in held:
        return

    def tick():
        sendInstruction(key)
        held[key] = widget.after(REPEAT_MS, tick)

    held[key] = None
    tick()

def release(widget, key):
    if key not in held:
        return
    instruction = held.pop(key)
    if instruction is not None:
        widget.after_cancel(instruction)
    if not held:
        sendStop()

#----One press---
def on_scan():
    console.logSent("Scanning")

def on_Q():
    console.logSent("Q")

ACTIONS = {"Q": on_Q, "E": on_scan}

def fireOnce(widget, key, action):
    if key in locked:
        return
    locked.add(key)
    widget.after(COOLDOWN_MS, lambda: locked.discard(key))
    action()

#-----UI Init-----
def buttonInit(parent):
    pad = tk.Frame(parent)
    pad.pixel = tk.PhotoImage(width = 1, height = 1)

    def makeButton(text, row, column, key = None):
        btn = tk.Button(pad, text = text, image = pad.pixel, compound = "center",
                        width = BTN_SIZE, height = BTN_SIZE)
        btn.grid(row = row, column = column, padx = 3, pady = 3)
        if key in ACTIONS:
            btn.bind("<ButtonPress-1>", lambda e: fireOnce(btn, key, ACTIONS[key]))
        else:
            btn.bind("<ButtonPress-1>", lambda e: press(btn, key))
            btn.bind("<ButtonRelease-1>", lambda e: release(btn, key))
        return btn

    makeButton("Confirm", 0, 0, "Q")
    makeButton("W", 0, 1, "W")
    makeButton("A", 1, 0, "A")
    makeButton("S", 1, 1, "S")
    makeButton("D", 1, 2, "D")
    makeButton("Scan", 0, 2, "E")

    return pad

def keyInit(root):
    for k in "WASD":
        for c in (k, k.lower()):
            root.bind(f"<KeyPress-{c}>", lambda e, k=k: keyPress(root, k))
            root.bind(f"<KeyRelease-{c}>", lambda e, k=k: keyRelease(root, k))
    for k, action in ACTIONS.items():
        for c in (k, k.lower()):
            root.bind(f"<KeyPress-{c}>", lambda e, k=k, a=action: fireOnce(root, k, a))