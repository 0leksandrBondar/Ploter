
import os
from pymavlink import mavutil
import csv

source_folder = 'sourceFlyData'
output_folder = 'deserializeFlyData'

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

tlog_files = [f for f in os.listdir(source_folder) if f.endswith('.tlog')]

for tlog_file in tlog_files:
    mavlog = mavutil.mavlink_connection(os.path.join(source_folder, tlog_file))
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
            RSSI = msg.z
            time_ms = mavlog.time_since('boot') * 1e3
            selected_data.append((time_ms, x_ang, y_ang, RSSI))
        if msg.get_type() == 'GLOBAL_POSITION_INT':
            Lat = msg.lat
            Lon = msg.lon
            selected_data.append((Lat, Lon))

        if msg.get_type() == 'GPS_GLOBAL_ORIGIN ':
            gLat = msg.latitude
            gLon = msg.longitude
            gAlt = msg.altitude
            selected_data.append((gLat, gLon))
            print("gLat = " + gLat)
            print("gLon = " + gLon)
            print("gAlt = " + gAlt)

    output_csv = os.path.join(output_folder, os.path.splitext(tlog_file)[0] + '.csv')
    with open(output_csv, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(['Time', 'x_ang', 'y_ang', 'RSSI'])
        csvwriter.writerows(selected_data)

print(f"processed {len(tlog_files)} files .tlog. Files .csv saved in folder {output_folder}.")
