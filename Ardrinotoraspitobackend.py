import serial
import time
import firebase_admin
from firebase_admin import credentials, db

cred = credentials.Certificate('/home/pi/serviceAccountKey.json')
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://your-project-id.firebaseio.com/'
})

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=2)

while True:
    try:
        line = ser.readline().decode('utf-8').strip()
        if not line:
            continue
        parts = line.split(',')
        if len(parts) != 6:
            continue
        armed, intruder, pir, sound, ir, ldr = parts

        snapshot = {
            'armed': bool(int(armed)),
            'intruder': bool(int(intruder)),
            'pir': int(pir),
            'sound': int(sound),
            'ir': int(ir),
            'ldr': int(ldr),
            'timestamp': int(time.time())
        }
        db.reference('alarmSystem/current').set(snapshot)
        if snapshot['armed'] and snapshot['intruder']:
            db.reference('alarmSystem/alerts').push(snapshot)
        time.sleep(0.3)
    except Exception as e:
        print('Error:', e)
        time.sleep(2)
