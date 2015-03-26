package com.TandonRobotics.ServoControllerServer;

public class Consts {
	
	//server
	public static final int SERVO_SERVER_PORT = 6134;
	public static final int SERVO_STATE_SENDING_MESSAGING_LATENCY = 2000;
	public static final int SERVO_STATE_RECEIVER_MESSAGING_LATENCY = 500;
	public static final int SERVO_SERVER_DISCONNECT_CTR = 10;
	
	//servo stuff
//	public static final int SERVO_SPEED = 150;
	public static final int SERVO_SPEED = 400;
	public static final int[] SERVO_IDS = {0,1,2,3,4,5,6,7,8,9,10,11};
	
}