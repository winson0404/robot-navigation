STARTMARKER = 60 # "<"
ENDMARKER = 62  # ">"
ENDIAN = "little" # little/big
ACKNOWLEDGEMENT_SUCCESS = 0
ACKNOWLEDGEMENT_FAIL = 1
COMM_MODE_SEND = 1
COMM_MODE_RECEIVE = 0
TIMEOUT = 0.400
SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 19200

NAVIGATION_MODE_ROAMING = "ROAMING"
NAVIGATION_MODE_PREDEFINED_PATH = "PREDEFINED"

VELOCITY_PERIOD = 50 # milliseconds

MAP1 = 0
MAP2 = 1
MAP3 = 2

DESTIONATION1 = 0
DESTIONATION2 = 1

# tasks
# comms
COMMS = 1
COMMS_TASK1 = 10

# sensors
SENSOR_PACKET_LENGTH = 15
SENSOR_MODE = 2
SENSOR_DATA_REQ = 20

# motors
MOTOR = 3
MOTOR_MOVE = 30
MOTOR_TURN = 31
MOTOR_VELOCITY_REQ = 32

# Conclusions
NO_OBSTACLE = 0
SMALL_LEFT = 1
SMALL_RIGHT = 2
SMALL_LEFT_RIGHT = 3
MEDIUM_LEFT = 4
MEDIUM_RIGHT = 5
EDGE = 6
SMALL_MIDDLE = 7
FULL_OBSTACLE = 8

DECISION_MOVE_FRONT = 0
DECISION_SMALL_ROTATE_COUNTER_CLOCKWISE = 1
DECISION_SMALL_ROTATE_CLOCKWISE = 2
DECISION_MEDIUM_ROTATE_COUNTER_CLOCKWISE = 3
DECISION_MEDIUM_ROTATE_CLOCKWISE = 4
DECISION_MEDIUM_ROTATE_BOTH_DIRECTION = 5
DECISION_BIG_ROTATE_COUNTER_CLOCKWISE = 3
DECISION_BIG_ROTATE_CLOCKWISE = 4
DECISION_BIG_ROTATE_BOTH_DIRECTION = 5