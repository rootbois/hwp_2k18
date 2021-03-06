#include <LiquidCrystal.h>

int c;

// PINS
#define R_S 3
#define E   4
#define DB4 6
#define DB5 7
#define DB6 8
#define DB7 9
#define NUM_CHAR 20
#define NUM_LINES 4

// Define the LCD screen
LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

// Aufgabe 1
void setPin11(bool high) {
  // setPin11() requires 125 milliseconds
  // setPin11Asm() requires 125 milliseconds
  // It seems that some kind of optimisation is 
  // executed by the compiler
  if (high) {
    PORTB |= (1 << 3);
  }
  else {
    PORTB &= ~(1 << 3);
  }
}
// Aufgabe 2
void setPin11Asm(bool high) {
 // for 10000 iterations of toggling between true/false it takes 8 milliseconds
 if (high) {
      asm volatile (
        "sbi %0, %1\n\t"
        :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB3)
        );
    }
    else {
      asm volatile (
        "cbi %0, %1\n\t"
        :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB3));
    }
}
/*void __attribute__((optimize("O0"))) setPin11Asm(bool high) {
  // for 10000 iterations of toggling between true/false it takes 109.0 milliseconds
  // that is way worse than the optimized variation above (8 milliseconds).
    if (high) {
      asm volatile (
        "sbi %0, %1\n\t"
        :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB3)
        );
    }
    else {
      asm volatile (
        "cbi %0, %1\n\t"
        :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB3));
    }
}*/

void setup() {
  // put your setup code here, to run once:
  pinMode(11,OUTPUT);
  lcd.begin(NUM_CHAR, NUM_LINES);
  c = 0;
  double start_Asm = millis();
  
  while (c < 100,00) {
    setPin11Asm(false);
    setPin11Asm(true);
    c++;
  }
  double end_Asm = millis();
  double dur_Asm = end_Asm - start_Asm;
  c = 0;
  double start_Norm = millis();
  
  while ( c < 100,00) {
    setPin11(false);
    setPin11(true);
    c++;
  }
  double end_Norm = millis();
  double dur_Norm = end_Norm - start_Norm;
  c = 0; 
  double start_BuiltIn = millis();
  
  while (c < 100,000) {
    analogWrite(11,LOW);
    analogWrite(11,HIGH);
    c++;
  }
  double end_BuiltIn = millis();
  double dur_BuiltIn = end_BuiltIn - start_BuiltIn;

  lcd.setCursor(0,0);
  lcd.print("With Assembly: ");
  lcd.setCursor(0,16);
  lcd.print(dur_Asm);
  lcd.setCursor(1,0);
  lcd.print("Without Assembly: ");
  lcd.setCursor(1,20);
  lcd.print(dur_Norm);
  lcd.setCursor(2,0);
  lcd.print("Built-In: ");
  lcd.setCursor(2,11);
  lcd.print(dur_BuiltIn);
}
void loop() {
  // put your main code here, to run repeatedly:
  }
