import tkinter as tk
from PIL import Image

Image.CUBIC = Image.BICUBIC
import ttkbootstrap as tb
import socket
import threading
import time

message = ""


HOST = "192.168.45.200"
PORT = 7070
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


def connect(app):
    try:
        s.connect((HOST, PORT))
        print("Conexión establecida. ")
        handle_messages(app)
    except KeyboardInterrupt as ex:
        print("Keyboard Interrupt.")
    except Exception as ex:
        print(ex)
    finally:
        app.close()
        s.close()
        print("Desconectado.")


def handle_messages(app):
    global message
    while True:
        message = s.recv(1024).decode()
        args = message.split(",")
        app.update(value=float(args[2]), mode=args[0], scale=args[1])


class App:
    def __init__(self, master):
        self.master = master
        self.master.title("Voltimetro Digital")
        self.master.geometry("500x550")

        self.volt_meter = tb.Meter(
            root,
            bootstyle="info",
            textleft="Tensión: ",
            textright="V",
            interactive=True,
            metertype="semi",
            metersize=250,
            subtextstyle="light",
        )
        self.volt_meter.pack(pady=50)

    def update(self, value=5.0, mode="DC", scale="10"):
        self.volt_meter.configure(amountused=value)
        self.volt_meter.configure(subtext=mode + " [0V - " + scale + "V]")

        if scale == "10":
            self.volt_meter.configure(amounttotal=10)
        else:
            self.volt_meter.configure(amounttotal=100)

    def close(self):
        self.master.quit()


if __name__ == "__main__":
    root = tb.Window("voltimetro", themename="darkly")
    app = App(root)

    client_thread = threading.Thread(target=connect, args=[app])
    client_thread.start()

    app.master.mainloop()

    client_thread.join()
