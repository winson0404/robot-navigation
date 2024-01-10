// Include MPLAB XC16 standard libraries.
#include <math.h>
#include <stdio.h>

#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\osmain.h"

#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_UART1_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_UART2_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_UART3_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_I2C2_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Bluetooth_HC_05\Driver_HC_05_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_Audio_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_RGB_LED_V110.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_ADC_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Driver_QuadEncoder_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Motor_DC\Driver_DualDCMotor_V100.h"
//#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Smart_Servo_G15\Driver_G15_V110.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Motor_RCServo\Driver_PWMServo_V100.h"
#include "C:\Users\User\Google Drive\Projects\Embedded_Systems\C_Library\dsPIC33C\Motor_RCServo\RCServo_API_V100.h"

int nPrintln(char *, int );
void SetSoundPattern(unsigned int *, int);          // Routine to setup the audio buffer so that a string of beeps can be generated.
int  nSetMovementTurn(int);                         // Routine to set the robot to turn a certain degree.
                                                    // +ve angle is left, -ve angle is right.
//
// --- PUBLIC DEFINITIONS ---
//

// Definition for movement direction:
#define		_FORWARD				1
#define		_REVERSE				2
#define     _BACKWARD               2
#define		_STOP					0

// Standard status:
#define     _READY          1
#define     _NOT_READY      0
#define     _ERROR          -1

#define     _FP_DEFAULT_FP      0b1111100010000000  // -7.500, Q8.8 format.
#define     _FKX_DEFAULT_FP     0b1111111000000000  // -2.000, Q8.8 format.
#define     _FKV_DEFAULT_FP     0b1111111000000000  // -2.000, Q8.8 format.
#define     _FCW_DEFAULT_FP     0b1111111110101110  // -0.32032, Q8.8 format.
#define     _R_DEFAULT_FP       0b1111111111111001  // -0.027344, Q8.8 format.
#define     _MOS_DEFAULT_FP     0b0000000000000000  // 0.00, Q8.8 format.
 
#define     _FP_INTERVAL_FP     0b0000000000100000  // +0.125, Q8.8 format.
#define     _FP_INTERVAL        0.125               // +0.125, floating point format.
#define     _FKX_INTERVAL_FP    0b0000000000001000  // +0.03125, Q8.8 format.
#define     _FKX_INTERVAL       0.03125             // +0.03125, floating point format.
#define     _FKV_INTERVAL_FP    0b0000000000001000  // +0.03125, Q8.8 format.
#define     _FKV_INTERVAL       0.03125             // +0.03125, floating point format.
#define     _FCW_INTERVAL_FP    0b0000000000000010  // +0.078125, Q8.8 format.
#define     _FCW_INTERVAL       0.0078125           // +0.078125, floating point format.
#define     _R_INTERVAL_FP      0b0000000000000001  // +0.00390625, Q8.8 format.
#define     _R_INTERVAL         0.00390625          // +0.0078125, floating point format.
#define     _MOS_INTERVAL_FP    0b0000000100000000  // +1, Q8.8 format.
#define     _MOS_INTERVAL       1  

#define     _MOTOR_ENCODER_CPR          48          // Motor encoder continous pulse per rotation
#define     _MOTOR_QUAD_DIVIDE          8           // Quadrature encoder divider ratio. 
#define     _WHEEL_PITCH_MM             194         // Wheel pitch.
#define     _WHEEL_DIAM_MM              100         // Wheel diamter.
#define     _MOTOR_GEAR_RATIO           75          // Motor gear ratio.   

#define     _NOTICKSROTATION            400         // This is no. of ticks per rotation of the wheel, (_MOTOR_ENCODER_CPR*_MOTOR_GEAR_RATIO)/_MOTOR_QUAD_DIVIDE  
#define     _TURN_DEG_PER_100           26          // _TURN_DEG_PER_UNIT * 100 (if heading change by 100 units, the
                                                        // actual change in degree is 26 degrees)

#define     _VELOCITY_MOVE_VERYSLOW     0.20        // In rotation per second.
#define     _VELOCITY_MOVE_SLOW         0.30
#define     _VELOCITY_MOVE_NORMAL       0.45
#define     _VELOCITY_MOVE_FAST         0.60 
#define     _VELOCITY_MOVE_VERYFAST     0.75

int       progData[5]   __attribute__((space(prog), address(0x010000))) = {_FP_DEFAULT_FP , _FKX_DEFAULT_FP, _FKV_DEFAULT_FP, _FCW_DEFAULT_FP, _R_DEFAULT_FP};

// Unsigned integers to store global system coefficients in Q8.8 fixed point format.   
unsigned int    gunProgDataKP_Q8_8;                 // System coefficients.
unsigned int    gunProgDataKX_Q8_8;
unsigned int    gunProgDataKV_Q8_8;
unsigned int    gunProgDataKWg_Q8_8;
unsigned int    gunProgDataR_Q8_8;

unsigned int    gunProgDataKP_Ori_Q8_8;             // Backup of system coefficients.
unsigned int    gunProgDataKX_Ori_Q8_8;
unsigned int    gunProgDataKV_Ori_Q8_8;
unsigned int    gunProgDataKWg_Ori_Q8_8;
unsigned int    gunProgDataR_Ori_Q8_8;

// --- ROBOT PARAMETERS ---
// --- Primary Control Loop Coefficients: Balancing Feedback Control Parameters ---
int   	gnC;                            // Control variable for motor driver, general.
int     gnCL;                           // Control variable for motor driver, left wheel (after adding offset).
int     gnCR;                           // Control variable for motor driver, right wheel (after adding offset).
                                        // Coefficients for state-space feedback control.
float 	gfR = 0.00;                     // Reference tilt angle in radian.
float   gfR_Ori = 0.00;                 // Backup for gfR.
float	gfP = 0.00;                     // Coefficient for tilt angle.
float   gfP_Ori = 0.00;                 // Backup for gfP.
float	gfKnw = 0.00;                   // Coefficient for linear distance traveled.
float   gfKX_Ori = 0.00;                // Backup for gfKX.  Note that gfKX and gfKnw are related by just a 
                                        // constant multiplier.
float	gfKomega = 0.00;                // Coefficient for linear velocity.
float   gfKV_Ori = 0.00;                // Backup for gfKV.
float 	gfCwg = 0.00;                   // Coefficient for time derivative of tilt angle.
float   gfCwg_Ori = 0.00;               // Backup for gfCWg.
int 	gnMotOffset_mV = 0;             // The motor driver circuit voltage offset, in mV.
                                        // This parameter is to account for dead-band in the motor driver
                                        // circuit and the motor/gearbox itself.  It is applied to both 
                                        // left and right motors.

float   gfER = 0.0;                     // Error term for tilt angle. i.e. (gfR + gfRoffset) - gfTheta_IMU;
float   gfEnw = 0.0;                    // Error term for wheen angle in integer.
float   gfEOmegaw = 0.0;                // Error term for wheel rotation speed.     

// --- Secondary Control Loop Coefficients: Turning, Linear movement.
int     gnKp_Turn = 0;                  // Coefficients for turning (heading mode) PD control loop.
int     gnKd_Turn = 0;
int     gnKi_Turn = 0;
int     gnKp_Move = 0;                  // Coefficients for linear movement PD control loop.
int     gnKd_Move = 0;
//int     gnKp_TurnCont = 0;              // Coefficients for turning (turn speed mode) PI control loop.
//int     gnKi_TurnCont = 0;

#define     _FP2_TURN_DEFAULT           10
#define     _FD2_TURN_DEFAULT           0
#define     _FI2_TURN_DEFAULT           1

// --- Tilt Angle and Orientation Parameters as measured by IMU ---
float   gfWg_MPU6050;                   // Instantaneous tilt angular velocity from MPU6050 gyroscope in radian/sec.
float   gfTheta_MPU6050;                // Robot's tilt angle in radian, from MPU6050, in radian.
int     gnTheta_Deg_MPU6050;            // Robot's tilt angle in degree, from MPU6050, in degree.
int     gnStatus_IMU = _NOT_READY;      // IMU status.  It is recommended to check this status
                                        // upon power up to make sure the IMU is READY, otherwise
                                        // the outputs from the IMU are not valid.

#define     _ROR_UPRIGHT        0       // Upright (within +- 3 deg of normal, i.e. 0 degree).
#define     _ROR_LEAN_FRONT     2       // Lean to the front (+4 to +29 degrees).
#define     _ROR_LEAN_BACK      -2      // Lean to the back (-4 to -29 degrees).
#define     _ROR_TOPPLE_FRONT   5       // Toppled to the front (> 29 degrees).
#define     _ROR_TOPPLE_BACK    -5      // Toppled to the back (< -29 degrees).
int     gnTiltOrient_IMU = _ROR_TOPPLE_BACK;          // Robot tilt orientation

typedef struct StructRobotState
{
    unsigned int unTemperature_C;
    unsigned int unBatteryLevel_mV;
    unsigned int unSensorRangeR_mm;     // Variable to store the approximate distance in mm
                                        // of any object detected by range sensors 
    unsigned int unSensorRangeL_mm;     // R = right, L = left, F = front, B = back.
    unsigned int unSensorRangeF_mm;
    unsigned int unSensorRangeB_mm;        
    unsigned int unMode;
} ROBOTSTATE;

ROBOTSTATE      gobjRobotState;

int             gnRCServoMotorAngleSet_Deg[8] = {0,0,0,0,0,0,0,0}; // Integer array to store the RC servo motor angle setting in degrees.
INT32   gnlDistanceSet = 0;             // Robot linear traveled distance setting in no. of ticks from quadrature encoder.
                                        // > 0 move forward.
                                        // < 0 move backward.
INT16   gnHeadingSet = 0;               // Robot heading (direction) setting.
                                        // 0 = Look straight ahead
                                        // > 0 turn left.
                                        // < 0 turn right.
float   gfOmegaWSet = 0.0;              // Robot wheel rotation velocity setting in rotation/sec.
                                        // > 0.0 move forward.
                                        // < 0.0 move backward.
INT16   gnTurnSpeedSet = 0;
//
// --- PRIVATE VARIABLES ---
//

// HC-05 Bluetooth wireless module command handler.
// The command packet from the Remote Host is interpreted here.  Essentially there
// are three types of command: device reset, restore parameters and set parameter.
// This function returns the next state for the state machine in the Driver_HC_05.

// --- Wireless link commands ---
#define     _SPP_ESTABLISH_LINK         0x10
#define     _SPP_LINK_ESTABLISHED       0x11
#define     _TUNABLE_COEFFICIENT_INFO   0x12
#define     _COEFFICENT_INFO            0x13
#define     _DEVICE_RESET               0x00
#define     _RES_PARAM                  0x01
#define     _SET_PARAM                  0x02

char            *gptrSystemText;                    // Pointer to string, for system text message to remote PC/host.
int             gnSystemTextLength;                 // No. of characters in system text message.

unsigned char   gbytCeoffKp[3] = {'K', 'P', ' '};                   // 3 characters name for each tunable coefficient.
unsigned char   gbytCoeffKx[3] = {'K', 'x', ' '}; 
unsigned char   gbytCoeffKv[3] = {'K', 'v', ' '};
unsigned char   gbytCoeffKw[3] = {'K', 'w', ' '};
unsigned char   gbytCoeffRs[3] = {'R', 's', 't'};
unsigned char   gbytCoeffMo[3] = {'K', 'p', 't'};
unsigned char   gbytCoeffKp2[3] = {'K', 'd', 't'};
unsigned char   gbytCoeffKd2[3] = {'K', 'i', 't'};
unsigned char   gbytCoeffKi2[3] = {'W', 'r', 'i'};
unsigned char   gbytCoeffEle[3] = {'E', 'l', 'e'};

// Sound vocabulary
// This series of array store various sound patterns to be used by the robot to signal different scenarios.
const unsigned int  gunSoundConnectedPC[] = {1,2,3,4,5};    // Connected to remote computer.
const unsigned int  gunSoundConnectedSP[] = {1,2,3,4,5};    // COnnected to remote smart phone or tablet.
const unsigned int  gunSoundDisconnected[] = {7,5,6,4,5,3}; // All remote devices disconnected.
const unsigned int  gunSoundSmartServoConnected[] = {2,7}; // All remote devices disconnected.
const unsigned int  gunSoundUpright[] = {3,8};
const unsigned int  gunSoundToppled[] = {9,4};
const unsigned int  gunSoundBatteryLow[] = {2,5,2,7};
const unsigned int  gunSoundIMUAbsent[] = {2,9,5,6};
const unsigned int  gunSoundSmartServoAbsent[] = {2,9,5,3};
const unsigned int  gunSoundSmartServoOverload[] = {2,3,7,4};
const unsigned int  gunSoundSmartServoReady[] = {3,2,10,8,6,5};
const unsigned int  gunSoundExcited[] = {1,3,5,7,9,4};
const unsigned int  gunSoundIdle[] = {2,5,4};

typedef struct	StructCoeffTuneDef
{
	unsigned int    unDefault;          // Default value in Q8.8 fixed point format.
    unsigned int    unInterval;         // Interval in Q8.8 fixed point format.
    float           fDefault;           // Default value in floating point format.
    unsigned char   *pbytDes;           // Pointer to a string.
}	COEFFTUNEDEF;

COEFFTUNEDEF    objCoeffDef[10];        // Objects to store tunable global parameters.

// Function to initialize a stream of tones in the Audio driver.
// Arguments:
// *unSoundArray = pointer to an unsigned integer array containing a string of tones to play.
// nCount = Indicate the length of the integer array.
void SetSoundPattern(unsigned int *unSoundArray, int nCount)
{
    int nIndex = 0;
    
    if (nCount > __DRIVER_AUDIO_MAX_TONE_INDEX)     // Limit to the maximum number of tones
    {                                               // supported by the audio driver.
        nCount = __DRIVER_AUDIO_MAX_TONE_INDEX;
    }
    if (nCount > 0)
    {
        while(nCount > 0)
        {
            gnAudioTone[nIndex] = unSoundArray[nIndex];
            nCount--;
            nIndex++;
        }
        gnAudioTone[nIndex] = 0;    // End the string of beep with a 0.
    }
}


