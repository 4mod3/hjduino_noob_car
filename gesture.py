# -*- coding:utf-8 -*-
import time
import cv2
import numpy as np
import math
import RPi.GPIO as GPIO
from picamera.array import PiRGBArray
from picamera import PiCamera


GPIO.setmode(GPIO.BCM)

GPIO.setup(18, GPIO.OUT)
GPIO.setup(23, GPIO.OUT)
GPIO.setup(24, GPIO.OUT)
GPIO.setup(25, GPIO.OUT)
GPIO.setup(4, GPIO.OUT)

IN1 = GPIO.PWM(18, 100)
IN2 = GPIO.PWM(23, 100)
IN3 = GPIO.PWM(24, 100)
IN4 = GPIO.PWM(25, 100)

def forward():
    IN1.start(25)
    IN2.start(0)
    IN3.start(25)
    IN4.start(0)
    print("F")

def backward():
    IN1.start(0)
    IN2.start(25)
    IN3.start(0)
    IN4.start(25)
    print("B")

def rotate_left():
    IN1.start(0)
    IN2.start(50)
    IN3.start(50)
    IN4.start(0)
    print("L")

def rotate_right():
    IN1.start(50)
    IN2.start(0)
    IN3.start(0)
    IN4.start(50)
    print("R")

def stop():
    IN1.start(0)
    IN2.start(0)
    IN3.start(0)
    IN4.start(0)
    print("S")

def bee():
    GPIO.output(4, GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(4, GPIO.LOW)
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(640, 480))
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

    frame = frame.array
    frame = cv2.flip(frame,1)
    kernel = np.ones((3,3), np.uint8)

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_skin = np.array([0,20,70], dtype=np.uint8)
    upper_skin = np.array([20,255,255], dtype=np.uint8)

    mask = cv2.inRange(hsv, lower_skin, upper_skin)

    mask = cv2.dilate(mask,kernel,iterations = 4)

    mask = cv2.GaussianBlur(mask,(5,5),100)

    _, contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    try:
        cnt = max(contours, key = lambda x: cv2.contourArea(x))

        epsilon = 0.0005*cv2.arcLength(cnt,True)
        approx = cv2.approxPolyDP(cnt,epsilon,True)

        hull = cv2.convexHull(cnt)

        areahull = cv2.contourArea(hull)
        areacnt = cv2.contourArea(cnt)

        arearatio = ((areahull-areacnt)/areacnt)*100

        hull = cv2.convexHull(approx, returnPoints=False)
        defects = cv2.convexityDefects(approx, hull)

        count = 1

        for i in range(defects.shape[0]):
            s,e,f,d = defects[i,0]
            start = tuple(approx[s][0])
            end = tuple(approx[e][0])
            far = tuple(approx[f][0])
            pt = (100,100)

            a = math.sqrt((end[0] - start[0])**2 + (end[1] - start[1])**2)
            b = math.sqrt((far[0] - start[0])**2 + (far[1] - start[1])**2)
            c = math.sqrt((end[0] - far[0])**2 + (end[1] - far[1])**2)
            s = (a+b+c)/2
            ar = math.sqrt(s*(s-a)*(s-b)*(s-c))

            d = (2*ar)/a

            angle = math.acos((b**2 + c**2 - a**2)/(2*b*c)) * 57

            if angle <= 90 and d>30:
                count += 1
                cv2.circle(frame, far, 3, [255,0,0], -1)

                cv2.line(frame, start, end, [0,255,0], 2)


        print(count)
        
        if count == 1:
            if arearatio<12:
                stop()

            else:
                forward()

        elif count == 2:
            backward()

        elif count == 3:
            rotate_left()

        elif count == 4:
            rotate_right()

        elif count == 5:
            bee()

        else:
            stop()
    except Exception as e:
        stop()
        print(e)

    cv2.imshow('mask', mask)
    cv2.imshow('frame', frame)
    rawCapture.truncate(0)

    k = cv2.waitKey(25) & 0xFF
    if k == 27:
        break








