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

        self.canvas_height = 300

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
        # Load YOLO object detection model
        net = cv2.dnn.readNet("yolov3.weights", "yolov3.cfg")

# Define the list of object classes that the YOLO model is trained to detect
        classes = []
        with open("coco.names", "r") as f:
            classes = [line.strip() for line in f.readlines()]

# Set the confidence threshold for object detection
        conf_threshold = 0.5

# Set the non-maximum suppression threshold for object detection
        nms_threshold = 0.4

# Convert the frame to a blob for input to the YOLO model
        blob = cv2.dnn.blobFromImage(frame, 1/255.0, (416, 416), swapRB=True, crop=False)

# Set the input to the YOLO model
        net.setInput(blob)

# Run forward pass through the YOLO model to detect objects
        output_layers = net.getUnconnectedOutLayersNames()
        layer_outputs = net.forward(output_layers)

# Process the outputs of the YOLO model to extract person detections
        person_detections = []
        for output in layer_outputs:
            for detection in output:
                scores = detection[5:]
                class_id = np.argmax(scores)
                confidence = scores[class_id]
                if class_id == 0 and confidence > conf_threshold:
                    box = detection[0:4] * np.array([frame.shape[1], frame.shape[0], frame.shape[1], frame.shape[0]])
                    (centerX, centerY, width, height) = box.astype("int")
                    x = int(centerX - (width / 2))
                    y = int(centerY - (height / 2))
                    person_detections.append([x, y, int(width), int(height)])

# Apply non-maximum suppression to remove duplicate detections
        indices = cv2.dnn.NMSBoxes(person_detections, [1.0]*len(person_detections), conf_threshold, nms_threshold)
        num_people = len(indices)

# Print the number of people detected
        print("Number of people detected: ", num_people)
        


    def update_GUI(self):

        if self.rounded_array is None:

            return

        frame = self.rounded_array.astype(np.uint8)


        frame = cv2.applyColorMap(frame, self.colormap)



        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)




        frame = cv2.resize(frame, (self.canvas_width, self.canvas_height), interpolation=cv2.INTER_NEAREST)




        self.photo = ImageTk.PhotoImage(Image.fromarray(frame))

        self.canvas2.itemconfig(self.img, image=self.photo)

        self.window.update()

if __name__ == "__main__":

    server = Evo_Thermal_Server("TTTEST")

    while True:

        server.run()