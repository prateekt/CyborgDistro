package com.TandonRobotics.Cyborg.RobotArm;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import android.util.SparseArray;
import android.util.SparseIntArray;

public class RobotArm {
	
	//fields
	protected String name;
	protected int[] jointServoIDs;	
	protected SparseArray<String> jointLabels;
	protected SparseIntArray jointMins;
	protected SparseIntArray jointMaxes;	
	protected SparseIntArray jointInitPos;
	protected Map<Integer, Integer> jointCurrentVals;
	protected Map<Integer, Integer> jointSuggestedVals;
	
	public RobotArm(String name, int[] jointServoIDs, String[] jointLabelsArr, int[] jointMinsArr, int[] jointMaxesArr, int[] jointInitPosArr) {
		this.name = name;
		this.jointServoIDs = jointServoIDs;
		jointLabels = new SparseArray<String>();
		jointMins = new SparseIntArray();
		jointMaxes = new SparseIntArray();
		jointInitPos = new SparseIntArray();
		jointCurrentVals = new ConcurrentHashMap<Integer,Integer>();
		jointSuggestedVals = new ConcurrentHashMap<Integer,Integer>();
		for(int x=0; x < jointServoIDs.length; x++) {
			int servoID = jointServoIDs[x];
			jointLabels.put(servoID, jointLabelsArr[x]);
			jointMins.put(servoID, jointMinsArr[x]);
			jointMaxes.put(servoID, jointMaxesArr[x]);
			jointInitPos.put(servoID, jointInitPosArr[x]);			
			jointCurrentVals.put(servoID,0);
			jointSuggestedVals.put(servoID, 0);
		}
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return the jointLabels
	 */
	public SparseArray<String> getJointLabels() {
		return jointLabels;
	}

	/**
	 * @return the jointMins
	 */
	public SparseIntArray getJointMins() {
		return jointMins;
	}

	/**
	 * @return the jointMaxes
	 */
	public SparseIntArray getJointMaxes() {
		return jointMaxes;
	}

	/**
	 * @return the jointServoIDs
	 */
	public int[] getJointServoIDs() {
		return jointServoIDs;
	}
		
	/**
	 * @return the jointCurrentVals
	 */
	public Map<Integer, Integer> getJointCurrentVals() {
		return jointCurrentVals;
	}

	/**
	 * @return the jointSuggestedVals
	 */
	public Map<Integer, Integer> getJointSuggestedVals() {
		return jointSuggestedVals;
	}

	public String toString() {
		return name;
	}

	/**
	 * @return the jointInitPos
	 */
	public SparseIntArray getJointInitPos() {
		return jointInitPos;
	}
	
}
