
package com.TandonRobotics.ServoControllerServer;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

import org.steelsquid.ssc32.SSC32Controller;
import org.steelsquid.ssc32.ServoGroup;

public class SteelSquidAPI {
		
	//fields
	protected SSC32Controller ssc32;
	protected ServoGroup servoGroup;
	protected ServoGroup moveGroup;
		
	//singleton
	protected SteelSquidAPI() {
		initController(identifyPort());
		servoGroup = new ServoGroup(true);
		moveGroup = new ServoGroup(true);
	}
	protected static SteelSquidAPI instance;
	public static SteelSquidAPI getInstance() {
		if(instance==null) {
			instance = new SteelSquidAPI();
		}
		return instance;
	}
	
	protected String executeSystemCommand(String command) {
		StringBuffer output = new StringBuffer();
		Process p;
		try {
			p = Runtime.getRuntime().exec(command);
			p.waitFor();
			BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
			String line = "";
			while ((line = reader.readLine())!= null) {
				output.append(line + "\n");
			}	
		} 
		catch (Exception e) {
			e.printStackTrace();
		}
		return output.toString();
	}	
	
	public String identifyPort() {
		for(int x=0; x < 50; x++) {
			String command =  "udevadm info --query=property -n /dev/ttyUSB"+x;
			String result = executeSystemCommand(command);
			if(result.contains("pl2303")) {
				System.out.println("Servo Controller identified on /dev/ttyUSB" + x);
				return "/dev/ttyUSB" + x;
			}
		}
		return null;
	}
	
	public void initController(String port) {
		try {
			if(port!=null) {
				System.out.println("Attempting to start Servo Controller on " + port);
				ssc32 = new SSC32Controller(port);
			}
			else {
				System.out.println("Could not identify port for servo controller.");
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public void moveServos(int[] servoIDs, int[] pos) {
		try {
			if(ssc32==null) {
				System.out.println("Could not move servos since ssc32 is not inited.");
				return;
			}
			if(servoIDs.length > 0) {
				moveGroup.reset(false);
				for(int x=0; x < servoIDs.length; x++) {
					moveGroup.setMoveSpeed(servoIDs[x],pos[x], Consts.SERVO_SPEED);
				}
				ssc32.move(moveGroup);
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public void moveServo(int servoID, int pos) {
		try {
			if(ssc32!=null) {
				ssc32.moveSpeed(servoID, pos,Consts.SERVO_SPEED);
				//ssc32.waitForLastMoveToComplete();
			}
			else {
				System.out.println("Could not move servos since ssc32 is not inited.");
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public Map<Integer,Integer> queryServoPositions() {
		try {
			if(ssc32!=null) {
				
				//build group command
				servoGroup.reset(false);
				for(int servoID : Consts.SERVO_IDS) { 	
					servoGroup.queryPosition(servoID);
				}
			
				//run
				ssc32.queryPositions(servoGroup);
			//	ssc32.waitForLastMoveToComplete();
			//	ssc32.move(0, (int)(2400*Math.random()));

				//return
				Map<Integer, Integer> rtn = new HashMap<Integer,Integer>();
				for(int servoID : Consts.SERVO_IDS) {
					int pos = servoGroup.getPosition(servoID);
					rtn.put(servoID, pos);
				}
				return rtn;
			}
			else {
				System.out.println("Could not query servos state since ssc32 is not inited.");				
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
		return null;
	}
}