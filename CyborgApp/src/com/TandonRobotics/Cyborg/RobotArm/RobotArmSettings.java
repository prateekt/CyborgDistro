package com.TandonRobotics.Cyborg.RobotArm;

import java.util.HashMap;
import java.util.Map;

public class RobotArmSettings {
	
    //lynx motion hard-coded robot arm specification
	/*
    protected static RobotArm lynxMotionArm;
    public static RobotArm getLynxMotionArm() {
    	if(lynxMotionArm==null) {
    		String name = "LynxMotion Arm";
    		int[] jointServoIDs = {0,1,2,3,4};
    		String[] jointLabels = {"Swivel Base", "Shoulder", "Elbow", "Gripper", "Wrist"};
    		int[] jointMins = {400,750,700,800,550};
    		int[] jointMaxes = {2400,1300,800,2400,2400};
    		int[] jointInitPos = {-1,1000,750,1440,1300};
    		lynxMotionArm = new RobotArm(name, jointServoIDs, jointLabels,jointMins, jointMaxes,jointInitPos);
    	}
    	return lynxMotionArm;
    }
    */
    
    //Dagu Arm 1 (LEFT)
    protected static RobotArm daguLeftArm;
    public static RobotArm getDaguLeftArm() {
    	if(daguLeftArm==null) {
    		String name = "Dagu (Left)";
    		int[] jointServoIDs = {0,1,2,3,4,5};
    		String[] jointLabels = {"Wrist", "Wrist Rotator", "Shoulder", "Elbow", "Swivel Base", "Gripper"};
    		int[] jointMins = {550,550,550,550,550,550};
    		int[] jointMaxes = {2400,2400,2400,2400,2400,2400};
    		int[] jointInitPos = {1700,1230,1610,845,830,2140};
    		daguLeftArm = new RobotArm(name, jointServoIDs, jointLabels,jointMins, jointMaxes,jointInitPos);
    	}
    	return daguLeftArm;
    }

    //Dagu Arm 2 (RIGHT)
    protected static RobotArm daguRightArm;
    public static RobotArm getDaguRightArm() {
    	if(daguRightArm==null) {
    		String name = "Dagu (Right)";
    		int[] jointServoIDs = {6,7,8,9,10,11};
    		String[] jointLabels = {"Swivel Base", "Shoulder", "Elbow", "Wrist", "Wrist Rotator", "Gripper"};
    		int[] jointMins = {550,550,550,550,550,550};
    		int[] jointMaxes = {2400,2400,2400,2400,2400,2400};
    		int[] jointInitPos = {780,1590,1830,900,1330,2350};    		
    		daguRightArm = new RobotArm(name, jointServoIDs, jointLabels,jointMins, jointMaxes,jointInitPos);
    	}
    	return daguRightArm;
    }
    
    //Dagu Gripper1
    protected static RobotArm daguGrippers;
    public static RobotArm getDaguGrippers() {
    	if(daguGrippers==null) {
    		String name = "Extra Dagu Extra Grippers";
    		int[] jointServoIDs = {28};
    		String[] jointLabels = {"Hat Gripper"};
    		int[] jointMins = {550};
    		int[] jointMaxes = {2400};
    		int[] jointInitPos = {2350};
    		daguGrippers = new RobotArm(name, jointServoIDs, jointLabels,jointMins, jointMaxes,jointInitPos);
    	}
    	return daguGrippers;
    }    
    
    //global mapping of Cyborg joints to IDS
    protected static Map<String,Integer> jointNameToServoID;
    public static Map<String,Integer> getJointNameToServoID() {
    	if(jointNameToServoID==null) {
    		jointNameToServoID = new HashMap<String,Integer>();
    		RobotArm[] arms = getAllArms();
    		for(RobotArm arm : arms) {
    			for(int x=0; x < arm.getJointServoIDs().length; x++) {
    				int servoID = arm.getJointServoIDs()[x];
    				jointNameToServoID.put(arm.getName() + ":" + arm.getJointLabels().get(servoID), arm.getJointServoIDs()[x]);
    			}
    		}
    	}
    	return jointNameToServoID;
    }
    
    //global mapping of Cyborg servo ids to arm
    protected static Map<Integer,RobotArm> servoIDsToRobotArm;
    public static Map<Integer,RobotArm> getServoIDsToRobotArm() {
		if(servoIDsToRobotArm==null) {
			servoIDsToRobotArm = new HashMap<Integer,RobotArm>();
			RobotArm[] arms = getAllArms();
			for(RobotArm arm : arms) {
				for(int servoID : arm.getJointServoIDs()) {
					servoIDsToRobotArm.put(servoID, arm);
				}
			}
		}
		return servoIDsToRobotArm;
    }
    
    public static String getInitMessage() {
    	StringBuffer rtn = new StringBuffer();
    	RobotArm[] arms = getAllArms();
		for(RobotArm arm : arms) {
			for(int servoID : arm.getJointServoIDs()) {
				if(arm.getJointInitPos().get(servoID)!=-1) {
					rtn.append(servoID + "," + arm.getJointInitPos().get(servoID) + " ");
				}
			}
		}
		return rtn.toString().substring(0, rtn.toString().length()-1);
    }
    
    public static RobotArm[] getAllArms() {
		RobotArm[] arms = {RobotArmSettings.getDaguLeftArm(), RobotArmSettings.getDaguRightArm(), RobotArmSettings.getDaguGrippers()};
		return arms;
    }
    
    //variable for GUI
    public static RobotArm currentRobotArmGUI = null;
    	
}