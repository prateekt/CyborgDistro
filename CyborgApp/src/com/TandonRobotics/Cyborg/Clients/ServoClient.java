package com.TandonRobotics.Cyborg.Clients;

import android.app.Activity;
import android.preference.PreferenceManager;
import android.util.Log;

import com.TandonRobotics.Cyborg.Activity.RobotArmControlActivity;
import com.TandonRobotics.Cyborg.ArmVis.ArmView;
import com.TandonRobotics.Cyborg.RobotArm.RobotArm;
import com.TandonRobotics.Cyborg.RobotArm.RobotArmSettings;
import com.TandonRobotics.Cyborg.Socket.SocketConnection;

public class ServoClient {

	//state
	protected volatile Activity parent;
	protected SocketConnection socketConn;
	
	public ServoClient(Activity parent) {
		this.parent = parent;
	}
	
	public void startServoProcessingThread() {
		
		//get params
		String serverIP = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getString("SERVER_IP", ClientSettingsDefault.DEFAULT_SERVER_IP);
		int sonarPort = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getInt("SERVO_PORT", ClientSettingsDefault.DEFAULT_SERVO_SERVER_PORT);
		
		//init socket connection
		final BooleanValHolder initCase = new BooleanValHolder();
		initCase.val = true;
		socketConn = new SocketConnection(parent, serverIP, sonarPort, ClientSettingsDefault.SERVO_SERVER_POLLING_LATENCY, ClientSettingsDefault.SERVO_SERVER_RECONNECTING_LATENCY) {
			@Override
			public void processMessage(String message) {
				
				//debug
				Log.d("OUT","Servo Server Message: " + message);
				
				//parse and update servo map
				String[] toks = message.split(" ");
				for(String tok : toks) {
					String[] strs = tok.split(",");
					int servoID = Integer.parseInt(strs[0]);
					int newVal = Integer.parseInt(strs[1]);
					RobotArm arm = RobotArmSettings.getServoIDsToRobotArm().get(servoID);
					Log.d("SERVO_D", servoID + " " + newVal + " " + arm);
					arm.getJointCurrentVals().put(servoID, newVal);
				}
				
				//update GUI
				Log.d("PARENT", "PARENT:" + parent);
				if(parent!=null && parent instanceof RobotArmControlActivity) {
					RobotArmControlActivity parentAct = (RobotArmControlActivity) parent;
					parentAct.updateGUI(initCase.val);
					initCase.val = false;
				}
			}

			@Override
			public void onConnect() {
				//send init message
				try {
					Thread.sleep(5000);
					ArmView.sendServoConfig();
//					socketConn.sendMessage(RobotArmSettings.getInitMessage());
				}
				catch(Exception e) {
					e.printStackTrace();
				}
			}
		};
				
		//start thread
		socketConn.startProcessingThread();		
		
	}

	public void sendServoMessage(String joint, int newValue) {
		
		//get servo ID
		int servoID = RobotArmSettings.getJointNameToServoID().get(joint);
		
		//send message for moving servo to particular value
		Log.d("SOCK", ""+socketConn);
		if(socketConn!=null) {
			socketConn.sendMessage(servoID + "," + newValue);
		}
	}

	public void sendServoMessage(int[] servoIDs, int[] newValues) {
		//create message
		StringBuffer msg = new StringBuffer(); 
		for(int x=0; x < servoIDs.length; x++) {
			
			//get
			int newValue = newValues[x];		
			int servoID = servoIDs[x];
			
			//put message
			msg.append(servoID + "," + newValue + " ");
		}
		
		//send message for moving servo to particular value
		Log.d("MESSAGE1", msg.toString().substring(0, msg.length()-1));		
		if(socketConn!=null && msg.toString().trim().length() > 0) {
			socketConn.sendMessage(msg.toString().substring(0, msg.length()-1));
		}
	}
	
	public void sendServoMessage(String[] joints, int[] newValues) {
		
		//create message
		StringBuffer msg = new StringBuffer(); 
		for(int x=0; x < joints.length; x++) {
			
			//get
			String joint = joints[x];
			int newValue = newValues[x];		
			int servoID = RobotArmSettings.getJointNameToServoID().get(joint);
			
			//put message
			msg.append(servoID + "," + newValue + " ");
		}
		
		//send message for moving servo to particular value
		if(socketConn!=null && msg.toString().trim().length() > 0) {
			socketConn.sendMessage(msg.toString().substring(0, msg.length()-1));
		}

	}
		
	public boolean isRunning() {
		return socketConn!=null && socketConn.isRunning();
	}
	
	public void stopServoProcessingThread() {
		if(socketConn!=null) {
			socketConn.stopProcessingThread();
		}
	}
	
	class BooleanValHolder {
		boolean val;
	}

	/**
	 * @param parent the parent to set
	 */
	public void setParent(Activity parent) {
		Log.d("RESUMING ACTIVITY...", "SET PARENS: " + parent);
		this.parent = parent;
		if(socketConn!=null) {
			socketConn.setParent(parent);
		}
	};
}