///
/// Function name	: SetMovementTurn
///
/// Author          : Fabian Kung
///
/// Last modified	: 30 June 2018
///
/// Code version	: 0.93
///
/// Processor		: Generic
///
/// Description		: Turn the robot.
///
/// Arguments		: Turn degree in integer format:
///                   1 < Argument <= 359 to turn left, 
///                   -359 <= Argument < -1 to turn right.
///                   >= 360 the turning angle will be set to 359.
///                   <= -360 the turning angle will be set to -359.
///                   0, stop turning.
///                  
/// Return          : Offset to gnHeadingSet.
///
/// Global variable	: gnHeadingSet
///                   gnTurnSpeedSet
///                   gnHeading
///
/// Description     : Call this function to turn the robot left or right by a certain
///                   degree or continuously.  
/// 
int nSetMovementTurn(int nAngle)
{
    int nTemp;
    
    if (nAngle > 0)                                 // Turn left, finite or continuous.
    {
        if (nAngle > 360)                           // 0 < nAngle < 360, turn left.
        {  
            nAngle = 359;                           // Limit maximum angle to 359 degrees.
        }
        nAngle = nAngle * 100;                      // Scale the angle.
        nTemp = nAngle/_TURN_DEG_PER_100;           // Integer division faster than floating point division.
        gnHeadingSet = gnHeadingSet + nTemp;  

    }
    else if (nAngle < 0)                            // Turn right, finite or continuous.
    {
        if (nAngle < -360)                          // -360 < nAngle < 0, turn right.
        {
            nAngle = -359;                          // Limit minimum angle to -359 degrees.
        }
        nAngle = nAngle * 100;                      // Scale the angle.
        nTemp = nAngle/_TURN_DEG_PER_100;           // Integer division faster than floating point division.
        gnHeadingSet = gnHeadingSet + nTemp;      
    }  
    else                                            // nAngle = 0.
    {
        gnTurnSpeedSet = 0;                         // Disable constant turning speed mode.
 //       gnTurnMode =  _TURN_MODE_NONE;              // Disable turning mode (if enable).                   // Disable continuous turning (if enable).    
        gnHeadingSet = gobjQEOut.nHeading;                   // Update heading setting to current direction, disable
                                                    // heading mode.
        nTemp = 0;
    }
    return nTemp;
}

// Callback function for HC-05 Bluetooth module
// Argument:        None
// Return:          The next state in the HC-05 driver state machine
 int HC05_CommHandler()
{
    int nTemp, nTemp2;
     
    if (gobjDriverHC05.bytRFCommand == _DEVICE_RESET)       // Check if the host request
                                                            // for client to reset.
    {
        gobjDriverHC05.nRFLinkState = 0;                    // Reset RF link established.
        gobjDriverHC05.nEnRFTxPeriodicData = 0;             // Disable periodic transmission of data to remote host.
        return 10;                                          // Return the RF SPP link process.
    }
    else if (gobjDriverHC05.bytRFCommand == _RES_PARAM)     // Restore parameter sequence.
    {
        return 21;                                          // Return to message clearing loop.
    }
    else if (gobjDriverHC05.bytRFCommand == _SET_PARAM)     // Set parameters routines.
    {
        gnAudioTone[0] = 3;                     // Beep.
        gnAudioTone[1] = 0;

        nTemp = gobjDriverHC05.bytRFArgument1;              // Get parameter index.
        nTemp2 = gobjDriverHC05.bytRFArgument2 - 90;        // Get the change in parameter from the initial or start value.
                                                            // This value ranges from 0-180, with initial value at 90.
        if (nTemp == 0)                         // First parameter.
        {
            gnKp_Turn = _FP2_TURN_DEFAULT + nTemp2;
        }
        else if (nTemp == 1)                    // Second parameter.
        {
            gnKd_Turn = _FD2_TURN_DEFAULT + nTemp2;
        }
        else if (nTemp == 2)                    // Third parameter.
        {
            gnKi_Turn = _FI2_TURN_DEFAULT + nTemp2;  
        }
        else if (nTemp == 3)                    // Fourth parameter.
        {
            
        }
        else if (nTemp == 4)                    // Fifth parameter.
        {
            
        }
        else if (nTemp == 5)
        {
            
        }
        else if (nTemp == 6)
        {           
              
        }
        else if (nTemp == 7)
        {
            
        }
        else if (nTemp == 8)
        {
            gnRCServoMotorAngleSet_Deg[0] = 0 + nTemp2;
            gnRCServoMotorAngleSet_Deg[1] = 0 + nTemp2;            
        }
        else if (nTemp == 9)
        {   
            gnRCServoMotorAngleSet_Deg[2] = 0 + nTemp2;
            gnRCServoMotorAngleSet_Deg[3] = 0 + nTemp2;       
        }
        else if (nTemp == 10)
        {
                     
        }
        else if (nTemp == 11)
        { 
                        
        }
        else if (nTemp == 12)
        {
                             
        }
        else if (nTemp == 13)
        {
            
        }
        else if (nTemp == 14)
        {
            
        }        
        return 21;                          // Return to message clearing loop.
    }
    else    // Unhandled commands.
    {
        return 21;                          // Return to message clearing loop.
    }
}

// Callback function for HC-05 Bluetooth module.
// HC-05 Bluetooth wireless module ASCII data handler.
// This callback function handles user command from Bluetooth client.
 void HC05_ASCIIHandler()
{  
    BYTE   bytTemp;
    
    bytTemp = gobjDriverHC05.bytRFCommand;
    
    if (bytTemp != '@')
    {
        switch (bytTemp)
        {
            case 'x': // Stop.
                gnAudioTone[0] = 1;
                gnAudioTone[1] = 0;   
                gfOmegaWSet = 0.0;     
                nSetMovementTurn(0);
                break;

            case 'f': // Move forward fast.
                gnAudioTone[0] = 2;
                gnAudioTone[1] = 0;          
                gfOmegaWSet = _VELOCITY_MOVE_FAST;   
                break;

            case 'F': // Move forward at normal speed.  
                gnAudioTone[0] = 3;
                gnAudioTone[1] = 0;                     
                gfOmegaWSet = _VELOCITY_MOVE_NORMAL;          
                break;
                
            case 's': // Move forward at slow speed.
                gnAudioTone[0] = 4;
                gnAudioTone[1] = 0;               
                gfOmegaWSet = _VELOCITY_MOVE_SLOW;         
                break;     
                
            case 't': // Move backward normal.
                gnAudioTone[0] = 5;
                gnAudioTone[1] = 0;         
                gfOmegaWSet = -_VELOCITY_MOVE_NORMAL;                
                break;
                
            case 'l': // Turn left a bit.
                gnAudioTone[0] = 6;
                gnAudioTone[1] = 0;
                nSetMovementTurn(15);
                //nSetMovementTurn(10);
                //gnTurnMode = _TURN_MODE_FIXED_HEADING;                
                break;
                
            case 'r': // Turn right a bit.
                gnAudioTone[0] = 7;
                gnAudioTone[1] = 0;
                nSetMovementTurn(-15);
                //nSetMovementTurn(-10);
                //gnTurnMode = _TURN_MODE_FIXED_HEADING;                
                break;
                
            case 'c': // Continuous turn left.
                      // Sending the character 'c' will toggle the continuous turning
                      // on or off.
                gnAudioTone[0] = 6;
                gnAudioTone[1] = 8;
                gnAudioTone[2] = 6;
                gnAudioTone[3] = 0;        
                if (gnTurnSpeedSet == 0)
                {    
                    //gnTurnSpeedSet = 2;         // Enable continuous turning, and constant turning speed
                                                // regular turning speed.
                    gnTurnSpeedSet = 3;         // Enable continuous turning, and constant turning speed, 
                                                // higher speed.  
                    //gnTurnMode = _TURN_MODE_CONT_TURN;
                }
                else
                {
                    nSetMovementTurn(0);        // Stop all turning modes.
                }     
                break;                

            case 'C': // Continuous turn right.
                      // Sending the character 'C' will toggle the continuous turning
                      // on or off.
                gnAudioTone[0] = 7;
                gnAudioTone[1] = 5;
                gnAudioTone[2] = 7;
                gnAudioTone[3] = 0;  
                if (gnTurnSpeedSet == 0)
                {    
                    //gnTurnSpeedSet = -2;        // Enable continuous turning, and constant turning speed.   
                    gnTurnSpeedSet = -3;            // Higher turning speed.
                    //gnTurnMode = _TURN_MODE_CONT_TURN;
                }
                else
                {
                    nSetMovementTurn(0);        // Stop all turning modes.
                }       
                break;                
                
/*            case 'S': // Store coefficients.
                
                    gnAudioTone[0] = 13;
                    gnAudioTone[1] = 7;
                    gnAudioTone[2] = 0;       
                    OSEnterCritical();                      // Disable all processor interrupts.
                    T1CONbits.TON = 0;                      // Turn off Timer1.  Timer1 powers the
                                                            // RTOS scheduler.
                    WDTCONLbits.ON = 0;         // Turn off watchdog timer (WDT). 
                    
                    // Read 1 page of flash memory location into RAM buffer.
                    ProgMemReadPage(__builtin_tblpage(progData),__builtin_tbloffset(progData));
                    gnRamBuffer[0] = gunProgDataKP_Q8_8;    // Update 1st system variable.
                    gnRamBuffer[1] = 0;                     // Only lower 16-bits are used.
                    gnRamBuffer[2] = gunProgDataKX_Q8_8;    // Update 2nd system variable.
                    gnRamBuffer[3] = 0;                     // Only lower 16-bits are used.
                    gnRamBuffer[4] = gunProgDataKV_Q8_8;    // Update 3rd system variable.
                    gnRamBuffer[5] = 0;                     // Only lower 16-bits are used.
                    gnRamBuffer[6] = gunProgDataKWg_Q8_8;   // Update 4th system variable.
                    gnRamBuffer[7] = 0;                     // Only lower 16-bits are used.
                    gnRamBuffer[8] = gunProgDataR_Q8_8;     // Update 5th system variable.
                    gnRamBuffer[9] = 0;                     // Only lower 16-bits are used.                
                                                            // Erase 1 page of flash memory which
                                                            // encompass the system variables.
                    ProgMemErasePage(__builtin_tblpage(progData),__builtin_tbloffset(progData));                       
                                                            // Program new coefficient values into Flash memory using double words write
                                                            // approach.
                    
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),0);   // Since 1 page consists of 8 rows, 
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),1);   // we need to execute flash write routine
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),2);   // 8 times to update all the flash memory
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),3);   // location contain in 1 page.
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),4);
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),5);
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),6);
                    //ProgMemRowWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),7);
                    
                    // Note: 16 July 2020, somehow the row write approach did not work in dsPIC33CKXXX
                    // processors.  Still working on it. Confirm that it is the processor issue, not the
                    // codes in the row write routine.  The codes below are just tentative solution.
                    ProgMemDoubleWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData),0,gunProgDataKP_Q8_8, 0,gunProgDataKX_Q8_8);
                    ProgMemDoubleWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData)+4,0,gunProgDataKV_Q8_8, 0,gunProgDataKWg_Q8_8);
                    ProgMemDoubleWrite(__builtin_tblpage(progData),__builtin_tbloffset(progData)+8,0,gunProgDataR_Q8_8, 0,0);               
                    
                    OSExitCritical();                       // Enable processor interrupts. 
                    T1CONbits.TON = 1;                      // Turn on Timer1 again.
                    WDTCONLbits.ON = 1;         // Turn on watchdog timer (WDT). 
 
                break;
*/
                
            case 'R': // Restore coefficients to factory default.

                break;
                
            default:
                break;
        } // switch (bytTemp)
    } 
    else
    {
        nPrintln("Request",7);
    }
	gobjDriverHC05.bytRFCommand = 0;                      // Reset command byte.
}

// Callback function for HC-05 Bluetooth module
// HC-05 Bluetooth wireless module binary data handler.
void HC05_BinaryHandler()
{

}

// Callback function for HC-05 Bluetooth module
// 
// Module: UART3
// 
// The periodic data packet is 64 bytes long.
// Byte 0 = ID of machine.
// Byte 1 = Identification of data packet, binary data in this case.
// Byte 2 = 8-bits positive integer nData8bit1.
// Byte 3 = 8-bits positive integer nData8bit2.
// Byte 4 = 8-bits positive integer nData8bit3.
// Byte 5 = 8-bits positive integer nData8bit4.
// Byte 6 = 8-bits positive integer nData8bit5.
// Byte 7 = 8-bits positive integer nData8bit6.
// Byte 8 = 8-bits positive integer nData8bit7.
// Byte 9 = 8-bits positive integer nData8bit8.
// Byte 10 = 8-bits positive integer nData8bit9.
// Byte 11 = 8-bits positive integer nData8bit10.
// Byte 12 = 8-bits positive integer nData8bit11.
// Byte 13 = 8-bits positive integer nData8bit12.
// Byte 14-15 = 16 bits positive integer data1.
// Byte 16-17 = 16 bits positive integer data2.
// Byte 18-19 = 16 bits positive integer data3.
// Byte 20-21 = 16 bits positive integer data4.
// Byte 22-63 = Miscellaneous data.  
//
// 31 Dec 2018:  Our assignment for the data payload as follows:
// nData8bit1 = tilt angle + 127 (127 being the offset as tilt angle ranges from -127 to + 127)
// nData8bit2 = 50x(Right wheel velocity) + 127 (actual value of velocity is small, so need to scale
// up)
// nData8bit3 = 50x(Left wheel velocity) + 127
// nData8bit4 = 50x(Average wheel velocity) + 127
// nData8bit5 = ((Right wheel count) / 4) + 127
// nData8bit6 = ((Left wheel count) / 4) + 127
// nData8bit7 = (Average wheel count / 4) + 127
// nData8bit8 = (Heading / 2) + 127
// nData8bit9 = (Set tilt angle) + 127
// nData8bit10 = 50x(Set average wheel velocity) + 127
// nData8bit11 = ((Set average wheel count) / 2) + 127
// nData8bit12 = (Set heading / 2) + 127

