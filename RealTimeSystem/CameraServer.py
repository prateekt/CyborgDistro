#!/usr/bin/env python
import os
import time

def takePictures():	
	os.system('streamer -c /dev/video0 -f jpeg -b 15 -o imgs/image0.jpeg')
	os.system('streamer -c /dev/video1 -f jpeg b 15 -o imgs/image1.jpeg')

def startPictureServer(W):
	while True:
		takePictures()
		time.sleep(W)

startPictureServer(0)
