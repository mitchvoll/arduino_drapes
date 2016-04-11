/*
 *
 * ARDUINO SMART DRAPES 
 * Mitch Vollebregt 2016
 *
 */

#include <IRremote.h>
#include <Servo.h>

// constants
#define RECV_PIN 11
#define SERVO_PIN 9

#define MAX_CC 135 // max servo speed open
#define MAX_C 45 // max servo speed closed
#define STOPPED_MICRO 1473 // servo position idle
#define STOPPED_MICRO 1473 // servo position idle

#define IR_OPEN 18615 // IR ON/OFF button toggles drapes open
#define IR_RIGHT -19891 // IR RIGHT ARROW button toggles small open 
#define IR_LEFT -30091 // IR LEFT ARROW button toggles small close 


// globals
int drapes_state = 0; // drapes state intialize to closed
IRrecv irrecv(RECV_PIN);
decode_results results;
Servo myservo;


void setup() {
    Serial.begin(9600);
//  myservo.write(STOPPED); // write 90 to the servo for no rotation
    myservo.writeMicroseconds(STOPPED_MICRO);

    irrecv.enableIRIn(); // Start the receiver

    pinMode(13, OUTPUT); // attach led to pin 13

    delay(2000);
    Serial.println("starting program");
}


void move_servo(int spd, int dist) {
    // servo has the tendency to creep so we attach the servo when
    // we need to use it and detach afterwords. 
    myservo.attach(SERVO_PIN); //  attach servo to digital pin 9
    String debug = "moving to dist " + dist + " at speed " + spd;
    Serial.print(debug);

    myservo.write(spd); // turn servo direction of spd at rate of spd
    delay(dist); // delay for dist amount of time

    myservo.writeMicroseconds(STOPPED_MICRO);
    myservo.detach(); // detach servo to prevent creeping  
    Serial.println("done moving");
}

void toggle_drapes(int ir_signal) {
//  Serial.print("received value: ");
//  Serial.println(ir_signal);
    
    if (ir_signal == IR_OPEN) { // remote ON/OFF button
        if (drapes_state == 0) { // open drapes      
            Serial.println("opening drapes");
            move_servo(MAX_CC, 4000); // move max counter-clockwise speed for 2 seconds
            drapes_state = 1; // set drapes to open
        }
        else if (drapes_state == 1) { // close drapes       
            Serial.println("closing drapes");
            move_servo(MAX_C, 2800); // move max clockwise speed for 2 seconds
            drapes_state = 0; // set drapes to closed
        }
    }
    else if (ir_signal == IR_RIGHT){ // remote right arrow button
        move_servo(MAX_CC, 500);
    }
    else if (ir_signal == IR_LEFT){ // remote left arrow button
        move_servo(MAX_C, 500);
    }
}

void loop() {
    if (irrecv.decode(&results)) {
        // Serial.println(results.value);
        toggle_drapes(results.value);
        irrecv.resume(); // Receive the next value
    }
    
    if (drapes_state == 1) // turn LED on if drapes are open
        digitalWrite(13, HIGH);
    else // turn LED off if drapes are closed
        digitalWrite(13, LOW);

    delay(100);
}