void HC05_SendPeriodData()
{
   unsigned int unTemp, unTemp2, unTemp3, unTemp4;
   int nData8bit1, nData8bit2, nData8bit3, nData8bit4;
   int nData8bit5, nData8bit6, nData8bit7, nData8bit8;
   int nData8bit9, nData8bit10, nData8bit11, nData8bit12;
   int nIndex;
   
    if (gobjDriverHC05.nEnRFTxPeriodicData == _DRIVER_HC_05_TX_BINARYDATA)      // Check if to transmit binary or ASCII data. 
    {   
        // 8-bits unsigned data packet
        nData8bit1 = gnTheta_Deg_MPU6050 + 127;;
        nData8bit2 = gobjQEOut.fOmegaRW * 20;
        nData8bit2 = nData8bit2 + 127;
        nData8bit3 = gobjQEOut.fOmegaLW * 20;
        nData8bit3 = nData8bit3 + 127;
        nData8bit4 = gobjQEOut.fOmegaW * 20; 
        nData8bit4 = nData8bit4 + 127;    
        nData8bit5 = (gobjQEOut.nDistanceMoveRW>>3) + 127;   // Distance in ticks divided by 8.
        nData8bit6 = (gobjQEOut.nDistanceMoveLW>>3) + 127;       
        nData8bit7 = (gobjQEOut.nDistanceMoveW>>3) + 127;  
        nData8bit8 = (gobjQEOut.nHeading>>1) + 127;
        nData8bit9 = 127;     
        nData8bit10 = gfOmegaWSet * 20;             // Wheel angular velocity set in rotation/sec.
        nData8bit10 = nData8bit10 + 127; 
        nData8bit11 = (gnlDistanceSet>>3) + 127;    // Distance set in ticks divided by 8.      
        nData8bit12 = (gnHeadingSet>>1) + 127;      // Heading set in differential ticks. 
        
        // 16-bits unsigned data packet.
        unTemp4 = 0 + 4095;       // Add positive offset to make the data unsigned integer.    
        //unTemp = gnKi_Turn*100 + 4095;
        unTemp = gobjDriverADC.unADC1_mV + 4095;
        unTemp2 = gobjDriverADC.unADC2_mV + 4095; 
        unTemp3 = gobjDriverADC.unADC3_mV + 4095;

        // Pack data into transmit buffers.
        gbytTXbuffer2[2] = nData8bit1;
        gbytTXbuffer2[3] = nData8bit2;
        gbytTXbuffer2[4] = nData8bit3;
        gbytTXbuffer2[5] = nData8bit4;
        gbytTXbuffer2[6] = nData8bit5;
        gbytTXbuffer2[7] = nData8bit6;
        gbytTXbuffer2[8] = nData8bit7;
        gbytTXbuffer2[9] = nData8bit8;
        gbytTXbuffer2[10] = nData8bit9;  
        gbytTXbuffer2[11] = nData8bit10;
        gbytTXbuffer2[12] = nData8bit11;
        gbytTXbuffer2[13] = nData8bit12;        
        gbytTXbuffer2[14] = unTemp >> 8;           // Send upper 8-bits.
        gbytTXbuffer2[15] = unTemp;                // Send lower 8-bits.
        gbytTXbuffer2[16] = unTemp2 >> 8;          // Send upper 8-bits.
        gbytTXbuffer2[17] = unTemp2;               // Send lower 8-bits.
        gbytTXbuffer2[18] = unTemp3 >> 8;          // Send upper 8-bits.
        gbytTXbuffer2[19] = unTemp3;               // Send lower 8-bits.  
        gbytTXbuffer2[20] = unTemp4 >> 8;          // Send upper 8-bits.
        gbytTXbuffer2[21] = unTemp4;               // Send lower 8-bits.
        
   }
   else if (gobjDriverHC05.nEnRFTxPeriodicData == _DRIVER_HC_05_TX_TEXTDATA)  // Transmit ASCII data packet. 
   {
        if (gnSystemTextLength > 50)
        {
            gnSystemTextLength = 50;        // Limit text length to 50 characters per line.
        }
        for (nIndex = 0; nIndex < gnSystemTextLength; nIndex++ )
        {
            gbytTXbuffer2[nIndex + 2] = *(gptrSystemText + nIndex);
        }
        gbytTXbuffer2[nIndex + 2] = 0xD;     // Append the newline character.        
        gobjDriverHC05.nEnRFTxPeriodicData = _DRIVER_HC_05_TX_BINARYDATA;   // Set the system back to default binary packet transmission.
   }
}

///
/// Function name	: nPrintln
///
/// Author          : Fabian Kung
///
/// Last modified	: 6 Sep 2016
///
/// Code version	: 0.90
///
/// Processor		: Generic
///
/// Description:
/// 1. Prints data to the remote host via wireless link as human-readable ASCII text
/// followed by a carriage return character (ASCII 13, or '\r') and a newline 
/// character (ASCII 10, or '\n').
/// 2. Requires that a wireless link using serial port protocol has been formed with a 
/// remote PC host (or similar class of devices).
/// 3. Maximum string length is 50 characters excluding the newline character.
///
/// Arguments:
/// ptrText = Address of character string.
/// nTextLength = Length of text string exclusive of the newline character (0xD).
/// Return:
/// The number of bytes send.
/// Usage example:
/// nPrintln("Hello",5);
///
/// or
/// char *ptrChar;
///
/// ptrChar = "Hello";
/// nPrintln(ptrChar,5);

int nPrintln(char *ptrText, int nTextLength)
{
    gptrSystemText = ptrText;                       // Assign string address and length.
    gnSystemTextLength = nTextLength;
    if (gobjDriverHC05.nRFLinkState == 1)           // Make sure a PC type remote device is connected
    {                                               // via wireless link.
        gobjDriverHC05.nEnRFTxPeriodicData = 2;     // Enable periodic data transmission, set ASCII packet.
        return nTextLength;
    }   
    else
    {
        return 0;
    }
} 


///
/// Process name		: User_Proce1
///
/// Author              : Fabian Kung
///
/// Last modified		: 20 Aug 2022
///
/// Code Version		: 0.841
///

/// Processor           : dsPIC33EP256MU80X family.
///
/// Processor/System Resources
/// PINS                : 
///
/// MODULES             : UART2
///
/// RTOS                : Ver 1 or above, round-robin scheduling.
///
/// Global variables	: 

#ifdef 				  __OS_VER			// Check RTOS version compatibility.
	#if 			  __OS_VER < 1
		#error "User_Proce1: Incompatible OS version"
	#endif
#else
	#error "User_Proce1: An RTOS is required with this function"
#endif

/// Description:
/// This process performs two important functions:
/// 1. It loads important global constants of the robot stored in the Program Memory into global
/// variables residing in Data Memory.
/// 2. It initiate connection to the a remote host via RF wireless link or wired link.  The 
/// global constants is send to the remote host to enable the user to adjust these constants if
/// needed.  Another process, "HC05_CommHandler( )" is tasked with capture commands from the user
/// and tune the global constant values.
/// The remote host should be a PC or Tablet computer or similar devices with 
/// high computing power and large display.  The wireless transceiver can be Bluetooth or other custom
/// devices.  The process polls the global variable gnRFLinkState until RF link is established by
/// the RF driver (Please refer to the header file for the respective RF link driver for further
/// details).  After the RF link is established, this process will transmit setup
/// data and robot physical parameters to the remote host so that the remote host knows the 
/// default values of the feedback control and physical parameters of the robot.  
/// This allows a user on the remote host to monitor the robot instantaneous state and tune 
/// the robot feedback control algorithm if necessary.
/// Note that the data transmitted includes Q8.8 fixed point format and unsigned integers.


void User_Proce1(TASK_ATTRIBUTE *ptrTask)
{
    static unsigned int unTemp;
    static int nCount = 0;
        
    if (ptrTask->nTimer == 0)
    {
        switch (ptrTask->nState)
        {
            case 0: // State 0 - Initialization.
                gnEnHC05Init = 0;                       // Disable initialization of HC-05 Bluetooth module.
                //gnEnHC05Init = 1;                    // Enable initialization of HC-05 Bluettoth module.
                OSSetTaskContext(ptrTask, 5, 100*__NUM_SYSTEMTICK_MSEC);    // Next state = 1, timer = 100 msec.
            break;
                         
            case 5: // State 5 - Initialize the tuning coefficient structures.
                objCoeffDef[0].fDefault = 0.0; // Initialize description of tunable parameter 0.
                objCoeffDef[0].unInterval = 0b0000000100000000;
                objCoeffDef[0].unDefault = 0b0000000000000000;
                objCoeffDef[0].pbytDes = gbytCeoffKp;    

                objCoeffDef[1].fDefault = 0.0; // Initialize description of tunable parameter 1.
                objCoeffDef[1].unInterval = 0b0000000100000000;
                objCoeffDef[1].unDefault = 0b0000000000000000;
                objCoeffDef[1].pbytDes = gbytCoeffKx;

                objCoeffDef[2].fDefault = 0.0; // Initialize description of tunable parameter 2.
                objCoeffDef[2].unInterval = 0b0000000100000000;
                objCoeffDef[2].unDefault = 0b0000000000000000;
                objCoeffDef[2].pbytDes = gbytCoeffKv;

                objCoeffDef[3].fDefault = 0.0; // Initialize description of tunable parameter 3.
                objCoeffDef[3].unInterval = 0b0000000100000000;
                objCoeffDef[3].unDefault = 0b0000000000000000;
                objCoeffDef[3].pbytDes = gbytCoeffKw; 
                
                objCoeffDef[4].fDefault = 0.0; // Initialize description of tunable parameter 4.
                objCoeffDef[4].unInterval = 0b0000000100000000;
                objCoeffDef[4].unDefault = 0b0000000000000000;
                objCoeffDef[4].pbytDes = gbytCoeffRs; 
                
                OSSetTaskContext(ptrTask, 6, 1);        // Next state = 6, timer = 1.
                break;
                
            case 6: // State 6 - Initialize the rest of the tuning coefficient structures.                                    
                objCoeffDef[5].fDefault = 0.0; // Initialize description of motor offset.
                objCoeffDef[5].unInterval = 0b0000000100000000;
                objCoeffDef[5].unDefault = 0b0000000000000000;
                objCoeffDef[5].pbytDes = gbytCoeffMo;    
 
                objCoeffDef[6].fDefault = 0.0;                      // Initialize description of coefficient 7.
                objCoeffDef[6].unInterval = 0b0000000100000000;     // Interval = 1.0
                objCoeffDef[6].unDefault = 0b0000000000000000;
                objCoeffDef[6].pbytDes = gbytCoeffKp2;    
                
                objCoeffDef[7].fDefault = 0.0;                      // Initialize description of coefficient 8.
                objCoeffDef[7].unInterval = 0b0000000100000000;     // Interval = 1.0
                objCoeffDef[7].unDefault = 0b0000000000000000;
                objCoeffDef[7].pbytDes = gbytCoeffKd2;
                
                objCoeffDef[8].fDefault = 0.0;                      // Initialize description of coefficient 9.
                objCoeffDef[8].unInterval = 0b0000000100000000;     // Interval = 1.0
                objCoeffDef[8].unDefault = 0b0000000000000000;
                objCoeffDef[8].pbytDes = gbytCoeffKi2;
                
                objCoeffDef[9].fDefault = 0.0;                      // Initialize description of coefficient 10.
                objCoeffDef[9].unInterval = 0b0000000100000000;     // Interval = 1.0
                objCoeffDef[9].unDefault = 0b0000000000000000;
                objCoeffDef[9].pbytDes = gbytCoeffEle;        
                
                OSSetTaskContext(ptrTask, 10, 1*__NUM_SYSTEMTICK_MSEC); // Next state = 1, timer = 1 msec.
                break;
                
            case 10: // State 10 - Wait until RF link is established with remote PC or similar devices before 
                     // proceeding, otherwise keep waiting.
                     // Note: if RF link with remote PC or Tablet computer is established, the robot will first 
                     // transmits the setup information for all tuning parameters to the remote host.  Then
                     // periodic data transmission to the remote PC and will also enabled by setting gnEnRFTxPeriodicData = 1.  
                     // Otherwise if RF link is established with remote host with smaller
                     // display such as smartphone, it is not possible to show robot status periodically and accommodate 
                     // the controls for coefficient tuning.  
                if (gobjDriverHC05.nRFLinkState == 1)     // Check if RF link is established with remote PC.  
                {
                    nCount = 0;       
                    SetSoundPattern(gunSoundConnectedPC,5);     // Generate a string of tones to indicate the RF link established
                                                                // with PC, tablet or similar devices.    
                    nPrintln("Connect PC",10);
                    SetRGBLED(100,100,100,200);                                 // Set indicator LED to white.
                    OSSetTaskContext(ptrTask, 11, 500*__NUM_SYSTEMTICK_MSEC); // Next state = 11, timer = 500 msec.
                }
                else if (gobjDriverHC05.nRFLinkState == 2)
                {
                    SetSoundPattern(gunSoundConnectedSP,5);     // Generate a string of tones to indicate the RF link established
                                                                // with smart phone or similar devices.      
                    nPrintln("Connect SP",10);
                    SetRGBLED(30,60,120,200);                                   // Set indicator LED to light blue.
                    OSSetTaskContext(ptrTask, 22, 500*__NUM_SYSTEMTICK_MSEC); // Next state = 22, timer = 500 msec.
                }                    
                else
                {
                    OSSetTaskContext(ptrTask, 10, 100*__NUM_SYSTEMTICK_MSEC); // Next state = 10, timer = 100 msec.
                }
                break;

            case 11: // State 11 - Transmit the start and interval for tunable coefficients.
                // 16-bits Fixed point format for representing real numbers
                // Q8.8 format is adopted.  The first 8 bits are the fractions, while the
                // subsequent 8 bits are the integers.
                if (gSCIstatus2.bTXRDY == 0)                         // Check if UART1 is idle.
                {    
                    gbytTXbuffer2[0] = gobjDriverHC05.bytRFAdd;      // Set destination address.
                    gbytTXbuffer2[1] = 0x55;                         // Indicate Command.
                    gbytTXbuffer2[2] = _TUNABLE_COEFFICIENT_INFO;
                    gbytTXbuffer2[3] = nCount+1;                     // Indicate coefficient no.
                    unTemp = objCoeffDef[nCount].unDefault;         // Load start value in Q8.8 format.
                                                                    // (relative to initial value), 0.0.
                    gbytTXbuffer2[4] = unTemp>>8;                    // Send upper byte of integer data.
                    gbytTXbuffer2[5] = unTemp;                       // Send lower byte of integer data.
                    unTemp = objCoeffDef[nCount].unInterval;        // Load interval, in Q8.8 format.
                    gbytTXbuffer2[6] = unTemp>>8;
                    gbytTXbuffer2[7] = unTemp;
                    gbytTXbuffer2[8] = *(objCoeffDef[nCount].pbytDes);    // Load description of coefficients.
                    gbytTXbuffer2[9] = *((objCoeffDef[nCount].pbytDes)+1);
                    gbytTXbuffer2[10] = *((objCoeffDef[nCount].pbytDes)+2);
                    gbytTXbuflen2 = 64;
                    gSCIstatus2.bTXRDY = 1;                          // Initiate TX.
                
                    nCount++;                                       // Point to next tuning coefficient.
                    if (nCount == 10)                               // Check for end of list.
                    {
                        OSSetTaskContext(ptrTask, 12, 300*__NUM_SYSTEMTICK_MSEC); // Next state = 12, timer = 300 msec.
                    }
                    else
                    {
                        OSSetTaskContext(ptrTask, 11, 300*__NUM_SYSTEMTICK_MSEC); // Next state = 11, timer = 300 msec.
                    }
                }
                else
                {
                    OSSetTaskContext(ptrTask, 11, 1*__NUM_SYSTEMTICK_MSEC); // Next state = 11, timer = 1 msec.
                }
                break;
                
            case 12: // State 12 - Transmit other constant coefficients (wheel physical properties).
                gbytTXbuffer2[0] = gobjDriverHC05.bytRFAdd;                  // Set destination address.
                gbytTXbuffer2[1] = 0x55;                                     // Indicate Command.
                gbytTXbuffer2[2] = _COEFFICENT_INFO;
                gbytTXbuffer2[3] = _TURN_DEG_PER_100;
                gbytTXbuffer2[4] = _WHEEL_DIAM_MM;
                gbytTXbuffer2[5] = _NOTICKSROTATION;                         // This might be an issue as transmit buffer is only 8 bits, while
                                                                            // constant _NOTICKSROTATION can be larger than 255.
                gbytTXbuflen2 = 64;
                gSCIstatus2.bTXRDY = 1;                                      // Initiate TX.                
                OSSetTaskContext(ptrTask, 21, 300*__NUM_SYSTEMTICK_MSEC); // Next state = 21, timer = 500 msec.
            break;
            
            case 21: // State 21 - Enable sending of periodic binary data.
                gobjDriverHC05.nEnRFTxPeriodicData = 1;         // This is to indicate to HC-05 module driver that system is ready,
                                                                // periodic binary data transmission can commence.
                OSSetTaskContext(ptrTask, 22, 1000);
            break;

            case 22: // State 22 - Check for RF linkage status with Remote Host.  If RF link is lost
                     // reset this process.
                if (gobjDriverHC05.nRFLinkState > 0)                            // Check if RF link is still established.
                {
                    //if (gobjRobotFlag.bRESCOEFF_FACDEF == 1)                    // Check if set to factory default request is asserted.
                    //{                  
                    //    gobjDriverHC05.nEnRFTxPeriodicData = 0;                 // Disable periodic binary data transmission to 
                    //                                                            // prevent collision of wireless data.
                    //    OSSetTaskContext(ptrTask, 30, 250*__NUM_SYSTEMTICK_MSEC);    // Next state = 30, timer = 250 msec.
                    //}   
                    //else
                    //{
                        OSSetTaskContext(ptrTask, 22, 10*__NUM_SYSTEMTICK_MSEC);    // Next state = 22, timer = 10 msec.
                    //}                    
                }
                else                                                            // RF link is lost, reset.
                {
                    SetSoundPattern(gunSoundDisconnected,6);                    // Generate a string of tones to indicate the RF link is lost.  
                    nPrintln("Disconnect",10);
                    SetRGBLED(30,150,50,240);                                   // Set indicator LED to light green.
                    OSSetTaskContext(ptrTask, 10, 10*__NUM_SYSTEMTICK_MSEC);    // Next state = 10, timer = 10 msec.
                }
            break;               
                
            case 100: // State 100 - Idle.
                OSSetTaskContext(ptrTask, 100, 100); // Next state = 100, timer = 100.
            break;

            default:
                gunOSError = gunOSError | 0x0001;   // Set OS error bit 0.
                OSSetTaskContext(ptrTask, 0, 1); // Back to state = 0, timer = 1.
            break;
        }
    }
}


