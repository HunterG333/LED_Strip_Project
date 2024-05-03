import serial
import time

#initialize serial
ser = serial.Serial('COM5', 1200)

time.sleep(2)
ser.reset_input_buffer()

while(True):
    print("Enter your command. EXIT to exit")
    command = input()

    if(command == "EXIT"):
        break

    ser.write(command.encode("utf-8"))

ser.close()