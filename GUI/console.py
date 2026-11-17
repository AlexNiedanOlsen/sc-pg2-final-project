import tkinter as tk

MAX_LINES = 500

console = None

def consoleInit(parent):
    global console
    frame = tk.Frame(parent)
    frame.columnconfigure(0, weight = 1)
    frame.rowconfigure(0, weight = 1)

    console = tk.Text(frame, width = 40, height = 12, state = "disabled", wrap = "word",
                      bg = "black", fg = "white", font = ("Menlo", 11))
    scroll = tk.Scrollbar(frame, command = console.yview)
    console.config(yscrollcommand = scroll.set)
    console.grid(row = 0, column = 0, sticky = "nsew")
    scroll.grid(row = 0, column = 1, sticky = "ns")

    console.tag_config("sent", foreground = "#6cb6ff")
    console.tag_config("received", foreground = "#7ee787")
    console.tag_config("info", foreground = "#aaaaaa")
    return frame

def _write(prefix, text, tag):
    atBottom = console.yview()[1] >= 0.999 #Scroll only when at bottom
    console.config(state = "normal")
    console.insert("end", f"{prefix} {text}\n", tag)
    lines = int(console.index("end-1c").split(".")[0])
    if lines > MAX_LINES:
       console.delete("1.0", f"{lines - MAX_LINES}.0")
    console.config(state = "disabled")
    if atBottom:
        console.see("end")

def logSent(text):
    _write(">>", text, "sent")

def logRecieved(text):
    _write("<<", text, "info")

def logInfo(text):
    _write("--", text, "info")