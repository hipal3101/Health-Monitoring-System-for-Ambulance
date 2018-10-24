import sys
import RPi.GPIO as GPIO
#import Adafruit_DHT
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import ssl
from time import sleep
from datetime import date, datetime
import time
import json
import random

import paho.mqtt.client as mqtt

data=""





# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()
 
# AWS IoT certificate based connection
myMQTTClient = AWSIoTMQTTClient("123afhlss456")
myMQTTClient.configureEndpoint("a3dzfhkq0a3rhd.iot.us-west-2.amazonaws.com", 8883)
myMQTTClient.configureCredentials("/home/pi/cert/CA.crt", "/home/pi/cert/54f2f053f3-private.pem.key", "/home/pi/cert/54f2f053f3-certificate.pem.crt")
myMQTTClient.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2)  # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10)  # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5)  # 5 sec
 
#connect and publish
myMQTTClient.connect()
myMQTTClient.publish("thing01/info", "connected", 0)
 
 
 
def on_connect(client, userdata, flags, rc):
    #print ("Connected with rc: " + str(rc))
    clientconnected_flag=True
    client.subscribe("data/temp")
    
def on_message(client, userdata, msg):
    data=msg.payload
    
    data= data.decode('UTF-8')
    #j=json.loads(data)
    #temp=j["Temp"]
    #bpm=j["BPM"]
    #bp=random.randint(120,139)
    if True:
        #payload = '{ "temperature": ' + str(temp) + ',"bpm": '+ str(bpm) + ',"bp":' + str(bp) +' }'
        #print (payload)
        print (data)
        print(myMQTTClient.publish("$aws/things/raspberryPi/shadow/update", data, 0))
        #sleep(4)
        
    else:
        print (".")
        sleep(1) 
       
#loop and publish sensor reading

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

    #client.connect("test.mosquitto.org", 1883, 60)
client.connect("broker.hivemq.com", 1883, 60)

client.loop_forever()    