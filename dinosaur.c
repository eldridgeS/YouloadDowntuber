#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#include <ADCDevice.hpp>

#define OFFSET_MS 3
#define SERVO_MIN_MS 5+OFFSET_MS  
#define SERVO_MAX_MS 10+OFFSET_MS
#define servoPin 1

ADCDevice *adc;  // Define an ADC Device class object

long map(long value,long fromLow,long fromHigh,long toLow,long toHigh){
    return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
}
void servoInit(int pin){        //initialization function for servo PMW pin
    softPwmCreate(pin,  0, 200);
}
void servoWrite(int pin, int angle){    //Specific a certain rotation angle (0-45) for the servo NOT IN USE
    if(angle > 45)
        angle = 45;
    if(angle < 0)
        angle = 0;
    softPwmWrite(pin,map(angle,0,180,SERVO_MIN_MS,SERVO_MAX_MS));   
}
void servoWriteMS(int pin, int ms){     //specific the unit for pulse(5-25ms) with specific duration output by servo pin: 0.1ms
    if(ms > SERVO_MAX_MS)
        ms = SERVO_MAX_MS;
    if(ms < SERVO_MIN_MS)
        ms = SERVO_MIN_MS;
    softPwmWrite(pin,ms);
}

int main(void){
    int i;
    adc = new ADCDevice();
    printf("Program is starting ... \n");
    
    if(adc->detectI2C(0x48)){    // Detect the pcf8591.
        delete adc;              // Free previously pointed memory
        adc = new PCF8591();     // If detected, create an instance of PCF8591.
    }
    else if(adc->detectI2C(0x4b)){// Detect the ads7830
        delete adc;               // Free previously pointed memory
        adc = new ADS7830();      // If detected, create an instance of ADS7830.
    }
    else{
        printf("No correct I2C address found, \n"
        "Please use command 'i2cdetect -y 1' to check the I2C address! \n"
        "Program Exit. \n");
        return -1;
    } 
    wiringPiSetup();    
    servoInit (servoPin);
    while(1){
        int value = adc->analogRead(0);  //read analog value of A0 pin
        if (value > 105)
        {
            printf("\nADC value : %d ",value);
            for(i=SERVO_MIN_MS;i<SERVO_MAX_MS;i++){  //make servo rotate from minimum angle to maximum angle
            servoWriteMS(servoPin,i);
            
        }
        delay(500);
        for(i=SERVO_MAX_MS;i>SERVO_MIN_MS;i--){  //make servo rotate from maximum angle to minimum angle
            servoWriteMS(servoPin,i);
           
        }
        
        continue;
        }
        else
        {
            continue;
        } 
    delay(100);
    return 0;
}
}

// Enter below in terminal
// cd ~/projects/dinosaur - file directory
// g++ dinosaur.c -o Dinosaur -lwiringPi -lADCDevice - compile with macros
// sudo ./Dinosaur - run code


