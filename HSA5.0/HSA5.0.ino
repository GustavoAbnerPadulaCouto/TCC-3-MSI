#include <LiquidCrystal.h>
#include<Servo.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 HSA(SS_PIN, RST_PIN);

LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

Servo tranca;

int control = 0;

int liberado = 0;
  
void setup() 
{ 
  tranca.attach(0); 
  lcd.begin(16, 4);

  SPI.begin();//Inicia o SPI bus.
  HSA.PCD_Init();//Inicia o MFRC522.

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("HOME SECURITY");
  lcd.setCursor(4,1);
  lcd.print("AUTOMATED");
}

void loop() 
{
  if (!HSA.PICC_IsNewCardPresent())
  {
    return; 
  }
  if(!HSA.PICC_ReadCardSerial())
  {
    return;  
  }

  String IDtag = "";
  byte letra;

  for(byte i = 0; i < HSA.uid.size; i++)
  {
    IDtag.concat(String(HSA.uid.uidByte[i] < 0x10 ? " 0": ""));
    IDtag.concat(String(HSA.uid.uidByte[i], HEX));
  }
  IDtag.toUpperCase();

  if(IDtag.substring(1) == "0665BF12" && liberado == 0)
  {
    tranca.write(90);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acesso Liberado");
    lcd.setCursor(1,1);
    lcd.print("HOME SECURITY");
    delay(1500);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("HOME SECURITY");
    lcd.setCursor(4,1);
    lcd.print("AUTOMATED");

    liberado = 1;
  }else if(IDtag.substring(1) == "0665BF12" && liberado == 1)
  {
     tranca.write(0);
     
     lcd.clear();
     lcd.setCursor(4, 0);
     lcd.print("Trancado");
     lcd.setCursor(1,1);
     lcd.print("HOME SECURITY");
     delay(1500);
     lcd.clear();
     lcd.setCursor(2, 0);
     lcd.print("HOME SECURITY");
     lcd.setCursor(4,1);
     lcd.print("AUTOMATED");

     liberado = 0;
    }else{
       lcd.clear();
       lcd.setCursor(2, 0);
       lcd.print("Acesso Negado");
       lcd.setCursor(4,1);
       lcd.print(IDtag);
       delay(1500);
       lcd.clear();
       lcd.setCursor(2, 0);
       lcd.print("HOME SECURITY");
       lcd.setCursor(4,1);
       lcd.print("AUTOMATED");
       
    }
  }

 
