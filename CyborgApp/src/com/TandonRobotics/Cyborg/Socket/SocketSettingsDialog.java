package com.TandonRobotics.Cyborg.Socket;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;

import com.TandonRobotics.Cyborg.R;
import com.TandonRobotics.Cyborg.Activity.BaseActivity;
import com.TandonRobotics.Cyborg.Clients.ClientSettingsDefault;

public class SocketSettingsDialog extends android.app.DialogFragment {

	//parent
	protected BaseActivity parent;
	
	public SocketSettingsDialog(BaseActivity parent) {
		this.parent = parent;
	}
	
	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) {
	    AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
	    LayoutInflater inflater = getActivity().getLayoutInflater();

	    // Inflate and set the layout for the dialog
	    // Pass null as the parent view because its going in the dialog layout
	    //fill fields with default
	    View promptsView= inflater.inflate(R.layout.settings, null);
		final EditText serverIPField = (EditText) promptsView.findViewById(R.id.domainField);
		final EditText sonarPortField = (EditText) promptsView.findViewById(R.id.sonarServerPortField);
		final EditText servoPortField = (EditText) promptsView.findViewById(R.id.servoControllerPortField);
		String serverIP = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getString("SERVER_IP", ClientSettingsDefault.DEFAULT_SERVER_IP);
		int sonarPort = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getInt("SONAR_PORT", ClientSettingsDefault.DEFAULT_SONAR_SERVER_PORT);	
		int servoPort = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext()).getInt("SERVO_PORT", ClientSettingsDefault.DEFAULT_SERVO_SERVER_PORT);
		serverIPField.setText(serverIP);
		sonarPortField.setText(""+sonarPort);
		servoPortField.setText(""+servoPort);
	    builder.setView(promptsView)
	    
			    // Add action buttons
	           .setPositiveButton(R.string.open_connection, new DialogInterface.OnClickListener() {
	               @SuppressWarnings("deprecation")
					@Override
	               public void onClick(DialogInterface dialog, int id) {
	            	   
	            	   //try parsing
	            	   try {
	            		   
	            		   //parse
	            		   String serverIP = serverIPField.getText().toString();
	            		   String sonarPortStr = sonarPortField.getText().toString();
	            		   String servoPortStr = servoPortField.getText().toString();
	            		   int sonarPort = Integer.parseInt(sonarPortStr);
	            		   int servoPort = Integer.parseInt(servoPortStr);
	            		   
	            		   //set settings
		            	   SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(parent.getApplicationContext());
	                	   Editor e =sharedPrefs.edit();
	                	   e.putString("SERVER_IP", serverIP);
	                	   e.putInt("SONAR_PORT",sonarPort);
	                	   e.putInt("SERVO_PORT", servoPort);
	                	   e.commit();	           
	                	   
	                	   //restart socket thread
	                	   parent.stopSocketThreads();
	                	   Thread.sleep(3*ClientSettingsDefault.DEFAULT_SONAR_SERVER_PORT);
	                	   parent.startSocketThreads();
	            		   	            		   
	            	   }
	            	   catch(Exception e) {
	            		   
	            		   //print errors
	            		   e.printStackTrace();
	            		   
	            		   //show error dialogue
	            		   AlertDialog alertDialog = new AlertDialog.Builder(getActivity()).create();
	            		   alertDialog.setTitle("Alert Dialog");	        
	            		   alertDialog.setMessage("Error: Port must be an integer.");
	            		   alertDialog.setIcon(R.drawable.ic_launcher);	        
	            		   alertDialog.setButton("OK", new DialogInterface.OnClickListener() {
		                       public void onClick(DialogInterface dialog, int which) {	                    	   
		                       }
	            		   });	        
	            		   alertDialog.show();	            		   
	            	   }
	               }
	           })
	           .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
	               public void onClick(DialogInterface dialog, int id) {
	                   SocketSettingsDialog.this.getDialog().cancel();
	               }
	           });      
	    return builder.create();
	}	
}
