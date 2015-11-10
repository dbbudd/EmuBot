#!/usr/bin/env python

import serial
import time

#import the default functions from the EmuBot.py file
from EmuBot import *

#configure dynamixel modes
def setupDxl():
    #Wheel IDs are 1,2,3,4
    wheelMode(1) 
    wheelMode(2)
    wheelMode(3)
    wheelMode(4)

    #Joint IDs are 5,6,7
    jointMode(5) 
    jointMode(6)
    jointMode(7)

def testJoints():
    '''
    moveJoint(ID,POSITION,SPEED)
    ID = 5,6 or 7
    POSITION = 0-999  .... test limits carefully!
    SPEED = 0-999 ... start slow!
    '''
    moveJoint(7,800,200)
    time.sleep(1)
    moveJoint(7,512,200)
    
def testWheels():
    '''
    moveWheel(ID, SPEED)
    ID = 1,2,3,4
    SPEED = -999(reverse) to 999(forward) ... test slowly
    '''
    moveWheel(1,200)
    time.sleep(2)
    moveWheel(1,-200)
    time.sleep(2)
    moveWheel(1,0)

if __name__ == "__main__":
    setupDxl()
    testJoints()
    testWheels()