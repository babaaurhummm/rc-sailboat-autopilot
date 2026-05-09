from arduino.app_utils import *
from DATA import DATA
import time

# Initialisation de la librairie DATA avec les colonnes de la session
db = DATA(["timestamp_ms", "heading_deg", "rudder_angle_deg", "awa_deg", "unmanned_status"])

def save_data(timestamp, heading, r_angle, awa, unmanned_status):
    db.write({"timestamp_ms": timestamp, "heading_deg": heading, "rudder_angle_deg": r_angle, "awa_deg": awa, "unmanned_status": unmanned_status})

def loop():
    time.sleep(1)

Bridge.provide("save_data", save_data)
App.run(user_loop=loop)
