package com.TandonRobotics.SystemID;

import java.util.List;
import org.steelsquid.classcollection.common.Constants;
import org.steelsquid.classcollection.common.UtilsConsole;
import org.steelsquid.ssc32.SSC32Controller;

import com.TandonRobotics.ServoControllerServer.SteelSquidAPI;

public class MultiArmTest {
	
	public static void main(String[] args) {
		SteelSquidAPI.getInstance();
		int[] servoIDs = {31,30,29,28,27,26};
		int[] pos0 = {500,500,500,500,500,500};
		int[] posf = {2400,2400,2400,2400,2400,2400};
		while(true) {
			try {
				SteelSquidAPI.getInstance().moveServos(servoIDs,posf);
				Thread.sleep(10000);
				SteelSquidAPI.getInstance().moveServos(servoIDs, pos0);
				Thread.sleep(10000);
			}
			catch(Exception e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public String toString() {
		return "ServoTest []";
	}
}