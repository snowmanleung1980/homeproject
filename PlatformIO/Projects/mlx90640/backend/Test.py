from paho.mqtt import client as mqtt_client

import random

import numpy as np

import cv2

from PIL import Image, ImageTk

import tkinter as Tk
import datetime
   




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
        self.x=0
       
       

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
        #print(message.payload)
        print(len(message.payload))
        assert len(message.payload) == 1536
        tempertureArray:list[float] = []
        for i in range(1536):
            if (i+1) % 2 == 0 and i != 0: 
                tempertureArray.append((message.payload[i-1]+message.payload[i]*256)/100)

            
            
        
        #min_temperture = min(tempertureArray)
       #max_temperture = max(tempertureArray)
        min_temperture = 21

        max_temperture = 39

        multiplier = 255/(max_temperture-min_temperture)

        for i in range(768):

            #if tempertureArray[i] <27 and tempertureArray[i] >23:

            #    tempertureArray[i] =25

            if tempertureArray[i] < min_temperture:

               tempertureArray[i] = min_temperture + 1


            if tempertureArray[i] >=31 and tempertureArray[i] < 32:

                tempertureArray[i] =35

            if tempertureArray[i] >=28 and tempertureArray[i] <31:

                tempertureArray[i] =32

            if tempertureArray[i] >=27 and tempertureArray[i] <28:

                tempertureArray[i] =31



            if tempertureArray[i] > max_temperture:

                tempertureArray[i] = max_temperture




            tempertureArray[i] = (tempertureArray[i]-min_temperture)*multiplier

        frame = np.array(tempertureArray) # type: ignore

        frame=frame.reshape(24,32)

        self.rounded_array = np.round(frame)

    def update_GUI(self):

        if self.rounded_array is None:

            return

        frame = self.rounded_array.astype(np.uint8)
        
        frame = cv2.applyColorMap(frame, self.colormap)



        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        #frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)
        #frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)




        frame = cv2.resize(frame, (self.canvas_width, self.canvas_height), interpolation=cv2.INTER_NEAREST)

       

        self.photo = ImageTk.PhotoImage(Image.fromarray(frame))


        now = datetime.datetime.now()

# Create a formatted string with the date and time
        #date_time_str = now.strftime("%Y-%m-%d_%H-%M-%S")
        #filename = "P:\\IT\\Intern\\2023\\Isaac\\data\\{}_image{}.jpg".format(date_time_str, self.x)  # Change the filename and extension as per your requirement
        #Image.fromarray(frame).save(filename)

        self.canvas2.itemconfig(self.img, image=self.photo)
        self.window.update()
        self.x=self.x+1
        #cv2.waitKey(1500)

if __name__ == "__main__":

    server = Evo_Thermal_Server("TTTTEST")

    while True:

        server.run()