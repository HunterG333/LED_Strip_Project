import serial
import time

#initialize serial
ser = serial.Serial('COM5', 1200)

time.sleep(2)
ser.reset_input_buffer()



ser.write(b'G')
ser.close()