package com.TandonRobotics.Cyborg.Activity;
import android.app.Activity;
import android.os.Bundle;

import com.TandonRobotics.Cyborg.R;
import com.TandonRobotics.Cyborg.ArmVis.ArmView;

public class MainActivity extends BaseActivity {
	
	protected void onCreate(Bundle savedInstanceState) {
		
		//set up layout
		super.onCreate(savedInstanceState);
//		setContentView(R.layout.activity_main);		
		setContentView(new ArmView(this, this));
		
	}
	
	
}