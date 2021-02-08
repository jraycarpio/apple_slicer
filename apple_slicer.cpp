/*
  
  Apple Slicer
  By: Jonathan Carpio
  
*/



#include <Stepper.h>



const int STEPS_PER_REV = 200;
const int upButton = 13;
const int downButton = 12;
const int quitButton = 7;
int upState;
int downState;
int quitState;
int motorSpeed;
int ans;
int pos;
int upDown;              // Moving up = 1, Moving down = 2 
int m1_steps;
int m2_steps;
int desired_steps;
int vDir;                // Vertical direction multiplier
boolean done;

// Define stepper motors
Stepper motor1(STEPS_PER_REV, 8, 9, 10, 11);
Stepper motor2(STEPS_PER_REV, 2, 3, 5, 6);

// Define functions

// Wait for user input thru monitor
int userInput() {
  while (Serial.available() == 0) {}
  String input = Serial.readString();
  return input.toInt();
}

// Move motors for some steps x, and record position
void moveX(int steps) {

  if (upDown == 1) {
    for (int i = 1; i <= steps; i++) {
      motor1.step(vDir);
      motor2.step(vDir);
      pos = pos + 1;
      Serial.print("POSITION = ");
      Serial.println(pos);
    }
    
  } else {
    for (int i = 1; i <= steps; i++) {
      motor1.step(vDir * (-1));
      motor2.step(vDir * (-1));
      pos = pos - 1;
      Serial.print("POSITION = ");
      Serial.println(pos);
    }
  
  }

  Serial.println();
  
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n\nBREAK ----");
  Serial.println("BREAK ----");
  Serial.println("BREAK ----");
  Serial.println("BREAK ----");
  Serial.println();
  Serial.println();
  Serial.println();

  delay(500);
  
  Serial.println("SYSTEM START\n");
  
  // Initialize default parameters
  motorSpeed = 100;
  pos = 0;
  vDir = 1;
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(quitButton, INPUT);

  delay(500);
}

void loop() {

  Serial.print("Current motor speed = ");
  Serial.println(motorSpeed);
  Serial.print("Current position = ");
  Serial.println(pos);
  Serial.println();
  
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);

  // Select action
  Serial.println("\nWhat would you like to do?");
  Serial.println("1) Change motor speed");
  Serial.println("2) Move for some x steps");
  Serial.println("3) Control motors manually with buttons");
  Serial.println("4) Redefine up/down direction");
  Serial.println("5) Zero position");
  Serial.println("6) Move to specified position");

  Serial.println("\n");

  ans = userInput();

  // Option 1: Change motor speed
  if (ans == 1) {
    Serial.println("Change motor speed. Define new speed: ");
    motorSpeed = userInput();
    Serial.print("\nNew motor speed is ");
    Serial.println(motorSpeed);
    Serial.println();
  }

  // Option 2: Move for some x steps
  else if (ans == 2) {
    Serial.println("Specify an amount of steps you would like to travel:");
    desired_steps = userInput();
    
    Serial.println("\nChoose direction of travel:");
    Serial.println("1) Up");
    Serial.println("2) Down");
    upDown = userInput();
    
    if (upDown == 1) {
      Serial.print("Moving up ");
      Serial.print(desired_steps);
      Serial.print(" steps.");
      moveX(desired_steps);
    }
    else {
      Serial.print("Moving down ");
      Serial.print(desired_steps);
      Serial.print(" steps.");
      moveX(desired_steps);
    }
  }

  // Option 3: Manual control of motors
  else if (ans == 3) {
    Serial.println("Please use the buttons to control the motors or quit this mode.");
    Serial.println("Button 1: up");
    Serial.println("Button 2: down");
    Serial.println("Button 3: quit");

    done = false;

    while (done == false) {
      upState = digitalRead(upButton);
      downState = digitalRead(downButton);
      quitState = digitalRead(quitButton);
      
      if (upState == HIGH) {
        upDown = 1;
        Serial.println("UP BUTTON PRESSED");
        moveX(100);
      }

      if (downState == HIGH) {
        upDown = 2;
        Serial.println("DOWN BUTTON PRESSED");
        moveX(100);
      }

      if (quitState == HIGH) {
        Serial.println("QUIT BUTTON PRESSED");
        done = true;
      }
    }
  }

  // Option 4: Redefine up/down direction
  else if (ans == 4) {
    done = false;

    while (done == false) {
      Serial.println("Choose an option below:");
      Serial.println("1) Test up direction for 100 steps");
      Serial.println("2) Test down direction for 100 steps");
      Serial.println("3) Switch current up/down direction to opposite direction");
      Serial.println("4) Quit");
      ans = userInput();

      if (ans == 1) {
        upDown = 1;
        moveX(5);
      }
      else if (ans == 2) {
        upDown = 2;
        moveX(5);
      }
      else if (ans == 3) {
        vDir = vDir * (-1);
        Serial.print("New vertical direction multiplier is now ");
        Serial.println(vDir);
      }
      else {
        done = true;
      }
    }
  }

  // Option 5: Zero position
  else if (ans == 5) {
    Serial.println("Please choose an option below to confirm that you would like to zero this current position.");
    Serial.println("1) Yes, zero this position");
    Serial.println("2) No, do not zero this position");
    ans = userInput();

    if (ans == 1) {
      Serial.println("Zeroing position...");
      pos = 0;
    }
    else {
      Serial.println("Not zeroing position.");
    }
  }

  // Option 6: Move to specific position
  else if (ans == 6) {
    Serial.println("Please specify a position you would like to move to:");
    ans = userInput();
    ans = ans - pos;

    if (ans > 0) {
      upDown = 1;
      moveX(ans);
    }
    else {
      upDown = 2;
      ans = ans * (-1);
      moveX(ans);
    }
  }




  
}
