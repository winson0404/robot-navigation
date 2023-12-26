import serial


def recvFromArduino(ser, startMarker, endMarker)->str:
    data = ""
    x = "z"  # any value that is not an end- or startMarker
    byteCount = -1  # to allow for the fact that the last increment will be one too many

    # wait for the start character
    while ord(x) != startMarker:
        x = ser.read()
        print(ord(x))
        
        if ser.in_waiting == 0:
            return data
    
    # print("Start marker found: ", ord(x))

    # save data until the end marker is found
    while ord(x) != endMarker:
        if ord(x) != startMarker:
            data+=x.decode("utf-8")
            byteCount += 1
        x = ser.read()
    print("Returning from recvFromArduino")
    return data

def sendToArduino(ser, sendStr):
    ser.write(sendStr.encode('utf-8'))
    
    #wait until send finishes
    while ser.out_waiting > 0:
        pass


def waitForArduino(ser):

    # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded
    
    global startMarker, endMarker
    
    msg = ""
    while msg.find("Arduino is ready") == -1:
        # print("Waiting for Arduino to be ready")
        while ser.in_waiting > 0: 
            # print("Available byte count: ", ser.in_waiting)   
            msg = recvFromArduino(ser, 60, 62)
            # check for empty msg
            if msg == "":
                print("Empty message received")
                continue
            print("Message: ", msg)
            break# python3 requires parenthesis
            
            
    print("Arduino is ready")
        