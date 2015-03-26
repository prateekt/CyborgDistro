package com.TandonRobotics.ServoControllerServer;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;
import java.net.*;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReentrantLock;

public class Server {
		
	public static void main(String[] args) {
		try {
			SteelSquidAPI.getInstance();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
		Server s = new Server();
		s.startServer();
	}
	
	//servo states
	protected volatile Map<Integer,Integer> servoStates = new HashMap<Integer,Integer>();
	protected ReentrantLock lock = new ReentrantLock();
	protected int numConn;
	
	public void startServer() {
		try {
						
			//create server socket
			System.out.println("Creating Server Socket...");
			ServerSocket server = new ServerSocket(Consts.SERVO_SERVER_PORT);
			System.out.println("Server Socket Created!");
			
			//start servo querying thread
			Thread servoQueryingThread =  new Thread(new Runnable() {
				public void run() {
					while(true) {
						try {
							lock.lock();
							System.out.println("Querying Servo Positions");
							try {
								servoStates = SteelSquidAPI.getInstance().queryServoPositions();
							}
							catch(Exception e) {
								e.printStackTrace();
							}
							lock.unlock();
							Thread.sleep(Consts.SERVO_STATE_SENDING_MESSAGING_LATENCY);
						}
						catch(Exception e) {
							e.printStackTrace();
						}
					}
				}
			});
			servoQueryingThread.start();
			
			//start listening
			while(true) {
				
				//wait for and accept client connection
				final Socket client = server.accept();
				System.out.println("Got Client");
				numConn++;
				final StatusFlag stillConnected = new StatusFlag();
				stillConnected.flag = true;
				
				//get streams
				final BufferedWriter out = new BufferedWriter(new OutputStreamWriter(client.getOutputStream()));
				final BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
				
				//start thread for sending servo state over wire
				Thread servoStateSendingThread = new Thread(new Runnable() {
					public void run() {
						Map<Integer,Integer> lastServoStates = null;
						while(client.isConnected() && stillConnected.flag) {
							try {
								
								//acquire lock
								lock.lock();
																
								//compute diff map
								System.out.println("COMPUTING DIFF MAP");
								Map<Integer,Integer> diffMap = servoStates;
								
								//remove lock
								lock.unlock();
								
								//compute message to send
								if(diffMap!=null && diffMap.size()>0) {
								
									//send message to client
									StringBuffer rtn = new StringBuffer();
									for(int servoID : diffMap.keySet()) {
										rtn.append(servoID + "," + diffMap.get(servoID) + " ");
									}
									String message = rtn.substring(0, rtn.length()-1);
									if(!message.equals("")) {
										System.out.println("Wrote Message: " + message);
										out.write(message+"\n");
										out.flush();
									}
								}
								
								//update state
								lock.lock();
								lastServoStates = servoStates;
								lock.unlock();
								
								//wait
								Thread.sleep(Consts.SERVO_STATE_SENDING_MESSAGING_LATENCY);
							}
							catch(Exception e) {
								e.printStackTrace();
							}
						}
					}
				});
				servoStateSendingThread.start();
								
				//start thread for receiving servo commands
				Thread servoStateReceivingThread = new Thread(new Runnable() {
					public void run() {
						int numConsecNullMessages = 0;
						while(client.isConnected() && stillConnected.flag) {
							try {
								
								//read message
								System.out.println("WAITING ON RECEIVE");
								String message = in.readLine();
								System.out.println("Message Received: " + message);
								
								//handle null message case -- exit if client connection is broken
								if(message==null) {
									numConsecNullMessages++;
									if(numConsecNullMessages >= Consts.SERVO_SERVER_DISCONNECT_CTR) {
										stillConnected.flag = false;
									}
								}
								
								//process message
								if(message!=null && !message.equals("")) {
									
									//debug
									numConsecNullMessages  =0;
									System.out.println("Latest Message: " + message);
									
									//parse message
									String[] servoMoves = message.split(" ");
									int[] servoIds = new int[servoMoves.length];
									int[] pos = new int[servoMoves.length];
									int ctr=0;
									for(String servoMove : servoMoves){
										String[] toks = servoMove.split(",");
										int servoID = Integer.parseInt(toks[0].trim());
										int newPos = Integer.parseInt(toks[1].trim());
										servoIds[ctr]  = servoID;
										pos[ctr] = newPos;
										ctr++;
									}
									
									//move servo
									System.out.println("Moving Servos " + servoIds + " " + pos);
									try {
										SteelSquidAPI.getInstance().moveServos(servoIds, pos);
									}
									catch(Exception ex) {
										ex.printStackTrace();
									}
//									System.out.println("Moving servo " + servoID + " " + newPos);
//									SteelSquidAPI.getInstance().moveServo(servoID, newPos);

								}
								
								//wait
								Thread.sleep(Consts.SERVO_STATE_RECEIVER_MESSAGING_LATENCY);
								
							}
							catch(Exception e) {
								e.printStackTrace();
							}
						}
					}
				});
				servoStateReceivingThread.start();
			}
			
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public Map<Integer,Integer> computeDiffMap(Map<Integer,Integer> lastState,Map<Integer,Integer> currentState) {
		Map<Integer,Integer> rtn = new HashMap<Integer,Integer>();
		if(lastState==null) {
			rtn = currentState;
		}
		else {
			if(currentState!=null) {
				for(int servoID : currentState.keySet()) {
					int servoVal = currentState.get(servoID);
					int oldServoVal = lastState.get(servoID);
					if(servoVal!=oldServoVal) {
						rtn.put(servoID, servoVal);
					}
				}
			}
		}
		return rtn;
	}
}