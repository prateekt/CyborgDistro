package com.TandonRobotics.Cyborg.Activity;

import android.app.Activity;
import android.app.FragmentTransaction;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

import com.TandonRobotics.Cyborg.R;
import com.TandonRobotics.Cyborg.Clients.ServoClient;
import com.TandonRobotics.Cyborg.Clients.SonarClient;
import com.TandonRobotics.Cyborg.RobotArm.RobotArm;
import com.TandonRobotics.Cyborg.RobotArm.RobotArmSettings;
import com.TandonRobotics.Cyborg.Socket.SocketSettingsDialog;

public class BaseActivity extends Activity {
	
	//clients
	public static SonarClient sonarClient;
	public static ServoClient servoClient;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		//super
		super.onCreate(savedInstanceState);
		
		//init sonar client and start
		if(sonarClient==null) {
			sonarClient = new SonarClient(this);
		}
		if(!sonarClient.isRunning()) {
			sonarClient.startSonarProcessingThread();
		}
		
		//init servo client and start
		if(BaseActivity.servoClient==null) {
			Log.d("SERVO CLIENT CREATED", "SERVO CLIENT CREATED");
			BaseActivity.servoClient = new ServoClient(this);
		}
		if(!BaseActivity.servoClient.isRunning()) {
			BaseActivity.servoClient.startServoProcessingThread();
		}
	}
	
	@Override
	public void onResume(){
		super.onResume();
		Log.d("RESUMING ACTIVITY...", "RESUME");
		if(BaseActivity.servoClient!=null) {			
			BaseActivity.servoClient.setParent(this);		
		}
	}
		
	public void startSocketThreads() {
//		sonarClient.startSonarProcessingThread();
		BaseActivity.servoClient.startServoProcessingThread();
	}
	
	public void stopSocketThreads() {
//		sonarClient.stopSonarProcessingThread();
		BaseActivity.servoClient.stopServoProcessingThread();
	}
	
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }    
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
        	case R.id.ServerSettingsMenuItem:
        		showSocketSettings();
        		return true;
        	case R.id.CameraServerMenuItem:
        		showCameraActivity();
        		return true;
        	case R.id.SonarServerMenuItem:
        		showSonarActivity();
        		return true;
        	case R.id.LeftRobotArmMenuItem:
        		showRobotArmActivity(RobotArmSettings.getDaguLeftArm());
        		return true;
        	case R.id.RightRobotArmMenuItem:
        		showRobotArmActivity(RobotArmSettings.getDaguRightArm());        		
        		return true;
        	case R.id.RobotTailMenuItem:
//        		showRobotArmActivity(RobotArmSettings.getLynxMotionArm());        		
        		return true;
        	case R.id.ExtraJointsMenuItem:
        		showRobotArmActivity(RobotArmSettings.getDaguGrippers());        		        		
        		return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
    
    public void showSocketSettings() {
        // Create and show the dialog.
        SocketSettingsDialog newFragment = new SocketSettingsDialog (this);
    	FragmentTransaction ft = getFragmentManager().beginTransaction();        
        newFragment.show(ft, "dialog");
    }
    
    public void showCameraActivity() {
		Intent i = new Intent(this, CameraActivity.class);
		i.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
		startActivity(i);
    }
   
    public void showRobotArmActivity(RobotArm arm) {
    	RobotArmSettings.currentRobotArmGUI = arm;
		Intent i = new Intent(this, RobotArmControlActivity.class);
		i.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
		startActivity(i);
    }
    
    public void showSonarActivity() {}
    
}
