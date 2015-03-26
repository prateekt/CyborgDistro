java -jar /home/pi/Desktop/CyborgDistro/RealTimeSystem/Server.jar &
cd /home/pi/Desktop/CyborgDistro/RealTimeSystem
python -m SimpleHTTPServer &
python CameraServer.py &
python UltrasonicServer.py & 

