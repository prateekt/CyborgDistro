package com.TandonRobotics.Cyborg.ArmVis;

import com.TandonRobotics.Cyborg.Activity.BaseActivity;
import com.TandonRobotics.Cyborg.Activity.MainActivity;
import com.TandonRobotics.Cyborg.RobotArm.DaguArmController;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.Log;
import android.view.View;

public class ArmView extends View {

    //robot model for arms
    protected static final double[] linkLengthsLHS = {200,300,300};
    protected static final double[] jointAnglesLHS = {5*Math.PI/6,Math.PI,Math.PI};
    protected static final int[] servoIDLHS = {2,3,0};
    protected static final double[] linkLengthsRHS = {200,300,300};
    protected static final double[] jointAnglesRHS = {Math.PI/6,0,0};
    protected static final int[] servoIDRHS = {7,8,9};
    protected static final int servoMin = 550;
    protected static final int servoMax = 2400; 
    
    //parent
    protected MainActivity parent;
    
	public ArmView(Context context, MainActivity mainActivity) {
		super(context);
		this.parent = mainActivity;
	}
	
	public static void sendServoConfig() {
		
		//convert joint angles to servo positions
		int[] servoLHS = DaguArmController.anglesToServoPos(jointAnglesLHS, servoMin, servoMax);		
		int[] servoRHS = DaguArmController.anglesToServoPos(jointAnglesRHS, servoMin, servoMax);
		
		//make overall arrars
		int[] servoIDsAll = new int[6];
		int[] servoValsAll = new int[6];
		for(int x=0; x < servoIDLHS.length; x++) {
			servoIDsAll[x] = servoIDLHS[x];
			servoValsAll[x] = servoLHS[x];
			Log.d("SERVO_VAL", servoIDsAll[x] + "," + servoValsAll[x]);
		}
		for(int x=0; x < servoIDRHS.length; x++) {
			servoIDsAll[x+3] = servoIDRHS[x];
			servoValsAll[x+3] = servoRHS[x];
			Log.d("SERVO_VAL", servoIDsAll[x+3] + "," + servoValsAll[x+3]);			
		}
		BaseActivity.servoClient.sendServoMessage(servoIDsAll, servoValsAll);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		
		//set up canvas
		super.onDraw(canvas);
		int x = canvas.getWidth();
		int y = canvas.getHeight();
        Paint paint = new Paint();
        paint.setStyle(Paint.Style.FILL);
        paint.setColor(Color.WHITE);
        paint.setStrokeWidth(10);
        canvas.drawPaint(paint);
        paint.setColor(Color.parseColor("#CD5C5C"));
        paint.setStyle(Paint.Style.STROKE);        
        com.TandonRobotics.Cyborg.RobotArm.Point[] linkPosLHS = DaguArmController.forwardKinematics(linkLengthsLHS, jointAnglesLHS, true);
        com.TandonRobotics.Cyborg.RobotArm.Point[] linkPosRHS = DaguArmController.forwardKinematics(linkLengthsRHS, jointAnglesRHS, false);
        int servoDrawRad = 30;
        
        //draw left hand side arm
        int originXLHS = x/2 + 100;
        int originYLHS = y/2;
        paint.setColor(Color.parseColor("#CD5C5C"));        
    	canvas.drawLine((float) (originXLHS), (float) (originYLHS), 
    			(float) (originXLHS+linkPosLHS[0].getX()), (float) (originYLHS+linkPosLHS[0].getY()), paint);
        paint.setColor(Color.GREEN);    	
    	canvas.drawCircle((float)(originXLHS), (float)(originYLHS), servoDrawRad, paint);
        for(int ind=1; ind < linkLengthsLHS.length; ind++) {
            paint.setColor(Color.GREEN);    	        	
        	canvas.drawCircle((float)(originXLHS+linkPosLHS[ind-1].getX()), (float)(originYLHS+linkPosLHS[ind-1].getY()), servoDrawRad, paint);        	
            paint.setColor(Color.parseColor("#CD5C5C"));                	
        	canvas.drawLine((float) (originXLHS+linkPosLHS[ind-1].getX()), (float) (originYLHS+linkPosLHS[ind-1].getY()), 
        			(float) (originXLHS+linkPosLHS[ind].getX()), (float) (originYLHS+linkPosLHS[ind].getY()), paint);
        }
        
        //draw right hand side arm
        int originXRHS = x/2 - 100;
        int originYRHS = y/2;
        paint.setColor(Color.parseColor("#CD5C5C"));                	        
    	canvas.drawLine((float) (originXRHS), (float) (originYRHS), 
    			(float) (originXRHS+linkPosRHS[0].getX()), (float) (originYRHS+linkPosRHS[0].getY()), paint);
        paint.setColor(Color.GREEN);    	
    	canvas.drawCircle((float)(originXRHS), (float)(originYRHS), servoDrawRad, paint);    	
        for(int ind=1; ind < linkLengthsRHS.length; ind++) {
            paint.setColor(Color.GREEN);    	
        	canvas.drawCircle((float)(originXRHS+linkPosRHS[ind-1].getX()), (float)(originYRHS+linkPosRHS[ind-1].getY()), servoDrawRad, paint);    	        	
        	paint.setColor(Color.parseColor("#CD5C5C"));                	                	
        	canvas.drawLine((float) (originXRHS+linkPosRHS[ind-1].getX()), (float) (originYRHS+linkPosRHS[ind-1].getY()), 
        			(float) (originXRHS+linkPosRHS[ind].getX()), (float) (originYRHS+linkPosRHS[ind].getY()), paint);
        }
	}
}