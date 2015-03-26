package com.TandonRobotics.SystemID;

import java.util.List;
import org.steelsquid.classcollection.common.Constants;
import org.steelsquid.classcollection.common.UtilsConsole;
import org.steelsquid.ssc32.SSC32Controller;

public class ServoTest {
	
	public static void main(String[] args) {
		final String port = "/dev/ttyUSB0"; //port to connect to (change if needed)  NOPMD
		SSC32Controller ssc32 = null; //NOPMD
		try {
			List<String> list  = SSC32Controller.getComPortList();
			for(String s : list) {
				System.out.println(s);
			}
	        ssc32 = new SSC32Controller(port);
			int ctr = 0;
			int step=50;
			for(int x=0; ctr+x*step < 2400; x++){
				ssc32.move(0,ctr+x*step);
				System.out.println("POS: " + (ctr+x*step));
				Thread.sleep(500);
			}	
			ssc32.disconnect();
			
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public String toString() {
		return "ServoTest []";
	}
}