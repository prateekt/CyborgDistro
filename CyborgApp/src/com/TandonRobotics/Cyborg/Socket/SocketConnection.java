package com.TandonRobotics.Cyborg.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

import android.app.Activity;

public abstract class SocketConnection {
		
	//io to server
	protected Scanner sc;
	protected BufferedReader in;
	public Socket socket;
	public PrintWriter out;
	protected Thread closeSocketOnShutdown;
	
	//params
	protected String domainName;
	protected int port;
	protected int processingLatency;	
	protected int reconnectionLatency;
	protected volatile Activity parent;
	
	//state
	protected volatile boolean running = false;
	
	public SocketConnection(Activity parent, String domainName, int port, int processingLatency, int reconnectionLatency) {
		this.parent = parent;
		this.domainName = domainName;
		this.port = port;
		this.processingLatency = processingLatency;
		closeSocketOnShutdown = new Thread() {
			public void run() {
				try {
					socket.close();
				}
				catch(Exception e) {
					e.printStackTrace();
				}
			}
		};
	}
	
	public void initializeConnection() {
		try {
//			String domainName = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getString("SOCKET_DOMAIN", SettingsDefault.DEFAULT_SERVER_IP);
//			int port = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getInt("SOCKET_PORT", SettingsDefault.DEFAULT_SERVER_PORT);
			socket = new Socket(domainName,port);
			out = new PrintWriter(socket.getOutputStream(), true);
			in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			sc = new Scanner(socket.getInputStream());
			Runtime.getRuntime().addShutdownHook(closeSocketOnShutdown);
			onConnect();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public void sendMessage(String message) {
		if(out!=null) {
			out.println(message);
//			out.flush();
		}
	}
	
	public void startProcessingThread() {
		
		//start processing thread thread
		(new Thread() {
			public void run() {
				running = true;
				
				//loop for reconnecting to server
				while(running) {				
					try {
						
						//initialize connection
						initializeConnection();
						
						//loop for reading data
						while(running) {
							String s = in.readLine();
							if(s!=null && !s.equals("")) {
								processMessage(s);
							}
							try {
								Thread.sleep(processingLatency);
							}
							catch(Exception e) {
								e.printStackTrace();
							}
						}
					}
					catch(Exception e) {
						e.printStackTrace();
						try {
							Thread.sleep(reconnectionLatency);
						}
						catch(Exception ex) {
							ex.printStackTrace();
						}
					}
				}
			}
		}).start();
	}
	
	public abstract void processMessage(String message);
	public abstract void onConnect();
	
	public void stopProcessingThread() {
		running = false;
	}

	/**
	 * @return the running
	 */
	public boolean isRunning() {
		return running;
	}

	/**
	 * @param parent the parent to set
	 */
	public void setParent(Activity parent) {
		this.parent = parent;
	}	
}