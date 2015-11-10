#!/usr/bin/env python

import serial
import time

s = serial.Serial()                        # create a serial port object
s.baudrate = 57600                        # baud rate, in bits/second
s.port = "/dev/ttyAMA0"            # this is whatever port your are using
s.timeout = 3.0
s.open()



def jointMode(ID):
        s.write('W'+'j'+chr(ID))


def wheelMode(ID):
        s.write('W'+'w'+chr(ID))

def readDxl(ID,cmd):
        #j m or s
        output = ""
        s.write('R'+str(cmd)+chr(ID))
        for line in s.readline():
                output = output + line
        return int(output[-3:])        

def moveJoint(ID, position, speed):
        #move to position between 0-1024
        #512 is 12 o'clock
        #WRITE ID
        s.write('W'+'p'+chr(ID))

        #write position        
        position = int(position)
        s.write(chr(int(position)%256))
        s.write(chr(int(position)>>8))
        
        #WRITE SPEED
        velocity = int(speed)
        s.write(chr(int(velocity)%256))
        s.write(chr(int(velocity)>>8))

        #READ POSITION
        time.sleep(0.5)
        print(readDxl(ID,"j"))
        

def moveWheel(ID, speed):
        
        s.write('W'+'s'+chr(ID))
        
        #WRITE ADDRESS
        addr = int(32)
        s.write(chr(int(addr)%256))
        s.write(chr(int(addr)>>8))
        
        #WRITE SPEED        
        if speed >= 0:
                #Wrapper for forward speed
                speed = int(speed)
        else:
                #Wrapper for backward speed
                speed = int(1024 + int(-speed))
      
        s.write(chr(int(speed)%256))
        s.write(chr(int(speed)>>8))

