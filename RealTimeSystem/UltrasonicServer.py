#!/usr/bin/env python
from subprocess import Popen,PIPE
import socket
import ctypes
import time 

def getSonarUSBPortsO():
	rtnList = []
	for i in range(0,50):
		command = 'udevadm info -a -n /dev/ttyUSB'+str(i)+' | grep  \'{serial}\' | head -n1'
		output = Popen(command, shell=True, stdout=PIPE,stderr=PIPE).communicate()
		output = str(output)
		if output.find('0000:00:1d.7') != -1:
			rtnList.append('/dev/ttyUSB'+str(i))
	return rtnList

def getSonarUSBPorts():
	rtnList = []
	for i in range(0,10):
		command = 'udevadm info --query=property -n /dev/ttyUSB'+str(i)
		output = Popen(command, shell=True, stdout=PIPE,stderr=PIPE).communicate()
		output = str(output)
		if output.find('ch341') != -1:
			rtnList.append('/dev/ttyUSB'+str(i))
	return rtnList

def startSonarSensor(sonarPort):
	return ctypes.CDLL('/home/pi/Desktop/CyborgDistro/RealTimeSystem/Ultrasonic_device_shared_lib.so').Ultrasonic_start(sonarPort)
	
def pollSensorValue(sonarPort):
	return ctypes.CDLL('/home/pi/Desktop/CyborgDistro/RealTimeSystem/Ultrasonic_device_shared_lib.so').Ultrasonic_getValue(sonarPort)/10.0

def createServerSocket():
	host = ''
	port = 5000
	backlog = 5
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((host,port))
	s.listen(backlog)
	return s

def startSonarMonitor(T,W):
	
	#get connected sonars
	sonarPorts = getSonarUSBPorts()
	print sonarPorts

	#start all sonars
	sonarPortsStarted = {}
	lastState = {}
	for sonarPort in sonarPorts:
		sonarPortsStarted[sonarPort] =  startSonarSensor(sonarPort)
		lastState[sonarPort] = -1
	print sonarPortsStarted

	#set up server socket	
	socketCreated = False
	while not socketCreated:
		try:
			serverSocket = createServerSocket()	
			socketCreated = True
			print 'Socket Succesfully Created'
		except socket.error, msg:
			print 'Failed to create socket. Error code: ' + str(msg[0]) + ' , Error message: ' + msg[1]
			socketCreated = False
			time.sleep(1)
		
	#global loop for networking
	while True:
		
		#wait to connect to phone
		print 'Waiting to connect to phone...'
		client, address = serverSocket.accept()
		print 'Connected to phone'

		#monitoring loop for this particular connection
		connected = True
		while connected:
			
			#find sensors that have changed state
			currentState = {}
			currentVals = {}
			for sonarPort in sonarPorts:
				if sonarPortsStarted[sonarPort]:
					sensorVal = pollSensorValue(sonarPort)
					currentVals[sonarPort] = sensorVal
					if sensorVal < T:
						currentState[sonarPort] = 1
					else:
						currentState[sonarPort] = 0
				else:
					currentVals[sonarPort] = -1
					currentState[sonarPort] = -1		
			
			#compose message for newly occluded sensors
			message = ''
			for sonarPort in sonarPorts:
				if sonarPortsStarted[sonarPort] and lastState[sonarPort]==0 and currentState[sonarPort]==1:		
					message = message + sonarPort  + ',' + str(currentState[sonarPort]) + ' '
				if sonarPortsStarted[sonarPort] and lastState[sonarPort]==1 and currentState[sonarPort]==0:
					message = message + sonarPort + ',' + str(currentState[sonarPort]) + ' '

			#update last state
			lastState = currentState
	
			#send message
			if message != '':
				print message
				try:
					client.send(message + '\n')
				except socket.error:
					print 'Sent Failed'
					connected = False

			
			#sleep
			time.sleep(W)
	
startSonarMonitor(20,0.25)