///
/// Process name	: Robot_Sensor_IMU_MPU6050_GP2Y0A41SK0F
///
/// Author          : Fabian Kung
///
/// Last modified	: 19 Dec 2023
///
/// Code Version	: 0.85
///
/// Processor		: dsPIC33EP256MU80X family.
///
/// Processor/System Resource
/// PIN             :  I2C2 pins
///                    AN0, AN1 and AN9 pins (Analog input channels)
///
/// MODULE          :  MPU6050 external IMU module.
///                    GP2Y0A41SK0F external reflective IR distance ranging sensor
///
/// DRIVER          :  Proce_I2C2_Driver
///                    Proce_ADC_Driver
///
/// RTOS            :  Ver 1 or above, round-robin scheduling.
///
/// Global variable :  gobjRobotState
///                    gnStatus_IMU
///                    gnTiltOrient_IMU
///                    gnTheta_Deg_MPU6050
///                    gfTheta_MPU6050   

/// Description     :  This is a driver for Invensense/TDK 6-axis IMU chip, the MPU6050.
///                    and Sharp GP2Y0A41SK0F IR distance sensor.
///                    [MPU6050] - The driver communicates with MPU6050 via I2C bus, obtaining
///                    the accelerometer and gyroscope output, and using complementary filter
///                    method calculate the estimated tilt angle (or pitch) of the robot
///                    platform. 
///                    [GP2Y0A41SK0F] - Up to 3 reflective IR sensors are supported. The driver
///                    samples the ADC channel for each IR sensor and convert the voltage level
///                    into distance of object from the sensor.
///
///                    If there is no delay in each states, the sampling rate is 4.0 msec for both
///                    sensors, e.g. tilt angle (and tilt angular velocity), and the IR sensors 
///                    status are updated every 4.0 msec.
///
///                    Sequence of operation:
///                    1. Upon power up the I2C driver is initialized.
///                    2. The driver then poll the MPU6050 WhoAmI register to make sure the sensor is
///                       attached to the robot controller.  If not the driver revert back to step 1.
///                    3. Once MPU6050 is verified connected to the robot controller, the driver
///                       proceeds to initialize the sensor.
///                    4. Finally the driver goes into normal operating mode, reading the output of the
///                       accelerometer and gyroscope at fixed intervals via I2C bus and compute the angle
///                       from these readings.
///                    5. As there is a waiting period for the I2C bus operation, the driver will use
///                       this waiting period to sample the ADC channels connected to the IR sensors 
///                       and compute the distance of object from the IR sensors.
///                     
/// Example of usage: 
///                   Make sure gnStatus_IMU equals _READY.
///                   gfTheta_MPU6050 gives the angle measured along Y axis in radian.
///                   gnTheta_Deg_MPU6050 gives the same angle in degree.
///                   gnTiltOrient_IMU gives the orientation (in enumerated form).
///                   gobjRobotState.nFrontObjDistance1_mm and gobjRobotState.nFrontObjDistance2_mm store
///                   the current state for front IR sensor.
///                   gobjRobotState.nBackObjDistance_mm stores the current state for the back IR sensor.
///                   These parameters can be read as frequent as required, but their values will only
///                   be updated every 4.0 msec.


// --- Check for RTOS version required with this code ---
#ifdef __OS_VER
	#if __OS_VER < 1
		#error "Robot_Sensor_MPU6050: Incompatible OS version"
	#endif
#else
	#error "Robot_Sensor_MPU6050: An RTOS is required with this function"
#endif

#define     __MPU6050_I2C_ADDRESS       0x68    // I2C slave address of MPU6050 (Assume pin AD0 is shorted to GND).

#define	_MPU6050_SMPLRT_DIV     0x19	//125Hz
#define	_MPU6050_CONFIG			0x1A	//0x06(5Hz)
#define	_MPU6050_GYRO_CONFIG	0x1B	//2000deg/s
#define	_MPU6050_ACCEL_CONFIG	0x1C	//5Hz
#define	_MPU6050_ACCEL_XOUT_H	0x3B
#define	_MPU6050_ACCEL_XOUT_L	0x3C
#define	_MPU6050_ACCEL_YOUT_H	0x3D
#define	_MPU6050_ACCEL_YOUT_L	0x3E
#define	_MPU6050_ACCEL_ZOUT_H	0x3F
#define	_MPU6050_ACCEL_ZOUT_L	0x40
#define	_MPU6050_TEMP_OUT_H		0x41
#define	_MPU6050_TEMP_OUT_L		0x42
#define	_MPU6050_GYRO_XOUT_H	0x43
#define	_MPU6050_GYRO_XOUT_L	0x44	
#define	_MPU6050_GYRO_YOUT_H	0x45
#define	_MPU6050_GYRO_YOUT_L	0x46
#define	_MPU6050_GYRO_ZOUT_H	0x47
#define	_MPU6050_GYRO_ZOUT_L	0x48
#define	_MPU6050_PWR_MGMT_1		0x6B	//
#define	_MPU6050_WHO_AM_I		0x75	//
#define	_MPU6050_SlaveAddress	0xD0	


