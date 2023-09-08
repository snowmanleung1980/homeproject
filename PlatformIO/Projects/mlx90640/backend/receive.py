from paho.mqtt import client as mqtt_client

import random

import numpy as np

import cv2

from PIL import Image, ImageTk

import tkinter as Tk
import datetime
   
import json



class Evo_Thermal_Server():

    def __init__(self):

                ### MQTT

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

   

  

        #dtype = float64, shape = 32x32

        # 假设已知原始frame的shape和dtype
        #print(message.payload)
    

# Do something with the first item
    def update_GUI(self):
        with open('temperatures.json') as f:
            data = json.load(f)

# Get the first item from the list
        for d in range(768):
            tempertureArray= data[d]
        #min_temperture = min(tempertureArray)
       #max_temperture = max(tempertureArray)
            min_temperture = 25
            max_temperture = 33
            multiplier = 255/(max_temperture-min_temperture)
            for i in range(768):
                if tempertureArray[i] < 25:
                    tempertureArray[i] = min_temperture
                    tempertureArray[i] = (tempertureArray[i]-min_temperture)*multiplier
            frame = np.array(tempertureArray) # type: ignore
            frame=frame.reshape(24,32)
            self.rounded_array = np.round(frame)
    

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
            cv2.waitKey(500)

if __name__ == "__main__":

    server = Evo_Thermal_Server()

    while True:

        server.run()