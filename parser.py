from pymavlink import mavutil

log_file = 'ok.tlog'
mavlog = mavutil.mavlink_connection(log_file)

selected_data = []

while True:
   msg = mavlog.recv_match(blocking=False)
   if msg is None:
       break

   if msg.get_type() == 'HEARTBEAT':
       is_guided = msg.custom_mode

   if msg.get_type() == 'DEBUG_VECT':
       x_ang = msg.x
       y_ang = msg.y
       RSSI  = msg.z
       time_ms = mavlog.time_since('boot') * 1e3
       selected_data.append((time_ms, x_ang, y_ang, RSSI))
       print("x_ang = "+ str(x_ang))
       print("y_ang = "+ str(y_ang))
       print("RSSI = "+ str(RSSI))



# Збереження вибраних даних у файл для подальшого аналізу в Octave
import csv

with open('selected_data.csv', 'w', newline='') as csvfile:
   csvwriter = csv.writer(csvfile)
   csvwriter.writerow(['Time', 'x_ang', 'y_ang', 'RSSI'])
   csvwriter.writerows(selected_data)