void Robot_Sensor_IMU_MPU6050_GP2Y0A41SK0F(TASK_ATTRIBUTE *ptrTask)
{
    unsigned int unTemp;
    long lngTemp;    
    static  int     nAccelRaw, nGyroRaw;
    static  float   fTemp1, fTemp2, fTemp3;
    static  float   fAngleAccel;
    static  float   fTheta, fTheta1;
     
    if (ptrTask->nTimer == 0)
    {
        switch (ptrTask->nState)
        {
            case 0: // State 0 - Initialization.
                gbytI2CSlaveAdd2 =  __MPU6050_I2C_ADDRESS;               // Initialize slave device I2C address. 
                fTheta = 0.0;
                fTheta1 = 0.0;
                nAccelRaw = 0.0;
                nGyroRaw = 0.0;
                gfWg_MPU6050 = 0.0;
                gfTheta_MPU6050 = 0.0;
                gnTheta_Deg_MPU6050 = 0;
                OSSetTaskContext(ptrTask, 1, 100*__NUM_SYSTEMTICK_MSEC); // Next state = 3, timer = 100 msec.
                break;
            
            case 1: // State 1 - Read WhoAmI register of MPU6050 to make sure it is present.    
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 1;                               // Indicate no. of bytes to read.
                    gbytI2CRegAdd2 = _MPU6050_WHO_AM_I;                  // Start address of register.
                    gI2CStat2.bRead = 1;           
                    OSSetTaskContext(ptrTask, 2, 1*__NUM_SYSTEMTICK_MSEC);      // Next state = 2, timer = 1 msec.
                    //OSSetTaskContext(ptrTask, 1, 100*__NUM_SYSTEMTICK_MSEC);      // Next state = 2, timer = 100 msec.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 1, 1);                    // Next state = 1, timer = 1.
                }                        
                break;
                
            case 2: // State 2 - Check the content of the read to make sure MPU6050 is present.  Else restart.
                if (gI2CStat2.bRead == 0)                               // Check if Read operation is completed.
                {                                                       // Read operation complete, check received data.
                    if (gbytI2CRXbuf2[0] == 0x68)                       // The default content of WhoAmI register is 0x68.
                    {
                        //gnAudioTone[0] = 4;                             // Sound a beep to indicate that IMU sensor is present.
                        //gnAudioTone[1] = 2;
                        //gnAudioTone[2] = 0;
                        OSSetTaskContext(ptrTask, 3, 1*__NUM_SYSTEMTICK_MSEC); // Next state = 3, timer = 1 msec.
                        //OSSetTaskContext(ptrTask, 1, 250*__NUM_SYSTEMTICK_MSEC); // Next state = 1, timer = 250 msec.
                    }
                    else
                    {
                        SetSoundPattern(gunSoundIMUAbsent,4);          // Sound an alarm if IMU not detected.  
                        nPrintln("IMU Absent",11);
                        OSSetTaskContext(ptrTask, 1, 500*__NUM_SYSTEMTICK_MSEC);    // Back to state = 0, timer = 500 msec.
                    }
                }
                else if (gI2CStat2.bCommError == 1)                     // Check for I2C bus error.
                {
                    gI2CStat2.bCommError = 0;                           // Clear bus error flag.
                    OSSetTaskContext(ptrTask, 0, 1);                    // Next state = 0, timer = 1.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 2, 1);                    // Next state = 2, timer = 1.
                }
                break;
                
            case 3: // State 3 - Initialize MPU6050.
                    // Set clock source to internal PLL (from X-gyroscope).
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 1;                              // Indicate no. of bytes to transmit.
                    gbytI2CRegAdd2 = _MPU6050_PWR_MGMT_1;               // Start address of register.
                    gbytI2CTXbuf2[0] = 0x01;                            // Data.  Wakes up device and set internal clock to X gyro PLL.
                    //gbytI2CTXbuf2[0] = 0x00;                            // Data.  Wakes up device and set internal clock to 8 MHz RC oscillator.
                    gI2CStat2.bSend = 1;
                    OSSetTaskContext(ptrTask, 4, 1*__NUM_SYSTEMTICK_MSEC);      // Next state = 4, timer = 1 msec.
                    //OSSetTaskContext(ptrTask, 1, 250*__NUM_SYSTEMTICK_MSEC); // Next state = 1, timer = 250 msec.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 3, 1);                    // Next state = 3, timer = 1.
                }                
                break;

            case 4: // State 4 - Initialize MPU6050.
                    // Set sample rate for both accelerometer and gyroscope.
                    // Sample rate = (Actual output rate)/(1 + _MPU6050_SMPLRT_DIV)
                    // According to Jeff Rowland, the sample rate for accelerometer is 1 kHz.
                    // For gyroscope it is 8 kHz if the digital LPF (dLPF) is disabled, and 1 kHz
                    // if dLPF is enabled.  I suppose this is an 8 tabs IIR filter.
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 1;                              // Indicate no. of bytes to transmit.
                    gbytI2CRegAdd2 = _MPU6050_SMPLRT_DIV;               // Start address of register.
                    //gbytI2CTXbuf[0] = 0x02;                             // Data - sample rate = 333.33 sample/sec.
                    //gbytI2CTXbuf[0] = 0x03;                             // Data - sample rate = 250 sample/sec.
                    //gbytI2CTXbuf[0] = 0x04;                             // Data - sample rate = 200 sample/sec.
                    gbytI2CTXbuf2[0] = 0x00;                            // Data - sample rate = 1000 sample/sec.
                    //gbytI2CTXbuf[0] = 0x06;                             // Data - sample rate = 142.857 sample/sec.
                    gI2CStat2.bSend = 1;
                    OSSetTaskContext(ptrTask, 5, 1*__NUM_SYSTEMTICK_MSEC);      // Next state = 5, timer = 1 msec.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 4, 1);                    // Next state = 4, timer = 1.
                }                
                break;
                
            case 5: // State 5 - Initialize MPU6050 - Set external synchronization and digital LPF bandwidth.
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 1;                              // Indicate no. of bytes to transmit.
                    gbytI2CRegAdd2 = _MPU6050_CONFIG;                   // Start address of register.
                    //gbytI2CTXbuf[0] = 0x01;                           // Data - disable external synchronization, BW around 194 Hz.
                    //gbytI2CTXbuf[0] = 0x02;                             // Data - disable external synchronization, BW around 94 Hz.
                    gbytI2CTXbuf2[0] = 0x03;                            // Data - disable external synchronization, BW around 44 Hz.
                    //gbytI2CTXbuf[0] = 0x04;                             // Data - disable external synchronization, BW around 20 Hz.
                    //gbytI2CTXbuf[0] = 0x05;                             // Data - disable external synchronization, BW around 10 Hz.
                    //gbytI2CTXbuf[0] = 0x06;                             // Data - disable external synchronization, BW around 5 Hz.
                    gI2CStat2.bSend = 1;
                    OSSetTaskContext(ptrTask, 6, 1*__NUM_SYSTEMTICK_MSEC);      // Next state = 6, timer = 1 msec.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 5, 1);                    // Next state = 5, timer = 1.
                }                
                break;                

            case 6: // State 6 - Initialize MPU6050 - Set full-scale range of gyroscope.
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 1;                              // Indicate no. of bytes to transmit.
                    gbytI2CRegAdd2 = _MPU6050_GYRO_CONFIG;              // Start address of register.
                    gbytI2CTXbuf2[0] = 0x00;                            // Data - +-250 deg/sec.
                    //gbytI2CTXbuf[0] = 0x08;                             // Data - +-500 deg/sec.
                    //gbytI2CTXbuf[0] = 0x10;                             // Data - +-1000 deg/sec.
                    //gbytI2CTXbuf[0] = 0x18;                             // Data - +-2000 deg/sec.
                    gI2CStat2.bSend = 1;
                    OSSetTaskContext(ptrTask, 7, 1*__NUM_SYSTEMTICK_MSEC);      // Next state = 7, timer = 1 msec.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 6, 1);                    // Next state = 6, timer = 1.
                }                
                break;   

            case 7: // State 7 - Initialize MPU6050 - Set full-scale range of accelerometer.
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 1;                              // Indicate no. of bytes to transmit.
                    gbytI2CRegAdd2 = _MPU6050_ACCEL_CONFIG;             // Start address of register.
                    //gbytI2CTXbuf[0] = 0x00;                             // +-2g.
                    gbytI2CTXbuf2[0] = 0x08;                            // +-4g.
                    gI2CStat2.bSend = 1;
                    OSSetTaskContext(ptrTask, 8, 1*__NUM_SYSTEMTICK_MSEC);      // Next state = 8, timer = 1 msec.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 7, 1);                    // Next state = 7, timer = 1.
                }                
                break;
                
            case 8: // State 8 - Read accelerometer data.
                    // Also set the robot orientation parameter gnTiltOrient_IMU.  
                    // Note:  Actually this can be done at any stages in the tilt angle computation.  
                    // Most other states (from state 9 to 13) has some floating points computations, 
                    // so the loading is high.  Thus we slot this routine in State 8 as it has no 
                    // floating point computation if this routine is not present.           
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 2;                              // Indicate no. of bytes to read.
                    gbytI2CRegAdd2 = _MPU6050_ACCEL_YOUT_H;             // Start address of register.
                    gI2CStat2.bRead = 1;                                // Start read.
                    OSSetTaskContext(ptrTask, 9, 7);                    // Next state = 9, timer = 7.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 8, 1);                    // Next state = 8, timer = 1.
                }
                
                gnTheta_Deg_MPU6050 = fTheta*57.2958;                   // Tilt angle in degree.
                gfTheta_MPU6050 = fTheta;                               // Tilt angle in radian.
                
                if (gnTheta_Deg_MPU6050 > 25)                       // Check tilt angle for threshold of leaning forward.
                {
                    gnTiltOrient_IMU = _ROR_TOPPLE_FRONT;           // Robot topples to the front.
                }
                else if (gnTheta_Deg_MPU6050 > 4)
                {
                    gnTiltOrient_IMU = _ROR_LEAN_FRONT;             // Robot leans to the front.
                }
                else if (gnTheta_Deg_MPU6050 >= -4)
                {
                    gnTiltOrient_IMU = _ROR_UPRIGHT;                // Robot is upright.
                }
                else if (gnTheta_Deg_MPU6050 > -25)                 // Check tilt angle for threshold of leaning back.
                {
                    gnTiltOrient_IMU = _ROR_LEAN_BACK;              // Robot leans to the back.
                }
                else
                {
                    gnTiltOrient_IMU = _ROR_TOPPLE_BACK;           // Robot topples to the back.               
                }                     
                break;
            
            case 9: // State 9 - Sample output of reflective IR Sensor 1 and convert to mm.
                if (gobjDriverADC.unADC1_mV > 0)                        // To prevent divide-by-zero.
                {
                    lngTemp = 125000/gobjDriverADC.unADC1_mV;           // See notes and datasheet on the derivation                                                                                    
                    gobjRobotState.unSensorRangeR_mm = lngTemp;         // of this sensor output.
                    if (gobjRobotState.unSensorRangeR_mm > 300)         // Limit to 300 mm.
                    {
                        gobjRobotState.unSensorRangeR_mm = 300;
                    }
                }
                else
                {
                    gobjRobotState.unSensorRangeR_mm = 300;
                }                
                OSSetTaskContext(ptrTask, 10, 1);                       // Next state = 10, timer = 1.
                break;
                
            case 10: // State 10 - Sample output of reflective IR Sensor 2 and convert to mm.
                if (gobjDriverADC.unADC2_mV > 0)                        // To prevent divide-by-zero.
                {    
                    lngTemp = 125000/gobjDriverADC.unADC2_mV;           // See notes and datasheet on the derivation                                                            
                    gobjRobotState.unSensorRangeL_mm = lngTemp;         // of this sensor.
                    if (gobjRobotState.unSensorRangeL_mm > 300)         // Limit to 300 mm.
                    {
                        gobjRobotState.unSensorRangeL_mm = 300;
                    }          
                }
                else
                {
                    gobjRobotState.unSensorRangeL_mm = 300;
                }                
                OSSetTaskContext(ptrTask, 11, 1);                       // Next state = 11, timer = 1.
                break;

            case 11: // State 11 - Sample output of reflective IR Sensor 3 and convert to mm.
                if (gobjDriverADC.unADC3_mV > 0)                        // To prevent divide-by-zero.
                {
                    lngTemp = 125000/gobjDriverADC.unADC3_mV;           // See notes and datasheet on the derivation                                                                                                
                    gobjRobotState.unSensorRangeF_mm = lngTemp;         // of this sensor.
                    if (gobjRobotState.unSensorRangeF_mm > 300)         // Limit to 300 mm.
                    {
                        gobjRobotState.unSensorRangeF_mm = 300;
                    }
                }   
                else
                {
                    gobjRobotState.unSensorRangeF_mm = 300;
                }                
                OSSetTaskContext(ptrTask, 12, 1);                   // Next state = 12, timer = 1.
                break;                
                
            case 12: // State 12 - Read accelerometer data.
                if (gI2CStat2.bRead == 0)                               // Check if Read operation is completed.
                {                                                       // Read operation complete, check received data.
                    unTemp = gbytI2CRXbuf2[0]<<8;                       // Get upper 8 bits.                                //
                    nAccelRaw = unTemp + gbytI2CRXbuf2[1];              // Form 16 bits unsigned integer by adding lower 8 bits. 
                                                                        // Note that the data is 16-bits 2's complement format.
                    if (nAccelRaw > 8192)                               // Limit the raw accelerator output, for +-4g full scale.
                    {
                        nAccelRaw = 8192;
                    }
                    if (nAccelRaw < -8192)
                    {
                        nAccelRaw = -8192;
                    }    
                    OSSetTaskContext(ptrTask, 13, 1);                   // Next state = 13, timer = 1.
                    fTemp1 = 0.00012207*nAccelRaw;                      // Convert to g, where 1/8192 = 0.00012207
                }
                else
                {
                    OSSetTaskContext(ptrTask, 12, 1);                   // Next state = 12, timer = 1.
                }
                break;
                
            case 13: // State 13 - Read gyroscope data.
                if (gI2CStat2.bI2CBusy == 0)                            // Make sure I2C module is not being used.
                {
                    gbytI2CByteCount2 = 2;                              // Indicate no. of bytes to read.
                    gbytI2CRegAdd2 = _MPU6050_GYRO_XOUT_H;              // Start address of register.
                    gI2CStat2.bRead = 1;                                // Start read. 
                    OSSetTaskContext(ptrTask, 14, 10);                  // Next state = 11, timer = 10.
                    
                    fAngleAccel = asinf(fTemp1);                        // Convert to radian.
                }                
                else
                {
                    OSSetTaskContext(ptrTask, 13, 1);                   // Next state = 13, timer = 1.
                }                  
                break;

            case 14: // State 14 - Read gyroscope data.
                if (gI2CStat2.bRead == 0)                               // Check if Read operation is completed.
                {                                                       // Read operation complete, check received data.
                    unTemp = gbytI2CRXbuf2[0]<<8;                       // Get upper 8 bits.                                //
                    nGyroRaw = unTemp + gbytI2CRXbuf2[1];               // Form 16 bits unsigned integer by adding lower 8 bits. 
                                                                        // Note that the data is 16-bits 2's complement format.
                    OSSetTaskContext(ptrTask, 15, 1);                   // Next state = 15, timer = 1.                    
                    gfWg_MPU6050 = 0.0001332*nGyroRaw;                  // Sensitivity = 1/131 or 0.0076336 for +-250 dps full scale.
                                                                        // Since our unit is radian not degree. We must multiply
                                                                        // by (pi/180) or 0.017453.  Thus the total coefficient is
                                                                        // 0.017453*0.0076336 = 0.0001332.
                }
                else
                {
                    OSSetTaskContext(ptrTask, 14, 1);                   // Next state = 14, timer = 1.
                }
                break;
                
            case 15: // State 15 - Compute tilt angle using Complementary Filter approach.  Sampling interval
                     // is 4 msec (0.004).
                fTheta1 = fTheta;                               // Store previous tilt angle sample.
                fTemp1 = gfWg_MPU6050*0.004;                    // Theta(n) = a(Theta(n-1) + W*Delta_t) + (1-a)(ThetaAcc(n))
                fTemp2 = fTheta1 + fTemp1;                      // The time-constant for the low-pass and high-pass filters                
                OSSetTaskContext(ptrTask, 16, 1);               // Next state = 16, timer = 1.
                break;

            case 16: // State 16 - Combine outputs from accelerometer and gyroscope using Complementary Filters
                    // approach.    Coefficient a = 0.994.
                fTemp2 = 0.994*fTemp2;			
                fTemp3 = 0.006*fAngleAccel;		 
                fTheta = fTemp2 + fTemp3;                       // Work out final tilt angle.
                
                gnStatus_IMU = _READY;                          // Indicate IMU module has valid output.
                OSSetTaskContext(ptrTask, 8, 1);                // Next state = 8, timer = 1.
            break;
           
            default:
                gunOSError = gunOSError | 0x0001;   // Set OS error bit 0.
                OSSetTaskContext(ptrTask, 0, 1); 	// Back to state = 0, timer = 1.
            break;                
        }
    }
}


///
/// Process name	: Robot_Move
///
/// Author          : Fabian Kung
///
/// Last modified	: 5 Dec 2023
///
/// Code version	: 1.00
///
/// Processor		: dsPIC33CK256MP50X family.
///
/// Processor/System Resource
/// PIN             :  None
///
/// MODULE          :  Motor Driver
///
/// DRIVER          :  Driver_DualDCMotor_V100.c
///
/// RTOS            :  Ver 1 or above, round-robin scheduling.
///
/// Global variables	: gnKp_Move, gnKd_Move, 
///                       gnKp_Turn, gnKd_Turn, gfKi_Turn
///                       gfOmegaW, gnDistanceMoveW
///                       gfOmegaWSet
///                       gnlDistanceSet
///                       gnHeading, gnTurnSpeedSet
///
///
/// Description	:  This routine control the dual DC motor driver to move the robot 
///                forward, backward, turn left and right. 
///                A simple 'bang-bang' control is used for linear motion of moving
///                forward and backward, with sampling interval of 1 msec, while 
///                a PID feedback control is used to make the robot turns, with
///                sampling interval of 20 msec.
///
///                Usage:
///                1. Linear motion: Just set the global variable gfOmegaWSet to set the linear 
///                movement speed of the robot.
///
///                gfOmegaWSet(in rotation/sec)  Movement
///                -------------                 --------
///                   > 0.0                      Forward, the larger the magnitude the faster the movement.
///                   < 0.0                      Backward, larger magnitude indicates faster movement.
///                   0.0                        Stationary.
///
///                Finally for forward or backward motion, there are 4 speed standard settings defined in the 
///                robot parameters header file, depending on the magnitude of gfOmegaWSet, e.g.:
///                              Very slow
///                              Slow
///                              Normal
///                              Fast
///               2. Turning: There are two turning modes:
///               a) Turn to a particular heading
///               Set gnTurnSpeed = 0, and just set the global variable gnHeadingSet to make the robot
///               turns to the required direction. gnHeadingSet = 0 the robot will point straight,
///               gnHeadingSet > 0 and the robot will turn left, gnHeadingSet < 0 and the robot
///               will turn right.
///               b) Continuous turning
///               Set gnTurnSpeedSet to positive to turn continuously left, and negative to turn 
///               continuously right. The parameter gnHeading will be ignored.
///              
///               3. If gfOmegaWSet is non-zero and gnHeadingSet or gnTurnSpeedSet is modify, the robot platform
///               will move linearly and turn and the same time.
///                   
/// Example of usage:
/// 1. To make the robot move forward at constant velocity at moderate speed, set 
/// gfOmegaWSet = 0.3 rotation/sec for forward motion or < -0.3 rotation/sec for backward motion. 
/// 2. To make the robot move forward at constant velocity at higher speed, set 
/// gfOmegaWSet = 0.6 for forward motion or < -0.6 for backward motion.  
/// For all (1) and (2) cases above, to stop, simply set gfOmegaWSet to 0.0.  
/// 3. Maximum speed is capped to +-_MOVELINEAR_MAX_SPEED rotation/sec in the codes.

void Robot_Move(TASK_ATTRIBUTE *ptrTask)
{
    static int  nSysTick1Second;                            
    static int  gnDistanceErrorOld;
    static int  nOmegaWSet;                 // Average required wheel rotation velocity in int format. 
    static int  nMaxOmegaWLimit;            // Maximum allowable speed.
    static int  nCurrentMotorSet;
    static int  nTurn;
    static int  nE1;		
    static int  nE2;
    static int  nESum;
    static int  nTurnCount;
 
#define     _MOVELINEAR_MAX_SPEED   1.0     // In rotation/sec
    
    if (ptrTask->nTimer == 0)
    {
        switch (ptrTask->nState)
        {
            case 0: // State 0 - Initialization. Set all system variables.
                gfOmegaWSet = 0.0;
                nOmegaWSet = 0;
                
                gnlDistanceSet = 0; 
                gnDistanceErrorOld = 0;
                nCurrentMotorSet = 0;
                gnTurnSpeedSet = 0;
                
                gnKp_Turn = _FP2_TURN_DEFAULT;                                  // Initialize the PID loop coefficients
                gnKd_Turn = _FD2_TURN_DEFAULT;                                  // for turning.
                gnKi_Turn = _FI2_TURN_DEFAULT;
                nE1 = 0;
                nE2 = 0;
                nESum = 0;
                nTurn = 0;
                nTurnCount = 0;
                
                gnKp_Move = 4;                                                  // Initialize the PD loop coefficients
                gnKd_Move = 0;                                                  // for linear motions.
                nSysTick1Second = __NUM_SYSTEMTICK_MSEC*1000;                   // Number of system ticks in 1 second.
                nMaxOmegaWLimit = _MOVELINEAR_MAX_SPEED * _NOTICKSROTATION;
                OSSetTaskContext(ptrTask, 1, 1000*__NUM_SYSTEMTICK_MSEC);        // Next state = 1, timer = 1000 msec.
            break;
                
            case 1: // State 1 - Idle.
                OSSetTaskContext(ptrTask, 2, 1);                                // Next state = 2, timer = 1.
                break;
                
            case 2: // State 2 - Set motor speed according to user setting. 
                nOmegaWSet = gfOmegaWSet*_NOTICKSROTATION;              // Compute the request linear velocity in integer format, normalized
                                                                        // to the number of ticks per rotation of wheel. The parameter gfOmegaWSet
                                                                        // is expressed in number of rotation per second.  So this translates to
                                                                        // a rotation speed of number of ticks per second.
                                                                        // The higher the wheel rotational speed the higher is the no. of pulses 
                                                                        // or ticks expected in 1 second.  
                if (nOmegaWSet > nMaxOmegaWLimit)                       // Limit to maximum and minimum limits.
                {
                    nOmegaWSet = nMaxOmegaWLimit;
                }
                if (nOmegaWSet < -nMaxOmegaWLimit)
                {
                    nOmegaWSet = -nMaxOmegaWLimit;
                }                
                
                if (nCurrentMotorSet < (gnKp_Move*nOmegaWSet))          // Adjust the motor drive setting linearly.
                {
                    nCurrentMotorSet++;
                }
                else if (nCurrentMotorSet > (gnKp_Move*nOmegaWSet))
                {
                    nCurrentMotorSet--;
                }    

                gobjDCMotor.nMotor1 = nCurrentMotorSet + nTurn;         // Drive motor, with differential offset
                gobjDCMotor.nMotor2 = nCurrentMotorSet - nTurn;         // added for turning.
                
                nTurnCount++;                                           // PID feedback control for turning is 
                if (nTurnCount == 10)                                   // only executed at 10 msec interval.
                {
                    nTurnCount = 0;
                    OSSetTaskContext(ptrTask, 3, 1);                            // Next state = 3, timer = 1.
                }
                else
                {
                    OSSetTaskContext(ptrTask, 2, 1*__NUM_SYSTEMTICK_MSEC);      // Next state = 2, timer = 1 msec.
                }
                break;
                
            case 3: // State 3 - PID feedback control loop to calculate offset value for turning.
                
                if (gnTurnSpeedSet > 0) // Continuous turning, left.
                {
                    nTurn = 500;
                }
                else if (gnTurnSpeedSet < 0) // Continuous turning, right.
                {
                    nTurn = -500;
                }
                else
                {
                    nE1 = gnHeadingSet - gobjQEOut.nHeading;                        // Compute error in current heading.
                    nESum = nESum + nE1;                                            // Accumulate error term.
                    nE2 = nE1 - nE2;                                                // Compute difference of current and previous error.
                    nTurn = gnKp_Turn*nE1 + gnKd_Turn*nE2 + (gnKi_Turn*nESum/4);                          // Compute the turn offset differential voltage.
                    //nTurn = nTurn + (gnKi_Turn*nESum);
                    if (nTurn > 1000)
                    {
                        nTurn = 1000;
                    }
                    else if (nTurn < -1000)
                    {
                        nTurn = -1000;
                    }
                }
                nE2 = nE1;                                                      // Store current error value.     
                OSSetTaskContext(ptrTask, 2, 1);                                // Next state = 2, timer = 1.
                break;
                
            default:
                gunOSError = gunOSError | 0x0001;   // Set OS error bit 0.
                OSSetTaskContext(ptrTask, 0, 1);                        // Back to state = 0, timer = 1.
            break;
        }
    }
}


