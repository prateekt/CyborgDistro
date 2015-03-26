package com.TandonRobotics.Cyborg.Clients;

import android.app.Activity;
import android.media.AudioManager;
import android.media.ToneGenerator;
import android.preference.PreferenceManager;
import android.util.Log;

import com.TandonRobotics.Cyborg.Socket.SocketConnection;

public class SonarClient {
	
	//state
	protected Activity parent;
	protected SocketConnection socketConn; 		
	protected volatile boolean playTone = false;
	protected boolean[] sonarState;
	
	//constructor
	public SonarClient(Activity parent) {
		this.parent = parent;
		sonarState = new boolean[10];
	}
	
	public void startToneThread() {
		
		//get tone generator
		final ToneGenerator toneG = new ToneGenerator(AudioManager.STREAM_ALARM, 100);
		(new Thread() {
			
			public void run() {
				while(true) {
					try {
						if(playTone) {
							toneG.startTone(ToneGenerator.TONE_CDMA_ALERT_CALL_GUARD, 200);
						}
						Thread.sleep(ClientSettingsDefault.TONE_LATENCY);
					}
					catch(Exception e) {
						e.printStackTrace();
					}
				}
				
			}
			
			
		}).start();
	}
	
	public void startSonarProcessingThread() {
		
		
		//get params
		String serverIP = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getString("SERVER_IP", ClientSettingsDefault.DEFAULT_SERVER_IP);
		int sonarPort = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getInt("SONAR_PORT", ClientSettingsDefault.DEFAULT_SONAR_SERVER_PORT);			
		
		//init socket connection
		socketConn = new SocketConnection(parent, serverIP, sonarPort, ClientSettingsDefault.SONAR_SERVER_POLLING_LATENCY, ClientSettingsDefault.SONAR_SERVER_RECONNECTING_LATENCY) {

			@Override
			public void processMessage(String message) {
				Log.d("OUT","Sonar Server Message: " + message);				
				try {
					
					//parse message
					String[] toks = message.split(" ");
					for(String tok : toks) {
						String[] toks2 = tok.split(",");
						if(toks2.length==2) {
							String portStr = toks2[0].trim();
							int portNum = Integer.parseInt(portStr.substring(portStr.length()-1));
							String stateStr = toks2[1].trim();
							int state = Integer.parseInt(stateStr);
							sonarState[portNum] = (state==1);
						}
					}
					
					//update tone variable based on new state
					playTone = false;
					loope:for(int x=0; x < sonarState.length; x++) {
						if(sonarState[x]) {
							playTone = true;
							break loope;
						}
					}
				}
				catch(Exception e) {
					e.printStackTrace();
				}
			}

			@Override
			public void onConnect() {}
		};
		
		//start thread
		socketConn.startProcessingThread();
		
		//start tone thread
		startToneThread();
	}
	
	public void stopSonarProcessingThread() {
		if(socketConn!=null) {
			socketConn.stopProcessingThread();
		}
	}
		
	public boolean isRunning() {
		return socketConn!=null && socketConn.isRunning();
	}
}