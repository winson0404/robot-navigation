import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600)


def recvFromArduino(ser, startMarker, endMarker):
    data = []
    x = "z"  # any value that is not an end- or startMarker
    byteCount = -1  # to allow for the fact that the last increment will be one too many

    # wait for the start character
    while ord(x) != startMarker:
        x = ser.read()

    # save data until the end marker is found
    while ord(x) != endMarker:
        if ord(x) != startMarker:
            data.append(x)
            byteCount += 1
        x = ser.read()

    return (data)

def sendToArduino(ser, sendStr):
    ser.write(sendStr.encode('utf-8'))


def waitForArduino(ser):

    # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded
    
    global startMarker, endMarker
    
    msg = ""
    while msg.find("Arduino is ready") == -1:

        while ser.in_waiting() == 0:
            pass
        
        msg = recvFromArduino()

        print (msg) # python3 requires parenthesis
        print ()
        