///
/// Process name		: Robot_Proce1
///
/// Author              : Fabian Kung
///
/// Last modified		: 8 Oct 2023
///
/// Code Version		: 1.00
///
/// Processor           : dsPIC33CK256MPXXX family.
///
/// Processor/System Resources
/// PINS                : 
///
/// MODULES             : All internal and external modules.
///
/// RTOS                : Ver 1 or above, round-robin scheduling.
///
/// Global variables    : 
///

#ifdef 				  __OS_VER			// Check RTOS version compatibility.
	#if 			  __OS_VER < 1
		#error "Robot_Proce1: Incompatible OS version"
	#endif
#else
	#error "Robot_Proce1: An RTOS is required with this function"
#endif

/// Description: Robot process 1.  
/// This process is the first layer of the robot processes.  It is like the basic instinct,
/// low level and involuntary movement control in vertebrate animals (e.g. the brain
/// stem and part of the reptilian brain). We can also compare this to the center nervous system
/// (CNS) of insect :P
/// The process main functions:
/// 1. Sets the operating mode of the robot into:
///   Normal mode
///   Test mode 1 - Test the motor driver for left and right wheels and encoders.
///   Test mode 2 - Disable the motor driver for wheels, used for all sensors.
///
/// 2. Set the joint angle and rotation speed of the servo motors (if attached), and also the
///    motor overload limit.
///
/// 3. Perform FFT computation on one system parameter, here we choose the average wheel velocity
///    as the parameter to be transformed into frequency domain.  The analysis duration
///    is 30x20msec = 1.5 sec.
///
/// 4. Computation of stability indices, i.e. a number of parameters that are used to indicate how
///    stable the robot platform is at any instant.  Here the indices are derived from the result
///    of the FFT. The stability indice indicate if the robot platform is standing still, moving
///    at constant velocity of vibrating violently. 
///
/// 5. Adaptive tuning of state-feedback control coefficients based on robot operating mode or
///    stability indices.
///
/// 7. Other basic functions that include:
/// a. Initialize the robot to default state upon power up (balancing, movement modes, all
///    servo motors and DC motors etc).
/// b. Monitors the states of the IMU sensors and determines if the robot is toppled or not.
/// c. Check for internal error condition like sensors and wheel encoder registers overflow.
///    Reset the robot if this happen.
/// d. Prevent damage to the robot by shutting down the motor driver circuit if the robot
///    is in the process of toppling over.  Also restore the robot to default stage when 
///    it is upright again.
/// e. Prevent damage to the smart servo motors from overload by limiting the shaft 
///    movement of the smart servo motor when overload threshold is exceeded.
///
/// The process executes with a periodic interval of 49.5 msec.  Thus in 1 second the
/// routines in the process will be executed around 20 times.
///
 
#define     _MAX_DISTANCE_MOVE  2147482648      // (2^31) - 1000, in terms of wheel encoder ticks.
#define     _MIN_DISTANCE_MOVE  -2147482648     // -(2^31) + 1000, in terms of wheel encoder ticks.
#define     _MOTOR_G15_OVERLOAD_THRESHOLD_DEFAULT   390 // A value between 1-511 (9 bits unsigned integer)
                                                // 511 corresponds to full load. Here I set the
                                                // threshold to 75% of the max load the motor
                                                // can handle.

void Robot_Proce1(TASK_ATTRIBUTE *ptrTask)
{    
    if (ptrTask->nTimer == 0)
    {
        switch (ptrTask->nState)
        {
            case 0: // State 0 - Initialization of on-board modules, drivers and global states.              
                // Set the robot drivers to a known state.                          
                
                gobjDriverQuadEncoder.unGearRatio = _MOTOR_GEAR_RATIO;       // Set DC motor gearbox ratio.
                gobjDriverQuadEncoder.unEncoder_CPR = _MOTOR_ENCODER_CPR;    // Set DC motor encoder count per rotation (CPR).
                gobjDriverQuadEncoder.unEncoder_Div = _MOTOR_QUAD_DIVIDE;    // Set the quadrature encoder division ratio.
                                                        // This is an integer value that divides the actual
                                                        // quadrature encoder output, otherwise the quadrature
                                                        // encoder output pulses maybe to frequent and fast 
                                                        // for the software to handle.
                gobjDriverPWM.unEnPWM = _ENABLE_PWMSERVO_DRIVER;  // Enable the PWM servo driver.  
                gobjDriverQuadEncoder.nQuadEncoderStatus = _QUADENCODER_RESET; // Start the quadrature encoder.
                gobjDCMotor.unEnDriver = _ENABLE_DCMOTOR_DRIVER;
                SetRGBLED(120,20,20,150);                       // Set indicator LED color and blink rate.
                OSSetTaskContext(ptrTask, 1, 1000*__NUM_SYSTEMTICK_MSEC); // Next state = 1, timer = 1000 msec.
            break;
                
            case 1: // State 1 - Start up. Add debug codes here if needed. 
                // --- For MVM V1.5C ---
                gbytTXbuffer[0] = 0x20;    // Enable IPA2 and IPA3 to run at prescribed intervals.                          
        		gbytTXbuflen = 1;          // Set TX frame length.
                gSCIstatus.bTXRDY = 1;     // Initiate TX.         
                OSSetTaskContext(ptrTask, 10, 1000*__NUM_SYSTEMTICK_MSEC); 
               // OSSetTaskContext(ptrTask, 100, 100*__NUM_SYSTEMTICK_MSEC);         // Next state = 100, timer = 100 msec.
                //OSSetTaskContext(ptrTask, 1, 1000*__NUM_SYSTEMTICK_MSEC);         // Next state = 1, timer = 1000 msec.
                break;
            
            case 2: // State 2 - Start up. Add debug codes here if needed.   
                gbytTXbuffer[0] = 0x30;    // Enable IPA2 and IPA3 to run at prescribed intervals.                          
        		gbytTXbuflen = 1;          // Set TX frame length.
                gSCIstatus.bTXRDY = 1;     // Initiate TX.         
                OSSetTaskContext(ptrTask, 1, 3000*__NUM_SYSTEMTICK_MSEC);                 
                break;
              
                
            case 10: // State 10 - 
                nSetRCServoMotor(50, _MOTORSPEED_SLOW,_MOTOR0_LEFT);
                nSetRCServoMotor(60, _MOTORSPEED_SLOW,_MOTOR1_LEFT);
                OSSetTaskContext(ptrTask, 11, 1*__NUM_SYSTEMTICK_MSEC); // Next state = 11, timer = 1 msec.
                break;
                
            case 11: // State 11 - 
                OSSetTaskContext(ptrTask, 10, 19*__NUM_SYSTEMTICK_MSEC); // Next state = 10, timer = 19 msec.
                break;
                
            // --- Debug or Production Test Routines ---   
            // Test motor driver routines: Rotate left and right wheels CW and CCW alternately.
            // These routines are used during production and trouble-shooting to check the condition
            // of the motor driver.
            case 100: // State 100 - Test.
                OSSetTaskContext(ptrTask, 100, 1*__NUM_SYSTEMTICK_MSEC); // Next state = 100, timer = 1 msec.
                //OSSetTaskContext(ptrTask, 101, 1*__NUM_SYSTEMTICK_MSEC); // Next state = 101, timer = 1 msec.
            break;
            
            case 101: // State 101 - Test 1.
                //SetRWheelProperty(_FORWARD);                  // Set wheel direction.
                //SetLWheelProperty(_FORWARD);    
                //SetRWheelProperty(_BACKWARD, -1.0);
                //SetLWheelProperty(_BACKWARD, -1.0);  
                //gnMotor1 = 600;    // Right motor.
                //gnMotor2 = 600;    // Left motor.
                gobjDCMotor.nMotor1 = 700;
                gobjDCMotor.nMotor2 = 700;
                OSSetTaskContext(ptrTask, 102, 1000*__NUM_SYSTEMTICK_MSEC); // Next state = 102, timer = 1 msec.
            break;

            case 102: // State 102 - Test.
                gobjDCMotor.nMotor1 = 0;
                gobjDCMotor.nMotor2 = 0;                
                OSSetTaskContext(ptrTask, 103, 250*__NUM_SYSTEMTICK_MSEC); // Next state = 103, timer = 0.25 msec.
                break;
                
            case 103: // State 103 - Test. 
                //SetRWheelProperty(_REVERSE);                  // Set wheel direction.
                //SetLWheelProperty(_REVERSE);                   
                //gnMotor1 = 600;
                //gnMotor2 = 600;
                gobjDCMotor.nMotor1 = -700;
                gobjDCMotor.nMotor2 = -700;                
                OSSetTaskContext(ptrTask, 104, 1000*__NUM_SYSTEMTICK_MSEC); // Next state = 104, timer = 1 msec.
            break;
                
             case 104: // State 104 - Test.
                gobjDCMotor.nMotor1 = 0;
                gobjDCMotor.nMotor2 = 0;                
                OSSetTaskContext(ptrTask, 101, 250*__NUM_SYSTEMTICK_MSEC); // Next state = 101, timer = 0.25 msec.
                break;
                
            default:
                gunOSError = gunOSError | 0x0001;   // Set OS error bit 0.
                OSSetTaskContext(ptrTask, 0, 1); // Back to state = 0, timer = 1.
            break;
        }
    }
}


///
/// Process name		: Robot_ExControllerMessageLoop
///
/// Author              : Fabian Kung
///
/// Last modified		: 25 Dec 2023
///
/// Code Version		: 0.96
///

/// Processor           : dsPIC33EP256MU80X family.
///
/// Processor/System Resources
/// PINS                : 
///
/// MODULES             : UART1 (Internal))
///
/// RTOS                : Ver 1 or above, round-robin scheduling.
///
/// Global variables	: 

#ifdef 				  __OS_VER			// Check RTOS version compatibility.
	#if 			  __OS_VER < 1
		#error "Robot_ExControllerMessageLoop: Incompatible OS version"
	#endif
#else
	#error "Robot_ExControllerMessageLoop: An RTOS is required with this function"
#endif

/// Description:
/// 1. This process handles text based information exchange with an External Controller (EC) via
/// UART2 port.  The process can accept text command and also return parameters requested
/// by the EC in alpha-numeric characters.  Parameters returned are either text string or 3-digits BCD 
/// from 0 to 255.
/// --- Command packet from EC --- 
/// [Command] + [Argument1] + [Argument2] + [Argument3] + [Argument4] + [Newline Character]
/// The Newline character corresponds to 0x0A.
///
/// --- Reply to EC ---
/// If [Command] is G:
/// [Description] + [Hundredth] + [Tenth] + [Digit] + [Newline Character]
/// Where [Description] indicates the type of parameters being reported back.
/// or
/// ['b'] + 7 bytes of binary data.
/// Else:
/// "OK" or "NO", each string ends with [Newline Character]
/// 
/// 2. If there is no command from EC, this process will send a packet containing the current status
/// of the robotic platform to the EC at a predefined rate, between 4 Hz to 10 Hz.

