import tkinter as tk
from PIL import Image

Image.CUBIC = Image.BICUBIC
import ttkbootstrap as tb
import socket
import time


class App:
    def __init__(self, master):
        self.master = master
        self.master.title("Voltimetro Digital")
        self.master.geometry("500x550")

        self.HOST = "192.168.45.200"
        self.PORT = 7070
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.volt_meter = tb.Meter(
            self.master,
            bootstyle="info",
            textleft="Tensión: ",
            textright="V",
            interactive=True,
            metertype="semi",
            metersize=250,
            subtextstyle="light",
        )

        self.connect_button = tb.Button(
            self.master,
            text="Connect",
            bootstyle="default",
            command=lambda: self.connect(),
        )

        self.exit_button = tb.Button(
            self.master,
            text="Exit",
            bootstyle="default",
            command=lambda: self.close(),
        )

        self.volt_meter.pack(pady=40)
        self.connect_button.pack(pady=20)
        self.exit_button.pack(pady=5)

    def update(self, value=5.0, mode="DC", scale="10"):
        self.volt_meter.configure(amountused=value)
        self.volt_meter.configure(subtext=mode + " [0V - " + scale + "V]")

        if scale == "10":
            self.volt_meter.configure(amounttotal=10)
        else:
            self.volt_meter.configure(amounttotal=100)

    def connect(self):
        try:
            self.s.connect((self.HOST, self.PORT))
            self.connect_button.configure(state="disabled")
            self.handle_messages()
        except KeyboardInterrupt as ex:
            print("Keyboard Interrupt.")
        except Exception as ex:
            print(ex)
            self.close()

    def handle_messages(self):
        message = self.s.recv(1024).decode()
        args = message.split("\n")
        args = args[0].split(",")
        self.update(value=float(args[2]), mode=args[0], scale=args[1])
        self.master.after(10, self.handle_messages)

    def close(self):
        self.s.close()
        self.master.quit()


if __name__ == "__main__":
    root = tb.Window("voltimetro", themename="darkly")
    app = App(root)

    app.master.mainloop()
