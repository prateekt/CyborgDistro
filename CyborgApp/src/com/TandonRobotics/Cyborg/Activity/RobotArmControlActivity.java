package com.TandonRobotics.Cyborg.Activity;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import android.app.ActionBar.LayoutParams;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

import com.TandonRobotics.Cyborg.R;
import com.TandonRobotics.Cyborg.RobotArm.RobotArm;
import com.TandonRobotics.Cyborg.RobotArm.RobotArmSettings;

public class RobotArmControlActivity extends BaseActivity {
		
	//state
	protected List<ServoLayoutStructure> layoutStructs;
	
	@Override
	public void onResume() {
		super.onResume();
		RobotArm robotArm = RobotArmSettings.currentRobotArmGUI;
		if(robotArm!=null) {
			renderLayout(robotArm);
		}
	}
	
	public void updateGUI(boolean initCase) {
		final RobotArm arm = RobotArmSettings.currentRobotArmGUI;		
		Log.d("UPDATING GUI", layoutStructs + " " + arm);
		if(layoutStructs!=null && arm!=null) {
			Log.d("NUM", "NUMLAYS:" + layoutStructs.size());
			for(final ServoLayoutStructure s : layoutStructs) {
				final int servoID = s.servoID;
				final int servoVal = arm.getJointCurrentVals().get(servoID);
				Log.d("LOZ","Redrawing servoID " + " " + servoVal);
				this.runOnUiThread(new Runnable() {
					public void run() {
//						s.seekbar.setProgress(servoVal-arm.getJointMins().get(servoID));
						s.view.setText(arm.getJointLabels().get(servoID) + " (Current Val: " + arm.getJointCurrentVals().get(servoID) + ", Suggested Val: " + arm.getJointSuggestedVals().get(servoID) + ")");
					}
				});
			}
		}
	}
	
	public void renderLayout(RobotArm arm) {
				
		//create servo layouts
		layoutStructs = Collections.synchronizedList(new ArrayList<ServoLayoutStructure>()); ;
		LinearLayout rtn = new LinearLayout(this);
		rtn.setOrientation(LinearLayout.VERTICAL);
		for(int x=0; x < arm.getJointServoIDs().length; x++) {
			ServoLayoutStructure toAdd = createServoLayout(arm, arm.getJointServoIDs()[x]);
			layoutStructs.add(toAdd);
			rtn.addView(toAdd.servoLayout);
		}
		
		//set as content view
		ScrollView v = new ScrollView(this);
		v.addView(rtn);
		setContentView(v);
		
	}
	
	public ServoLayoutStructure createServoLayout(final RobotArm arm, final int servoID) {
				
		//create top level
		final ServoLayoutStructure rtnStruct = new ServoLayoutStructure();
		LinearLayout rtn = new LinearLayout(this);
		rtn.setOrientation(LinearLayout.VERTICAL);
		rtnStruct.servoLayout = rtn;
		rtnStruct.servoID = servoID;
		
		//text label
		final TextView cbLabel = new TextView(this);
		cbLabel.setText(arm.getJointLabels().get(servoID) + " (Current Val: " + arm.getJointCurrentVals().get(servoID) + ", Suggested Val: " + arm.getJointSuggestedVals().get(servoID) + ")");				
		rtnStruct.view = cbLabel;
		LinearLayout.LayoutParams param1 = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, 0, 0.5f);
		cbLabel.setLayoutParams(param1);
		rtn.addView(cbLabel);
				
		//slider
		int range = (arm.getJointMaxes().get(servoID)-arm.getJointMins().get(servoID));
		final SeekBar seekbar = new SeekBar(this); 
		rtnStruct.seekbar = seekbar;
		seekbar.setMax(range);
		seekbar.setProgress(arm.getJointCurrentVals().get(servoID)-arm.getJointMins().get(servoID));
		seekbar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if(fromUser) {
					int suggestedVal = progress + arm.getJointMins().get(servoID);
					arm.getJointSuggestedVals().put(servoID, suggestedVal);
					cbLabel.setText(arm.getJointLabels().get(servoID) + " (Current Val: " + arm.getJointCurrentVals().get(servoID) + ", Suggested Val: " + arm.getJointSuggestedVals().get(servoID) + ")");
				}
            }
            
			//unused
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {}
			@Override
            public void onStartTrackingTouch(SeekBar seekBar) {}
            
        });
		LinearLayout.LayoutParams param2 = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, 0,0.5f);
		seekbar.setLayoutParams(param2);
		rtn.addView(seekbar);
		
		//buttons layout
		LinearLayout buttons = new LinearLayout(this);
		buttons.setOrientation(LinearLayout.HORIZONTAL);
		
		//reset button
        Button resetButton = new Button(this);
        resetButton.setText("Reset");
        resetButton.setOnClickListener(new View.OnClickListener() {
            @Override
			public void onClick(View view) {
            	arm.getJointSuggestedVals().put(servoID,arm.getJointCurrentVals().get(servoID));
        		cbLabel.setText(arm.getJointLabels().get(servoID) + " (Current Val: " + arm.getJointCurrentVals().get(servoID) + ", Suggested Val: " + arm.getJointSuggestedVals().get(servoID) + ")");				
        		seekbar.setProgress(arm.getJointCurrentVals().get(servoID)-arm.getJointMins().get(servoID));        		
            }
        });	        
		LinearLayout.LayoutParams param3 = new LinearLayout.LayoutParams(0,LayoutParams.MATCH_PARENT, 0.5f);        
		resetButton.setLayoutParams(param3);
		buttons.addView(resetButton);
        
		//set button
        Button setButton = new Button(this);
        setButton.setText("Set Servo Value");
        setButton.setOnClickListener(new View.OnClickListener() {
            @Override
			public void onClick(View view) {
            	
            	//update gui
            	/*
            	currentVal.val = suggestedVal.val;
				cbLabel.setText(label + " (Current Val: " + rtnStruct.currentVal + ", Suggested Val: " + rtnStruct.suggestedVal + ")");				
        		seekbar.setProgress(currentVal.val-min);
        		*/
            	
        		//send message to servos
        		if(BaseActivity.servoClient!=null) {
        			if(!BaseActivity.servoClient.isRunning()) {
        				BaseActivity.servoClient.startServoProcessingThread();
        			}
        			BaseActivity.servoClient.sendServoMessage(arm.getName() + ":" + arm.getJointLabels().get(servoID),arm.getJointSuggestedVals().get(servoID));
        		}
            }
        });	        
		LinearLayout.LayoutParams param4 = new LinearLayout.LayoutParams(0,LayoutParams.MATCH_PARENT, 0.5f);
		setButton.setLayoutParams(param4);
		buttons.addView(setButton);
		
		//return
		LinearLayout.LayoutParams param5 = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, 0, 0.5f);
		buttons.setLayoutParams(param5);
		rtn.addView(buttons);
		return rtnStruct;

	}
		
	class ServoLayoutStructure {
		public volatile LinearLayout servoLayout;
		public volatile int servoID;
		public volatile SeekBar seekbar;
		public volatile TextView view;
	};
}