void Robot_ExControllerMessageLoop(TASK_ATTRIBUTE *ptrTask)
{
    static unsigned int unRXTimer;
    static unsigned int unIdleTimer;
    static unsigned int unParam;
    int nIndex;
    static unsigned int unDigit;
    static unsigned int unTen;
    static unsigned int unHundred;
    static int nNextState;
    int nTemp, nTemp2;
    unsigned int unSum;
    static int  nSign;
    
    if (ptrTask->nTimer == 0)
    {
        switch (ptrTask->nState)
        {
            case 0: // State 0 - Initialization.
                unRXTimer = 0;                                                  // Initialize RX timeout timer to 0.
                unIdleTimer = 0;
                OSSetTaskContext(ptrTask, 1, 1000*__NUM_SYSTEMTICK_MSEC);        // Next state = 1, timer = 1000 msec  .
            break;
                         
            case 1: // State 1 - Check for incoming data.  Here this state also guarded against transaction
                    // that takes too long, e.g. timeout.
                if (gbytRXbufptr > 0)
                {
                    unRXTimer++;                                                // Increment RX timeout timer.
                    if (unRXTimer > (10*__NUM_SYSTEMTICK_MSEC))                 // Check for timeout, about 10 msec. Whenever a byte
                                                                                // of data is received, this timer will run.  If a 
                                                                                // full packet (of 5 bytes) is not received within the
                                                                                // stipulated period, timeout event is assumed.
                    {                                                           // If timeout occurred.
                        unRXTimer = 0;                                          // Reset RX timeout timer.
                        PIN_ILED2 = 0;                                          // Off indicator LED2.
                        gSCIstatus.bRXRDY = 0;                                  // Reset valid data flag.
                        gbytRXbufptr = 0;                                       // Reset pointer.   
                        gSCIstatus.bRXOVF = 0;                                  // Reset overflow error flag.                        
                        OSSetTaskContext(ptrTask, 1, 1);                        // Next state = 1, timer = 1.
                    }
                    else if (gbytRXbufptr > 5)                                  // Check if complete string of command is received. Expect
                    {                                                           // 6 bytes of incoming data packet.
                        if (gSCIstatus.bRXOVF == 0)                             // Make sure no overflow error.
                        {                     
                                                                                // Calculate checksum value from 1st 
                                                                                // 5 bytes. Use sum complement method.
                                                                                // Byte 6 is the checksum. So if we sum up 
                                                                                // all bytes we should get 0 for lower 8-bits.
                            unSum = gbytRXbuffer[0]+gbytRXbuffer[1]+gbytRXbuffer[2]+gbytRXbuffer[3]+gbytRXbuffer[4]+gbytRXbuffer[5];
                            unSum = unSum & 0x00FF;                             // Mask out all bits except lower 8-bits.
                            if (unSum == 0)                                     // All the bytes add up should give 0.
                            {
                                OSSetTaskContext(ptrTask, 2, 1);                    // Next state = 2, timer = 1.
                            }                               
                            else
                            {
                                OSSetTaskContext(ptrTask, 1, 1);                    // Next state = 1, timer = 1.
                            }
                        }
                        else
                        {
                       
                            gSCIstatus.bRXOVF = 0;                              // Reset overflow error flag.
                            OSSetTaskContext(ptrTask, 1, 1);                    // Next state = 1, timer = 1.
                        }
                        unRXTimer = 0;                                          // Reset RX timeout timer.
                        gSCIstatus.bRXRDY = 0;                                  // Reset valid data flag.
                        gbytRXbufptr = 0;                                       // Reset pointer, clear buffer.                           
                    }
                    else
                    {
                        OSSetTaskContext(ptrTask, 1, 1);                        // Next state = 1, timer = 1.
                    }
                        
                }
                else
                {
                    unIdleTimer++;                                              // Increment idle timer.
                    if (unIdleTimer > (200*__NUM_SYSTEMTICK_MSEC))              // Every 200 msec.
                    {
                        unIdleTimer = 0;                                        // Reset idle timer.
                        OSSetTaskContext(ptrTask, 50, 1);                       // Next state = 50, timer = 1.
                    }
                    else
                    {
                        OSSetTaskContext(ptrTask, 1, 1);                        // Next state = 1, timer = 1.
                    }
                }                
                break;        

            case 50: // State 50 - Send binary data packet periodically that contains the robot platform status.
                     // 
                     // Use checksum (sum complement method).
                     // Format:
                     // Byte0: 'b'
                     // Byte1-2: Right wheel angular velocity in (rev/sec)*100 + 1024.
                     // Byte3-4: Left wheel angular velocity in (rev/sec)*100 + 1024.
                     // Byte5-6: Heading in encoder ticks.
                     // Byte7-8: Reserved.
                     // Byte9-10: Right range sensor. Unsigned integer.
                     // Byte11-12: Left range sensor. Unsigned integer.
                     // Byte13-14: Front range sensor. Unsigned integer.
                     // Byte15: Checksum.
                
                nTemp = gobjQEOut.fOmegaRW * 100;                               // Convert float data to integer and add offset.
                nTemp = nTemp + 1024;
                nTemp2 = gobjQEOut.fOmegaLW * 100;                              // Convert float data to integer and add offset.
                nTemp2 = nTemp2 + 1024;
                gbytTXbuffer[0] = 'b';                                          // Indicating binary data packet.
                // Load distance traveled in encoder ticks.
                gbytTXbuffer[1] = nTemp >> 8;                                   // Get upper 8 bits of right wheel velocity in rev/sec.
                gbytTXbuffer[2] = nTemp;                                        // Get lower 8 bits of right wheel velocity.
                gbytTXbuffer[3] = nTemp2 >> 8;                                  // Get upper 8 bits of left wheel velocity in rev/sec.
                gbytTXbuffer[4] = nTemp2;                                       // Get lower 8 bits of left wheel velocity.                
                gbytTXbuffer[5] = gobjQEOut.nHeading >> 8;                      // Get upper 8 bits of heading in ticks. 
                gbytTXbuffer[6] = gobjQEOut.nHeading;                           // Get lower 8 bits of heading.
                gbytTXbuffer[7] = 0;                                            // Reserved.
                gbytTXbuffer[8] = 0;                                            // Reserved.
                gbytTXbuffer[9] = gobjRobotState.unSensorRangeR_mm>>8;
                gbytTXbuffer[10] = gobjRobotState.unSensorRangeR_mm;
                gbytTXbuffer[11] = gobjRobotState.unSensorRangeL_mm>>8;
                gbytTXbuffer[12] = gobjRobotState.unSensorRangeL_mm;
                gbytTXbuffer[13] = gobjRobotState.unSensorRangeF_mm>>8;
                gbytTXbuffer[14] = gobjRobotState.unSensorRangeF_mm;           
                                                                                // Compute checksum using sum complement
                                                                                // algorithm.
                unSum = gbytTXbuffer[0]+gbytTXbuffer[1]+gbytTXbuffer[2]+gbytTXbuffer[3]+gbytTXbuffer[4];
                unSum = unSum + gbytTXbuffer[5]+gbytTXbuffer[6]+gbytTXbuffer[7]+gbytTXbuffer[8]+gbytTXbuffer[9];
                unSum = unSum + gbytTXbuffer[10]+gbytTXbuffer[11]+gbytTXbuffer[12]+gbytTXbuffer[13]+gbytTXbuffer[14];
                unSum = ~unSum + 1;                                             // 2's complement. 
                gbytTXbuffer[15] = unSum & 0x00FF;                              // Only return the lower 8 bits.
                gbytTXbuflen = 16;                                              // Set TX frame length.
                gSCIstatus.bTXRDY = 1;                                          // Initiate TX.                
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);          // Next state = 1, timer = 2 msec.
                break;
                
            case 2: // State 2 - Decode message from External Controller (EC).  EC is connected to
                // UART2 port and each data packet is 5 bytes in length. 
                // The format is as follows:
                // Length = 6 bytes.
                // Command packet: [Command] + [Argument1] + [Argument2] + [Argument3] + + [Argument4] + [Newline Character]
                // The Newline character corresponds to 0x0A.
                
                switch(gbytRXbuffer[0])
                {       
                    // Command from EC.
                    // Byte 0 = Command.
                    // Byte 1 = Argument 1 for command.
                    // Byte 2 = Argument 2 for command.
                    // Byte 3 = Argument 3 for command.
                    // Byte 4 = Argument 4 for command.
                    // Byte 5 = Newline character.
                    case 'S': // Sound a beep. 
                              // Argument 1 = Tone value from 1 to 9.
                              // Argument 2 = Tone value from 1 to 9.
                              // Argument 3 = Tone value from 1 to 9.
                              // Argument 4 = Volume, from 0 to 5.
                        gnAudioTone[0] = gbytRXbuffer[1]-48;            // The argument is ASCII or UTF-8 character, '1' to '9'.
                                                                        // We thus need to convert to normal integer.            
                        if (gbytRXbuffer[2] > 48)
                        {
                            gnAudioTone[1] = gbytRXbuffer[2]-48;
                            if (gbytRXbuffer[3] > 48)
                            {
                                gnAudioTone[2] = gbytRXbuffer[3]-48;
                                gnAudioTone[3] = 0;
                            }
                            else
                            {
                                gnAudioTone[2] = 0; 
                            }
                        }
                        else
                        {
                            gnAudioTone[1] = 0;      
                        }
                        //gunAudioVolume = gbytRXbuffer2[4]-48;           // Set volume.
                        OSSetTaskContext(ptrTask, 100, 1);              // Next state = 100, timer = 1.
                        break;                               
                                    
                    case 'F': // Move forward or backward.
                              // Argument 1 = '-' (for backward) or any other character (forward).
                              // Argument 2 = Speed setting, from '1' to '4'.
                              // Argument 3 = Don't care, can set to '0'. 
                              // Argument 4 = Don't care, can set to '0'.
                            gnAudioTone[0] = 3;
                            gnAudioTone[1] = 0;  
                            nTemp = gbytRXbuffer[2]-48;                 // The argument is ASCII or UTF-8 character.  So
                                                                        // we turn it into numerics between 0-9.    
                            if (nTemp == 1)
                            {
                                gfOmegaWSet = _VELOCITY_MOVE_SLOW;
                            }
                            else if (nTemp == 2)
                            {
                                gfOmegaWSet = _VELOCITY_MOVE_NORMAL;
                            }
                            else if (nTemp == 3)
                            {
                                gfOmegaWSet = _VELOCITY_MOVE_FAST;
                            }
                            else if (nTemp == 4)
                            {
                                gfOmegaWSet = _VELOCITY_MOVE_VERYFAST;
                            }
                            else
                            {
                                OSSetTaskContext(ptrTask, 101, 1);      // Next state = 1, timer = 1.
                                break;
                            }                                
                            if (gbytRXbuffer[1] == '-')                 // Check if direction is backward.
                            {
                                gfOmegaWSet = -gfOmegaWSet;
                            }    
                            OSSetTaskContext(ptrTask, 100, 1);          // Next state = 100, timer = 1.
                        break; 

                    case 'T': // Turn, between -99 to +99 degree at 1 degree interval, or continuous turning
                              // at constant turning speed (if turning angle is > +99 or < -99 degrees).
                              // Positive angle turns the robot facing left, while negative angle turns
                              // the robot facing right.
                              // Argument 1 = '-' or any other character (will be interpreted as positive).
                              // Argument 2 = Hundredth value, Set to '0' for turn to fixed angle or 
                              //              '1' for continuous turning at default speed.
                              //              Any other value for continuous turning at high speed.
                              // Argument 3 = Tenth value, '0' to '9'.  Ignored for continuous turning.
                              // Argument 4 = Digit value, '0' to '9'.  Ignored for continuous turning.
                        gnAudioTone[0] = 7;
                        gnAudioTone[1] = 0;
                        nTemp = gbytRXbuffer[2]-48;                 // Get hundredth digit.
                                                                    // The argument is ASCII or UTF-8 character.  So
                                                                    // we turn it into numerics between 0-9.
                        if (nTemp > 9)                              // Limit to between 0-9.
                        {
                            OSSetTaskContext(ptrTask, 101, 1);      // Next state = 101, timer = 1.
                            break;
                        }                                    
                        if (nTemp == 0)                             // Check if equals '0'. Turn to 
                        {                                           // fixed heading mode.
                            nTemp2 = gbytRXbuffer[3]-48;            // Get the tenth digit.
                            if (nTemp2 > 9)                         // Limit to between 0-9.
                            {
                                OSSetTaskContext(ptrTask, 101, 1);  // Next state = 101, timer = 1.
                                break;
                            }                   
                            nTemp = nTemp2 * 10;                    // Convert to tenth.
                            nTemp2 = gbytRXbuffer[4]-48;            // Get unit digit.
                            if (nTemp2 > 9)                         // Limit to between 0-9.
                            {
                                OSSetTaskContext(ptrTask, 101, 1);  // Next state = 101, timer = 1.
                                break;
                            }                                     
                            nTemp = nTemp + nTemp2;                 // Compute the total angle in degrees.
                            if (gbytRXbuffer[1] == '-')             // Check direction.
                            {    
                                nSetMovementTurn(-nTemp);           // Execute turn right.
                            }
                            else
                            {
                                nSetMovementTurn(nTemp);            // Execute turn left.
                            }
                        }
                        else                                        // If hundredth digit is not '0',
                        {                                           // it is continuous turn mode.
                            //gnTurnMode = _TURN_MODE_CONT_TURN;
                            if (nTemp == 1)                         // Check to turn at high or low
                                                                    // angular velocity.
                            {                                       // Low angular velocity.
                                if (gbytRXbuffer[1] == '-')         // Check direction.
                                {   
                                    gnTurnSpeedSet = -2;    
                                }
                                else
                                {
                                    gnTurnSpeedSet = 2;
                                }
                            }
                            else
                            {                                       // High angular velocity.
                                if (gbytRXbuffer[1] == '-')             // Check direction.
                                {    
                                    gnTurnSpeedSet = -3;
                                }
                                else
                                {
                                    gnTurnSpeedSet = 3;
                                }                                    
                            }
                        }                                 
                        OSSetTaskContext(ptrTask, 100, 1);          // Next state = 100, timer = 1.
                        break;
                        
                    case 'X': // Command = Stop all movements.
                              // Argument 1 = Don't care, can set to '0'.
                              // Argument 2 = Don't care, can set to '0'.
                              // Argument 3 = Don't care, can set to '0'.
                              // Argument 4 = Don't care, can set to '0'.
                        gnAudioTone[0] = 1;
                        gnAudioTone[1] = 0;                          
                        //SetMovementStop();
                        nSetMovementTurn(0);
                        gfOmegaWSet = 0.0;
                        OSSetTaskContext(ptrTask, 100, 1);              // Next state = 100, timer = 1.
                        break;                              
                                    
                    case 'D': // Set RC Servo motor angle, differential mode.
                              // Argument 1 = Motor ID, from '0' to '7'.  Currently only 8 RC servo motors
                              //              are supported.
                              // Argument 2 = Motor output angle direction, '-' = negative, else positive. 
                              // Argument 3 = Differential motor output angle between 0-9.
                              // Argument 4 = don't care.
                        nTemp = gbytRXbuffer[3]-48;                     // The argument is ASCII or UTF-8 character.  So
                                                                        // we turn it into numerics between 0-9.
                        if (nTemp > 9)                                  // Limit to between 0-9.
                        {
                            nTemp = 9;
                        }                  
                        if (gbytRXbuffer[2] == 45)                      // If equal '-', negative.
                        {
                            nTemp = -nTemp;
                        }
                        nTemp2 = gbytRXbuffer[1] - 48;                  // 48 = '0', 49 = '1' and so on. So we are converting
                                                                        // the ASCII character back to actual integer value.
                        if (nTemp2 > 7)                                 // Check if the index to array is over limit.
                        {
                            OSSetTaskContext(ptrTask, 101, 1);          // Send NACK, Next state = 101, timer = 1.
                            break;
                        }
                        if (nTemp2 < 0)                                 // Check if the index to array is over limit.
                        {
                            OSSetTaskContext(ptrTask, 101, 1);          // Send NACK, Next state = 101, timer = 1.
                            break;
                        }                        
                        gnRCServoMotorAngleSet_Deg[nTemp2] = gnRCServoMotorAngleSet_Deg[nTemp2] + nTemp;     
                                                                        // Set corresponding RC servo motor angle in degrees.                        
                        OSSetTaskContext(ptrTask, 100, 1);              // Next state = 100, timer = 1.
                        break;
                        
                    case 'M': // Set RC Servo motor angle, absolute.
                              // Argument 1 = Motor ID, from '0' to '7'.  Currently only 8 RC servo motors
                              //              are supported.
                              // Argument 2 = Motor output angle direction, '-' = negative, else positive. 
                              // Argument 3,4 = Motor output angle from 0 to 99.
                        nTemp = gbytRXbuffer[3]-48;                     // The argument is ASCII or UTF-8 character.  So
                                                                        // we turn it into numerics between 0-9.
                        nTemp2 = gbytRXbuffer[4]-48;                    // The argument is ASCII or UTF-8 character.  So
                                                                        // we turn it into numerics between 0-9.
                        if ((nTemp > 9) || (nTemp2 > 9))                // Limit both characters to between 0-9.
                        {
                            OSSetTaskContext(ptrTask, 101, 1);          // Next state = 101, timer = 1.
                            break;
                        }                  
                        if (gbytRXbuffer[2] == 45)                      // If equal '-', negative.
                        {
                            nTemp = -(nTemp*10 + nTemp2);               // Combine both digits into a negative integer value.
                        }
                        else
                        {
                            nTemp = nTemp*10 + nTemp2;                  // Combine both digits into a positive integer value.
                                                                        // Store in nTemp.
                        }
                        
                        nTemp2 = gbytRXbuffer[1] - 48;                  // 48 = '0', 49 = '1' and so on. So we are converting
                                                                        // the ASCII character back to actual integer value.
                        if (nTemp2 > 7)                                 // Check if the index to array is over limit.
                        {
                            OSSetTaskContext(ptrTask, 101, 1);          // Send NACK, Next state = 101, timer = 1.
                            break;
                        }
                        if (nTemp2 < 0)                                 // Check if the index to array is over limit.
                        {
                            OSSetTaskContext(ptrTask, 101, 1);          // Send NACK, Next state = 101, timer = 1.
                            break;
                        }                        
                        gnRCServoMotorAngleSet_Deg[nTemp2] = nTemp;     // Set corresponding RC servo motor angle in degrees.
                        OSSetTaskContext(ptrTask, 100, 1);              // Send ACK, Next state = 100, timer = 1.
                        break;                        
                        
                    case 'G': // Command - Get parameters.
                        // Argument 1 = Type of parameters.
                        // Argument 2 = don't care or optional parameter.
                        // Argument 3 = don't care.
                        // Argument 4 = don't care.
                        if (gbytRXbuffer[1] == 'D')                     // If it is get distance sensor output.
                        {
                            OSSetTaskContext(ptrTask, 3, 1);            // Next state = 3, timer = 1.
                        }
                        else if (gbytRXbuffer[1] == 'B')                // If it is get battery level output. 
                        {
                            OSSetTaskContext(ptrTask, 5, 1);            // Next state = 5, timer = 1.
                        }
                        else if (gbytRXbuffer[1] == 'F')                // If it is the firmware version.
                        {
                            OSSetTaskContext(ptrTask, 10, 1);           // Next state = 10, timer = 1.
                        }
                        else if (gbytRXbuffer[1] == 'R')                // If it is get robot status.
                        {
                            OSSetTaskContext(ptrTask, 20, 1);           // Next state = 20, timer = 1.
                        }
                        else if (gbytRXbuffer[1] == 'V')                // Wheel velocity.
                        {
                            OSSetTaskContext(ptrTask, 25, 1);           // Next state = 25, timer = 1.
                        }
                        else if (gbytRXbuffer[1] == 'A')                // Tilt angle.
                        {
                            OSSetTaskContext(ptrTask, 27, 1);           // Next state = 27, timer = 1.
                        }     
                        else if (gbytRXbuffer[1] == 'b')                // Send robot status and parameters in binary format.
                        {
                            OSSetTaskContext(ptrTask, 30, 1);           // Next state = 30, timer = 1.
                        }                         
                        else
                        {
                            OSSetTaskContext(ptrTask, 101, 1);          // Next state = 101, timer = 1.
                        }
                        break;
                                        
                    default:
                        OSSetTaskContext(ptrTask, 1, 1);                // Next state = 1, timer = 1.
                        break;    
                }
            break;
                
            case 3: // State 3 - Send distance sensor output to remote controller, part 1.
                    // '0' = Right sensor 1.
                    // '1' = Left sensor 2.
                    // Else = Front sensor.
                if (gbytRXbuffer[2] == '0')
                {
                    unParam = gobjRobotState.unSensorRangeR_mm;               
                }
                else if (gbytRXbuffer[2] == '1')
                {
                    unParam = gobjRobotState.unSensorRangeL_mm;               
                }  
                else
                {
                    unParam = gobjRobotState.unSensorRangeF_mm;               
                }                    
                nNextState = 4;                                                 // After converting unParam to BCD, should return to state 4.
                OSSetTaskContext(ptrTask, 200, 1);                              // Next state = 200, timer = 1.
                break;  
                
            case 4: // State 4 - Send distance sensor output to remote controller, part 2.
                if (gSCIstatus.bTXRDY == 0)                                     // Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'D';                                      // Load data.
                    gbytTXbuffer[1] = unHundred + 0x30;                         // Convert number to ASCII.
                    gbytTXbuffer[2] = unTen + 0x30;                             // Convert number to ASCII.
                    gbytTXbuffer[3] = unDigit + 0x30;                           // Convert number to ASCII.
                    gbytTXbuffer[4] = 0x0A;                                     // Newline character.
                    gbytTXbuflen = 5;                                           // Set TX frame length.
                    gSCIstatus.bTXRDY = 1;                                     // Initiate TX.
                }
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);  // Next state = 1, timer = 2 msec.
                break;
                
            case 5: // State 5 - Send battery level output to External Controller, part 1.
                unParam = (gobjDriverADC.unADC3_mV) >> 3;                       // Divide by 8, as the ADC output is from 0 to 2047 (11 bits). 
                nNextState = 6;                                                 // After converting unParam to BCD, should return to state 6.
                OSSetTaskContext(ptrTask, 200, 1);                              // Next state = 200, timer = 1.
                break;
                
            case 6: // State 6 - Send battery level output to External Controller, part 2.
                if (gSCIstatus.bTXRDY == 0)	// Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'B';                                      // Load data.
                    gbytTXbuffer[1] = unHundred + 0x30;                         // Convert number to ASCII.
                    gbytTXbuffer[2] = unTen + 0x30;                             // Convert number to ASCII.
                    gbytTXbuffer[3] = unDigit + 0x30;                           // Convert number to ASCII.
                    gbytTXbuffer[4] = 0x0A;                                     // Newline character.
                    gbytTXbuflen = 5;                                           // Set TX frame length.
                    gSCIstatus.bTXRDY = 1;                                      // Initiate TX.
                }
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);  // Next state = 1, timer = 2 msec.
                break;
                
            case 10: // State 10 - Send firmware version to EC.
                if (gSCIstatus.bTXRDY == 0)                                     // Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'F';                                      // Load data.
                    gbytTXbuffer[1] = '0';                                      // Load data.
                    gbytTXbuffer[2] = '.';                                      // Load data.
                    gbytTXbuffer[3] = '0';                                      // Load data.
                    gbytTXbuffer[4] = '0';                                      // Load data.
                    gbytTXbuffer[5] = 0x0A;                                     // Newline character.                   
                    gbytTXbuflen = 6;                                           // Set TX frame length.
                    gSCIstatus.bTXRDY = 1;                                      // Initiate TX.
                }
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);          // Next state = 1, timer = 2 msec.
                break;            
                
            case 25: // State 25 - Send robot average wheel velocity to EC, part 1.
                     // Convert average wheel velocity to unsigned integer
                     //  1.0 revolution per second = 20 units.
                     //  0 = -5.0 revolution per second.
                     //  100 = 0 revolution per second.
                     //  200 = 5.0 revolution per second.
                     //  > 200 = Overflow or underflow.
                nSign = 1;                                                      // Default to positive sign.
                nTemp = gobjQEOut.fOmegaW*20;
                if (nTemp < 0)
                {
                    nTemp = -nTemp;
                    nSign = -1;                                                 // Set to negative sign.
                }
                unParam = nTemp;                                                                
                nNextState = 26;                                                // After converting unParam to BCD, should return to state 6.
                OSSetTaskContext(ptrTask, 200, 1);                              // Next state = 200, timer = 1.                
                break;
                
            case 26: // State 26 - Send robot average wheel velocity to EC, part 2.                 
                if (gSCIstatus.bTXRDY == 0)                                     // Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'V';                                      // Load data.
                    if (nSign > 0)
                    {
                        gbytTXbuffer[1] = '+';                                  // Set sign.
                    }
                    else
                    {
                        gbytTXbuffer[1] = '-';
                    }
                    gbytTXbuffer[2] = unTen + 0x30;                             // Convert number to ASCII.
                    gbytTXbuffer[3] = unDigit + 0x30;                           // Convert number to ASCII.
                    gbytTXbuffer[4] = 0x0A;                                     // Newline character.
                    gbytTXbuflen = 5;                                           // Set TX frame length.
                    gSCIstatus.bTXRDY = 1;                                      // Initiate TX.
                }
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);          // Next state = 1, timer = 2 msec.                
                break;
                
            case 27: // State 27 - Send robot tilt angle to EC, part 1.
                nSign = 1;                                                      // Default to positive sign.
                nTemp = gnTheta_Deg_MPU6050;
                if (nTemp < 0)
                {
                    nTemp = -nTemp;
                    nSign = -1;                                                 // Set to negative sign.
                }
                unParam = nTemp;                                                                
                nNextState = 28;                                                // After converting unParam to BCD, should return to state 6.
                OSSetTaskContext(ptrTask, 200, 1);                              // Next state = 200, timer = 1.                 
                break;
                
            case 28: // State 28 - Send robot tilt angle to EC, part 2.              
                if (gSCIstatus.bTXRDY == 0)                                     // Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'A';                                      // Load data.
                    if (nSign > 0)
                    {
                        gbytTXbuffer[1] = '+';                                  // Set sign.
                    }
                    else
                    {
                        gbytTXbuffer[1] = '-';
                    }
                    gbytTXbuffer[2] = unTen + 0x30;                             // Convert number to ASCII.
                    gbytTXbuffer[3] = unDigit + 0x30;                           // Convert number to ASCII.
                    gbytTXbuffer[4] = 0x0A;                                     // Newline character.
                    gbytTXbuflen = 5;                                           // Set TX frame length.
                    gSCIstatus.bTXRDY = 1;                                      // Initiate TX.
                }
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);  // Next state = 1, timer = 2 msec.                
                break;
                
            case 30: // State 30 - Send back robot status and parameters in binary format.  The return packet consists of 8
                     // bytes, with a header and 7 bytes of payload.  The length payload can be updated in future.
                if (gSCIstatus.bTXRDY == 0)                                     // Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'b';                                      // Indicate all parameters in binary.
                    nTemp = 1 & 0x0F;                                 // bit3-0 represents the Robot Mode.
                                                                                // Bit4 indicates whether Robot Controller
                                                                                // is in manual (0) or auto mode (1).
                                                                                // Bit 5 indicates whether Robot platform collides
                                                                                // with object or not (based on stability indices
                                                                                // signature).
                    //if (gnRobotMoveMode == _ROBOT_MOVE_AUTO)                    // Set robot move mode.
                    //{
                    //    nTemp = nTemp | 0x10;                                   // Set bit4.
                    //}
                    //if (gobjRobotFlag.bCOLLISION != 0)
                    //{
                    //    nTemp = nTemp | 0x20;                                   // Set bit5.
                    //    gobjRobotFlag.bCOLLISION = 0;                           // Clear flag.
                    //}
                    gbytTXbuffer[1] = nTemp;                                    // Set robot controller status.
                    gbytTXbuffer[2] = gnTheta_Deg_MPU6050 + 127;                // Get tilt angle, offset by 127.
                    gbytTXbuffer[3] = gobjQEOut.nHeading>>8;                    // Get upper 8 bits of heading. 
                    gbytTXbuffer[4] = gobjQEOut.nHeading;                       // Get lower 8 bits of heading
                    gbytTXbuffer[5] = gobjRobotState.unSensorRangeF_mm;         // Get the front distance sensor output, unit in mm.
                    gbytTXbuffer[6] = gobjQEOut.nDistanceMoveW>>8;              // Get upper 8 bits of distance moved.
                    gbytTXbuffer[7] = gobjQEOut.nDistanceMoveW;                 // Get lower 8 bits of distance moved.
                    gbytTXbuflen = 8;		// Set TX frame length.
                    gSCIstatus.bTXRDY = 1;	// Initiate TX.
                }
                OSSetTaskContext(ptrTask, 1, 5*__NUM_SYSTEMTICK_MSEC);  // Next state = 1, timer = 5 msec.                    
                break;
               
               
            case 100: // State 100 - Send acknowledgement to EC.
                /*
                if (gSCIstatus.bTXRDY == 0)	// Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'O';	// Load data.
                    gbytTXbuffer[1] = 'K';	// Load data.
                    gbytTXbuffer[2] = 0x0A; // Newline character.                   
                    gbytTXbuflen = 3;		// Set TX frame length.
                    gSCIstatus.bTXRDY = 1;	// Initiate TX.
                } */
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);  // Next state = 1, timer = 2 msec.
                break;  
                
            case 101: // State 100 - Send non-acknowledgement to EC.
                if (gSCIstatus.bTXRDY == 0)	// Check if any data to send via UART.
                {
                    gbytTXbuffer[0] = 'N';	// Load data.
                    gbytTXbuffer[1] = 'O';	// Load data.
                    gbytTXbuffer[2] = 0x0A; // Newline character.                   
                    gbytTXbuflen = 3;		// Set TX frame length.
                    gSCIstatus.bTXRDY = 1;	// Initiate TX.
                }
                OSSetTaskContext(ptrTask, 1, 2*__NUM_SYSTEMTICK_MSEC);  // Next state = 1, timer = 2 msec.
                break;                   
                
            case 200: // State 200 - Using the double-dabble method (also known as shift and add 3) 
                      // to convert 9-bits unsigned integer to 3 digits BCD.  The unsigned integer 
                      // value to convert is unParam.
                unDigit = 0;
                unTen = 0;
                unHundred = 0;
                for (nIndex = 0; nIndex < 9; nIndex++) // 9-bits unsigned integer.
                {
                    if (unHundred > 4)
                    {
                        unHundred = unHundred + 3;
                    }
                    if (unTen > 4)
                    {
                        unTen = unTen + 3;
                    }
                    if (unDigit > 4)
                    {
                        unDigit = unDigit + 3;
                    }                    
                    unHundred = unHundred<<1;
                    // Add bit 3 of unTen to unHundred.
                    if ((unTen & 0x0008)>0)
                    {
                        unHundred++;
                    }
                    unTen = (unTen<<1) & 0x000F;  // Shift and only retain lower 4 bits.
                    // Add bit 3 of unTen to unHundred.
                    if ((unDigit & 0x0008)>0)
                    {
                        unTen++;
                    }        
                    unDigit = (unDigit<<1) & 0x000F;  // Shift and only retain lower 4 bits.
                    // Add bit 8 of parameter to unDigit.
                    if ((unParam & 0x0100)>0)
                    {
                        unDigit++;
                    }                    
                    unParam = (unParam<<1) & 0x01FF;                   
                }            
                OSSetTaskContext(ptrTask, nNextState, 1);  // Next state = nNextState, timer = 1.
                break;
                
            default:
                gunOSError = gunOSError | 0x0001;   // Set OS error bit 0.
                OSSetTaskContext(ptrTask, 0, 1); // Back to state = 0, timer = 1.
            break;
        }
    }
}
