#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h>
#include "pitches.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define buzzer 2
int solenoid = 3;

int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};
int failureMelody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int failureNoteDurations[] = {
  250, 125, 125, 250, 250, 250, 250, 250
};
// Durasi setiap nada (4 = seperempat nada, 8 = seperdelapan nada, dll)
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2
};

int melody1[] = {
  NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G4,
  NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4
};

int durations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 2, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 2
};

char customKey;
const byte ROWS = 4;
const byte COLS = 4;
const int SensorProximity = 4;
char password[5];  // Tambahkan satu elemen lagi untuk null-termination
char pass[5], pass1[5];
int i = 0;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};

int x = 0;
unsigned long nilaisetmax = 0;
unsigned long nilai = 0;
unsigned long saldox = 0;
unsigned long tabungan = 0;
String input = "";
bool maxSet = false;

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo myServo;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome To TAC");
  lcd.setCursor(0, 1);
  lcd.print("Tabungan Anak Ceria");
  playMelody(); // Nada ketika LCD menyala
  delay(3000);
  lcd.clear();
  displayMenu();
  pinMode(SensorProximity, INPUT); // Initialize proximity sensor pin as input
  pinMode(buzzer, OUTPUT);         // Initialize buzzer pin as output
  pinMode(solenoid, OUTPUT);          
  digitalWrite(solenoid,HIGH);
}

void loop() {
  customKey = customKeypad.getKey();
  if (customKey) {
    beep();
    handleMenuSelection();
  }
}

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (x == 0) {
    if (!maxSet) {
      lcd.setCursor(0, 0);
      lcd.print("     MENU : ");
      lcd.setCursor(0, 1);
      lcd.print("   1.Target      ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("PILIHAN : ");
      lcd.setCursor(0, 1);
      lcd.print("1.Target (Done)");
    }
  } else if (x == 1) {
    lcd.setCursor(0, 0);
    lcd.print("2.Nabung       ");
    lcd.setCursor(0, 1);
    lcd.print("3.Cek Saldo");
  } else if (x == 2) {
    lcd.setCursor(0, 0);
    lcd.print("4.Tarik Uang");
    lcd.setCursor(0, 1);
    lcd.print("5.Atur Password");
  }
}

void handleMenuSelection() {
  switch (customKey) {
    case 'A':
      x++;
      break;
    case 'B':
      x--;
      break;
    case 'D':
      playMelody1();
    case '1':
      if (!maxSet) {
        setmax();
      }
      break;
    case '2':
      if (maxSet) {
        nabungLoop();
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tentukan target dulu!");
        delay(2000);
        displayMenu();
      }
      break;
    case '3':
      saldo();
      break;
    case '4':
      tariktabungan();
      break;
    case '5':
      Password();
      break;
    case '#': // Tombol Enter
      if (x == 0 && !maxSet) {
        setmax();
      }
      if (x == 1) {
        if (maxSet) {
          nabungLoop();
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tentukan target dulu!");
          delay(2000);
          displayMenu();
        }
      }
      if (x == 2) {
        saldo();
      }
      break;
  }

  if (x > 2) {
    x = 0;
  }
  if (x < 0) {
    x = 2;
  }

  displayMenu();
}

void setmax() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Target: ");
  digitalWrite(solenoid,HIGH);

  nilai = 0;
  input = "";

  while (true) {
    customKey = customKeypad.getKey();

    if (customKey) {
      beep();
    }

    if (customKey >= '0' && customKey <= '9') {
      input += customKey;
      nilai = input.toInt();
      lcd.setCursor(0, 1);
      lcd.print("Berapa: ");
      lcd.print(nilai);
    }

    if (customKey == '*') { // Tombol enter
      nilaisetmax = nilai;
      maxSet = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Yeaay , target berhasil di set");
      playTone(1000, 200); // Nada ketika target disetel
      delay(2000);
      displayMenu();
      return;
    }
    if (customKey == '#') {
      deleteLastCharacter();
    }
  }
}

