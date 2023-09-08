from paho.mqtt import client as mqtt_client

import random

import numpy as np

import cv2

from PIL import Image, ImageTk

import tkinter as Tk

   




class Evo_Thermal_Server():

    def __init__(self,topic):

                ### MQTT

        self.broker = '192.168.2.35'

        self.broker_port = 1883

        self.topic = topic

        self.mqtt_username = "admin"

        self.mqtt_password = "Castco@ctc2021"

        self.client_id = f'evothermal-mqtt-{random.randint(0, 1000)}'

        self.client = self.connect_mqtt()

        self.client.on_message = self.on_message

        self.client.loop_start()

        self.client.subscribe(topic)

                ### Visualization window ###

        self.activate_visualization = True

        self.window = Tk.Tk()

        self.window.wm_geometry("640x720")

        self.canvas_width = 400

        self.canvas_height = 400

        self.canvas2 = Tk.Canvas(self.window, width=self.canvas_width, height=self.canvas_height)

        self.canvas2.pack(side=Tk.TOP)

        self.photo = ImageTk.PhotoImage("P")

        self.img = self.canvas2.create_image(200, 200, image=self.photo)

        self.text2 = Tk.Label(self.window)

        self.text2.config(height=10, width=20, text='', font=("Helvetica", 25))

        self.text2.pack(side=Tk.BOTTOM)

        self.text2.config(text="Evo Thermal")




        self.MinAvg = []

        self.MaxAvg = []

       

        r = []

        g = []

        b = []

        self.rounded_array = None

        with open('./colormap.txt', 'r') as f:

            for i in range(256):

                x,y,z = f.readline().split(',')

                r.append(x)

                g.append(y)

                b.append(z.replace(";\n", ""))

        self.colormap = np.zeros((256, 1, 3), dtype=np.uint8)

        self.colormap[:, 0, 0] = b

        self.colormap[:, 0, 1] = g

        self.colormap[:, 0, 2] = r

    def run(self):

        self.update_GUI()

    def connect_mqtt(self):

        def on_connect(client, userdata, flags, rc, _):

            if rc == 0:

                print("Connected to MQTT Broker!")

            else:

                print("Failed to connect, return code %d\n", rc)

        # Set Connecting Client ID

        client = mqtt_client.Client(client_id=self.client_id,protocol=mqtt_client.MQTTv5)

        client.username_pw_set(self.mqtt_username, self.mqtt_password)

        client.on_connect = on_connect

        print(f"connecting -- username:{self.mqtt_username} password:{self.mqtt_password}")

        client.connect(self.broker, self.broker_port)

        return client

    def on_message(self,client,userdata,message):

        #dtype = float64, shape = 32x32

        # 假设已知原始frame的shape和dtype
        # print(message.payload)
        if len(message.payload)==769:
            message.payload = message.payload[:-1]


        frame = np.array(np.frombuffer(message.payload, dtype=np.byte))
        frame=frame.reshape(24,32)
        self.rounded_array = frame
    def update_GUI(self):

        if self.rounded_array is None:

            return

        frame = self.rounded_array.astype(np.uint8)


        frame = cv2.applyColorMap(frame, self.colormap)



        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)




        frame = cv2.resize(frame, (self.canvas_width, self.canvas_height), interpolation=cv2.INTER_NEAREST)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Apply threshold
        _, thresh = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY)

    # Perform morphological operation
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        thresh = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)

    # Find contours
        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Count objects
        count = 0
        for contour in contours:
            area = cv2.contourArea(contour)
            if area > 1000:  # Filter out small contours
              count += 1

        print(count)


        self.photo = ImageTk.PhotoImage(Image.fromarray(frame))

        self.canvas2.itemconfig(self.img, image=self.photo)

        self.window.update()

if __name__ == "__main__":

    server = Evo_Thermal_Server("TTTEST")

    while True:

        server.run()