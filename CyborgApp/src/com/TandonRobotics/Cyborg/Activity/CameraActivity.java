package com.TandonRobotics.Cyborg.Activity;

import java.io.InputStream;
import java.net.URL;

import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.widget.ImageView;

import com.TandonRobotics.Cyborg.R;
import com.TandonRobotics.Cyborg.Clients.ClientSettingsDefault;

public class CameraActivity extends BaseActivity {
	
    //image views for different cameras
    protected ImageView cam1, cam2, cam3, cam4;
    
    //state
    protected volatile boolean running = false;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		//set up layout
		super.onCreate(savedInstanceState);
		setContentView(R.layout.camera_layout);
		
		//get components
		cam1 = (ImageView) findViewById(R.id.cam1);
		cam2 = (ImageView) findViewById(R.id.cam2);
		cam3 = (ImageView) findViewById(R.id.cam3);
		cam4 = (ImageView) findViewById(R.id.cam4);
		
		//start camera thread
		if(!running) {
			startCameraProcessingThread();
		}
	}
	
	public void startCameraProcessingThread() {
		//get server IP
		String serverIP = PreferenceManager.getDefaultSharedPreferences(getApplicationContext()).getString("SERVER_IP", ClientSettingsDefault.DEFAULT_SERVER_IP);

		//start camera threads
		startCameraProcessingThreadAgent("http://"+serverIP + ":8000/imgs/image0.jpeg",cam1);
		startCameraProcessingThreadAgent("http://"+serverIP + ":8000/imgs/image1.jpeg",cam2);
//		startCameraProcessingThreadAgent("http://"+serverIP + ":8000/imgs/image2.jpeg",cam3);
//		startCameraProcessingThreadAgent("http://"+serverIP + ":8000/imgs/image3.jpeg",cam4);
		
	}

	public void startCameraProcessingThreadAgent(final String url, final ImageView view) {
		(new Thread() {
			
			public void run() {
				running = true;
				while(running) {
					
					try {
												
						//download image and display
						DownloadImageTask t1 = (new DownloadImageTask());
						t1.setImageView(view);
						t1.execute(url);
						
						//sleep
						Thread.sleep(ClientSettingsDefault.CAMERA_SERVER_POLLING_LATENCY);
						
					}
					catch(Exception e) {
						e.printStackTrace();
					}
				}
			}
			
		}).start();
	}

	/*
	public void startCameraProcessingThread() {
		(new Thread() {
			
			public void run() {
				running = true;
				while(running) {
					
					try {
						
						//get server IP
						String serverIP = PreferenceManager.getDefaultSharedPreferences(getApplicationContext()).getString("SERVER_IP", ClientSettingsDefault.DEFAULT_SERVER_IP);
						
						//download images
						DownloadImageTask t1 = (new DownloadImageTask());
						t1.setImageView(cam1);
						t1.execute("http://"+serverIP + ":8000/imgs/image0.jpeg");
						DownloadImageTask t2 = (new DownloadImageTask());
						t2.setImageView(cam2);
						t2.execute("http://"+serverIP + ":8000/imgs/image1.jpeg");
						DownloadImageTask t3 = (new DownloadImageTask());
						t3.setImageView(cam3);
						t3.execute("http://"+serverIP + ":8000/imgs/image2.jpeg");
						DownloadImageTask t4 = (new DownloadImageTask());
						t4.setImageView(cam4);
						t4.execute("http://"+serverIP + ":8000/imgs/image3.jpeg");
						
						//sleep
						Thread.sleep(ClientSettingsDefault.CAMERA_SERVER_POLLING_LATENCY);
						
					}
					catch(Exception e) {
						e.printStackTrace();
					}
				}
			}
			
		}).start();
	}
	*/
	
	public void stopCameraProcessingThread() {
		running = false;
	}
	
	private class DownloadImageTask extends AsyncTask<String, Integer, Drawable> {
		
		protected ImageView view;
		
		public void setImageView(ImageView view) {
			this.view = view;
		}
		
		private Drawable loadImageFromNetwork(String url) {			
			try {
				InputStream is = (InputStream) new URL(url).getContent();
				Drawable d = Drawable.createFromStream(is, "src name");
				return d;
			} catch (Exception e) {
				e.printStackTrace();
				return null;
			}
		}
 
		protected Drawable doInBackground(String... urls) {
			return loadImageFromNetwork(urls[0]);
		}
 
		protected void onPostExecute(Drawable result) {
			if(result!=null && view!=null) {
				view.setImageDrawable(result);
			}
		}
	}
}