void nabungLoop() {
  while (true) {
    int hasil = digitalRead(SensorProximity);
    if (hasil == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Masukkan uang ");
      lcd.setCursor(0, 1);
      lcd.print("   dulu ya  ");
      delay(1000);
    } else if (hasil == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mau nabung berapa?");
      tabungan = 0;
      input = "";
      while (true) {
        customKey = customKeypad.getKey();
        if (customKey) {
          beep();
        }
        if (customKey >= '0' && customKey <= '9') {
          input += customKey;
          tabungan = input.toInt();
          lcd.setCursor(0, 1);
          lcd.print("Input: ");
          lcd.print(tabungan);
        }
        if (customKey == '*') {
          saldox += tabungan;
          playTone(1500, 200); // Nada ketika uang berhasil ditambahkan
          lcd.clear();
          if (saldox >= nilaisetmax) {
            lcd.setCursor(0, 0);
            lcd.print("Horee, Kamu berhasil!");
            lcd.setCursor(0, 1);
            lcd.print("Silahkan ambil uangmu");
            playMelody(); // Nada ketika target tercapai
            delay(3000);
            openDoor();
            delay(3000);  // Keep door open for 3 seconds
            closeDoor();
            resetSystem();
          } else {
            lcd.setCursor(0, 0);
            lcd.print("Uang sudah !");
            lcd.setCursor(0, 1);
            lcd.print("ditabung!");
            delay(2000);
          }
          displayMenu();
          return;
        }
        if (customKey == '#') {
          deleteLastCharacter();
        }
      }
    }
  }
}

void Password(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Input Password:");
  lcd.setCursor(0,1);
   if (EEPROM.read(0) == 0xFF) { // Asumsikan EEPROM tidak terinisialisasi jika byte pertama adalah 0xFF
    for (int j = 0; j < 4; j++)
      EEPROM.write(j, j + 49); // Password default '1234'
  }
  for (int j = 0; j < 4; j++)
    pass[j] = EEPROM.read(j);
  pass[4] = '\0'; // Null-terminate string password

  while (true) {
    customKey = customKeypad.getKey();
    if (customKey) {
      beep();
    }
    if (customKey == 'C') {
      changePassword();
      return;  // Tambahkan return untuk keluar dari loop setelah mengganti password
    }
    if (customKey == '#') {
      deleteLastCharacter();
    }
    if (customKey && i < 4) {
      password[i++] = customKey;
      lcd.print(customKey);
    }
    if (i == 4) {
      password[4] = '\0'; // Null-terminate password yang dimasukkan
      delay(200);

      if (strncmp(password, pass, 4) == 0) {
        //digitalWrite(solenoid, HIGH);
        lcd.clear();
        lcd.print("Password diterima");
        delay(3000);
        lcd.setCursor(0, 1);
        lcd.print("C.Ubah Password");
      } else {
        //digitalWrite(solenoid,LOW);
        lcd.clear();
        lcd.print("Password ditolak");
        lcd.setCursor(0, 1);
        lcd.print("C.Ubah Password");
        delay(2000);
        lcd.clear();
        lcd.print("Input Password:");
        lcd.setCursor(0, 1);
      }

      i = 0; // Reset indeks
      memset(password, 0, sizeof(password)); // Bersihkan buffer password
      //digitalWrite(solenoid, LOW);
    }
    if (customKey == '*') {
      lcd.clear();
      displayMenu();
      return;
    }
  }
}

void changePassword() {
  int j = 0;
  lcd.clear();
  lcd.print("Past Password :");
  lcd.setCursor(0, 1);

  while (j < 4) {
    char key = customKeypad.getKey();
    if (key) {
      beep();
      pass1[j++] = key;
      lcd.print(key);
    }
  }
  pass1[4] = '\0'; // Null-terminate password lama

  delay(500);

  if (strncmp(pass1, pass, 4) != 0) {
    lcd.clear();
    lcd.print("Password salah");
    lcd.setCursor(0, 1);
    lcd.print("Coba lagi");
    delay(1000);
  } else {
    j = 0;
    lcd.clear();
    lcd.print("Password Baru  :");
    lcd.setCursor(0, 1);

    while (j < 4) {
      char key = customKeypad.getKey();
      if (key) {
        beep();
        pass[j] = key;
        lcd.print(key);
        EEPROM.write(j, key);
        j++;
      }
    }
    pass[4] = '\0'; // Null-terminate password baru
    lcd.print("  Done");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Input Password:");
  lcd.setCursor(0, 1);
  customKey = 0;
}

void saldo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Target=");
  lcd.print(nilaisetmax);

  lcd.setCursor(0, 1);
  lcd.print("Saldo=");
  lcd.print(saldox);

  while (true) {
    customKey = customKeypad.getKey();
    if (customKey) {
      beep();
    }
    if (customKey == '*') {
      lcd.clear();
      displayMenu();
      return;
    }
  }
}

