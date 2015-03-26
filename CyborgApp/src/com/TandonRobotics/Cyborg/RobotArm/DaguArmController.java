package com.TandonRobotics.Cyborg.RobotArm;
import android.util.Log;
import Jama.*;

public class DaguArmController {
	
	public static double[] inverseKinematics(Point target, double[] linkLengths, double[] jointMins, double[] jointMaxes, double disc, boolean leftArm) {
		double minErr = Double.MAX_VALUE;
		double[] jointAnglesRtn = new double[3];
		double[] jointAngles = new double[3];
		for(double t1=jointMins[0]; t1 <= jointMaxes[0]; t1=t1+disc) {
			for(double t2=jointMins[1]; t2 <= jointMaxes[1]; t2=t2+disc) {
				for(double t3=jointMins[2]; t3 <= jointMaxes[2]; t3=t3+disc) {
					jointAngles[0] = t1;
					jointAngles[1] = t2;
					jointAngles[2] = t3;
					Point[] res = forwardKinematics(linkLengths,jointAngles, leftArm);
					double err = Math.sqrt(Math.pow(res[2].getX()-target.getX(), 2)+Math.pow(res[2].getY()-target.getY(), 2));
					if(err < minErr) {
						minErr = err;
						jointAnglesRtn[0] = t1;
						jointAnglesRtn[1] = t2;
						jointAnglesRtn[2] = t3;
					}
				}
			}
		}
		return jointAnglesRtn;
	}
	
	public static int[] anglesToServoPos(double[] jointAngles, int servoMin, int servoMax) {
		int[] servoPos = new int[jointAngles.length];
		for(int x=0; x < jointAngles.length; x++) {
			servoPos[x] = (int) (((jointAngles[x] / Math.PI) * (servoMax-servoMin)) + servoMin);
			Log.d("IOFUCK", jointAngles[x] + " " + servoPos[x]);			
		}
		return servoPos;
	}
	
	public static Point[] forwardKinematics(double[] linkLengths, double[] jointAnglesO, boolean leftArm) {
		
		//deep copy
		double[] jointAngles = new double[jointAnglesO.length];
		for(int x=0; x < jointAngles.length; x++) {
			jointAngles[x] = jointAnglesO[x];
		}
		
		//corrections for joint angle orientations (different for left arm vs. right arm)
		if(!leftArm) {
			jointAngles[0] = jointAngles[0] + Math.PI/2;
			jointAngles[1] = -1*jointAngles[1] + Math.PI/2;
			jointAngles[2] = jointAngles[2] - Math.PI/2;
		}
		else {
			jointAngles[0] = jointAngles[0] - Math.PI/2;
			jointAngles[1] = -1*jointAngles[1] + Math.PI/2;
			jointAngles[2] = jointAngles[2] - Math.PI/2;
		}
		
		//forward kinematics
		Point[] rtn = new Point[linkLengths.length];
		Matrix T = Matrix.identity(4, 4);
		for(int x=0; x < linkLengths.length; x++) {
			double[][] Aarr = {
					{Math.cos(jointAngles[x]), -1*Math.sin(jointAngles[x]), 0, linkLengths[x]*Math.cos(jointAngles[x])},
					{Math.sin(jointAngles[x]), -1*Math.cos(jointAngles[x]), 0, linkLengths[x]*Math.sin(jointAngles[x])},
					{0,0,1,0},
					{0,0,0,1}
			};
			Matrix A = new Matrix(Aarr);
			T = T.times(A);
			rtn[x] = new Point(T.get(0,3), T.get(1, 3));
		}
		return rtn;
	}
}
