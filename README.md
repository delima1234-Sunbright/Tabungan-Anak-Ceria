# Tabungan Anak Ceria (TAC)

## Overview
Tabungan Anak Ceria (TAC) is a savings product designed specifically for children aged 5 to 12 years. This project aims to encourage children to develop a habit of saving by incorporating interactive and engaging features such as setting savings goals, audio notifications, and a buzzer to celebrate when targets are achieved. The system is built using an Arduino microcontroller and is a collaborative effort between the Computer Systems Engineering and Product Design Engineering departments.

## Features
- **Savings Goal Setting**: Allows children to set a savings target.
- **Interactive Menu**: Navigate through options like setting a target, saving money, checking balance, withdrawing savings, and changing the password.
- **Audio Feedback**: Plays melodies via a buzzer to notify users when a target is set, money is saved, or a goal is reached.
- **Password Protection**: Secures withdrawals with a 4-digit password stored in EEPROM, with the ability to change it.
- **LCD Display**: A 16x2 LCD screen displays menus, prompts, and status updates.
- **Proximity Sensor**: Detects when money is inserted to initiate the saving process.
- **Solenoid Lock**: Controls access to the savings compartment, unlocking only when the correct password is entered or the savings goal is met.

## Hardware Requirements
- Arduino Microcontroller (e.g., Arduino Uno)
- 16x2 LCD with I2C interface
- 4x4 Keypad
- Buzzer
- Solenoid Lock
- Proximity Sensor
- Servo Motor (optional, if used for mechanical components)
- EEPROM (onboard Arduino)
- Connecting wires and breadboard

## Software Requirements
- Arduino IDE
- Required Libraries:
  - `Wire.h`
  - `LiquidCrystal_I2C.h`
  - `Keypad.h`
  - `EEPROM.h`
  - `Servo.h`
  - Custom `pitches.h` for buzzer melodies (included in the repository)

## Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/delima1234-Sunbright/Tabungan-Anak-Ceria.git
   ```
2. **Install Libraries**:
   - Install the required libraries in the Arduino IDE via the Library Manager or manually add them to the Arduino libraries folder.
3. **Upload Code**:
   - Open the `.ino` file in the Arduino IDE.
   - Connect the Arduino board to your computer.
   - Upload the code to the Arduino.
4. **Connect Hardware**:
   - Wire the components according to the pin assignments in the code:
     - Buzzer: Pin 2
     - Solenoid: Pin 3
     - Proximity Sensor: Pin 4
     - Keypad: Pins 6, 7, 8, 9 (columns), 10, 11, 12, 13 (rows)
     - LCD: I2C address 0x27
   - Ensure proper connections for the servo (if used) and other components.

## Usage
1. **Power On**:
   - The system boots up, displaying "Welcome To TAC" and playing a startup melody.
2. **Menu Navigation**:
   - Use the keypad to navigate:
     - `A`: Move to the next menu.
     - `B`: Move to the previous menu.
     - `1` to `5`: Select a menu option (Set Target, Save, Check Balance, Withdraw, Change Password).
     - `*`: Confirm/Enter.
     - `#`: Delete last input or cancel.
3. **Set Target**:
   - Select option 1 to set a savings goal.
   - Input the target amount using the keypad and confirm with `*`.
4. **Save Money**:
   - Select option 2 (available after setting a target).
   - Insert money (detected by the proximity sensor).
   - Input the amount saved and confirm with `*`.
   - If the savings reach or exceed the target, a celebratory melody plays, and the solenoid unlocks.
5. **Check Balance**:
   - Select option 3 to view the current savings and target.
6. **Withdraw Savings**:
   - Select option 4 and confirm with `*`.
   - Enter the 4-digit password.
   - If correct, the solenoid unlocks to allow withdrawal, and the system resets.
7. **Change Password**:
   - Select option 5 and enter the current password.
   - If correct, input a new 4-digit password, which is saved to EEPROM.

## Code Structure
- **Main File**: Contains the core logic for menu navigation, input handling, and hardware control.
- **pitches.h**: Defines musical note frequencies for the buzzer melodies.
- **Functions**:
  - `setup()`: Initializes hardware and displays the welcome message.
  - `loop()`: Handles keypad input and menu selection.
  - `displayMenu()`: Updates the LCD with menu options.
  - `setmax()`: Sets the savings target.
  - `nabungLoop()`: Manages the saving process.
  - `saldo()`: Displays the current balance and target.
  - `tariktabungan()`: Handles withdrawal with password verification.
  - `Password()`: Manages password input and verification.
  - `changePassword()`: Updates the stored password.
  - `playMelody()`, `playMelody1()`, `playTone()`: Control buzzer sounds.
  - `openDoor()`, `closeDoor()`: Control the solenoid lock.
  - `resetSystem()`: Resets all variables and system state.

## Notes
- The default password is `1234` if EEPROM is uninitialized.
- Ensure the proximity sensor is calibrated to detect money insertion accurately.
- The solenoid lock is active-high (HIGH to unlock, LOW to lock).
- The system resets automatically after a successful withdrawal or when the savings goal is met.

## Teams 
Developed by the Computer Systems Engineering and Product Design Engineering teams.
- Delima Ester Purba - Computer Systems Engineering - Universitas Prasetiya Mulya 
- Zaky Alfarizi - Computer Systems Engineering - Universitas Prasetiya Mulya 
- Amrina Rosyada - Product Design Engineering - Universitas Prasetiya Mulya 
- Chui Nyiun - Product Design Engineering - Universitas Prasetiya Mulya 