void deleteLastCharacter() {
  if (input.length() > 0) {
    input = input.substring(0, input.length() - 1);
    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear the line
    lcd.setCursor(0, 1);
    lcd.print("Input: ");
    lcd.print(input);
    nilai = input.toInt();
  }
}

void tariktabungan() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Apa kamu yakin? ");
  lcd.setCursor(0, 1);
  lcd.print("*:Ya      #:Tidak");

  while (true) {
    customKey = customKeypad.getKey();
    if (customKey) {
      beep();
    }
    if (customKey == '*') { // Jika 'Ya'
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Input Password:");
      lcd.setCursor(0, 1);

      if (EEPROM.read(0) == 0xFF) {
        for (int j = 0; j < 4; j++)
          EEPROM.write(j, j + 49); // Password default '1234'
      }
      for (int j = 0; j < 4; j++)
        pass[j] = EEPROM.read(j);
      pass[4] = '\0'; // Null-terminate string password

      while (true) {
        customKey = customKeypad.getKey();
        if (customKey) {
          beep();
        }
        if (customKey == '#') {
          deleteLastCharacter();
        }
        if (customKey && i < 4) {
          password[i++] = customKey;
          lcd.print(customKey);
        }
        if (i == 4) {
          password[4] = '\0'; // Null-terminate password yang dimasukkan
          delay(200);

          if (strncmp(password, pass, 4) == 0) {
            digitalWrite(solenoid, LOW);
            lcd.clear();
            lcd.print("Pass Accepted");
            delay(3000);
            lcd.clear();
            lcd.print("Take your saving");
            lcd.setCursor(0, 1);
            lcd.print("You lose :(");
            playMelody(); 
            delay(3000); 
            resetSystem();
            return;
          } else {
            lcd.clear();
            //digitalWrite(solenoid, LOW); // Mematikan relay jika password salah
            lcd.print("Password Rejected");
            delay(2000);
            lcd.clear();
            lcd.print("Input Password:");
            lcd.setCursor(0, 1);
          }

          i = 0; // Reset indeks
          memset(password, 0, sizeof(password)); // Bersihkan buffer password
          closeDoor();
          //digitalWrite(solenoid, LOW); // Memastikan relay mati
        }
        if (customKey == '*') {
          lcd.clear();
          displayMenu();
          return;
        }
      }
    }
  }
}

void playMelody1() {
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    int duration = 1000 / durations[note];
    tone(buzzer, melody1[note], duration);

    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    noTone(buzzer);
  }
}

void beep(char key){
  digitalWrite(buzzer, HIGH);
  delay(20);
  digitalWrite(buzzer, LOW);
}

void playTone(int frequency, int duration) {
  tone(buzzer, frequency, duration);
  delay(duration);
  noTone(buzzer);
}

void resetSystem() {
  nilaisetmax = 0;
  nilai = 0;
  saldox = 0;
  tabungan = 0;
  input = "";
  maxSet = false;
  x = 0;
  displayMenu();
  closeDoor();
}

void openDoor() {
  digitalWrite(solenoid,HIGH);  // Open door
}

void closeDoor() {
  digitalWrite(solenoid,LOW);  
}

void beep(){
  digitalWrite(buzzer, HIGH);
  delay(20);
  digitalWrite(buzzer, LOW);

}
void playMelody() {
  for (int thisNote = 0; thisNote < sizeof(melody)/sizeof(melody[0]); thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(buzzer);
  }
}