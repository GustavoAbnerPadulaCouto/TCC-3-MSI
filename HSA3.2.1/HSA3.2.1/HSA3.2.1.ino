//Inclui as bibliotecas. 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include<Servo.h>

//Define os pinos do arduino.
#define SS_PIN 10
#define RST_PIN 9
 
//Variável que controla o estado da fechadura.
int liberado = 0;

//Variável que controla a abertura da porta.
int control = 0;

//Define os pinos do LCD.
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

//Cria uma instância para o leitor RFID. 
MFRC522 HSA(SS_PIN, RST_PIN);

//Cria um objeto para o servo.
Servo tranca;

void setup() 
{
  tranca.attach(0); //Incia o servo.
  
  lcd.begin(16,4); //Define o número de colunas e linhas do LCD.
  
  Serial.begin(9600);//Inicia o monitor serial.
  SPI.begin();//Inicia o SPI bus.
  HSA.PCD_Init();//Inicia o MFRC522.

  //mensagem incial
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("HOME SECURITY");
  lcd.setCursor(4,1);
  lcd.print("AUTOMATED");
  Serial.println("Aproxime o seu cartão do leitor...");
  Serial.println();
}

void loop() 
{
  //Procura tags a serem lidas.
  if (!HSA.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!HSA.PICC_ReadCardSerial())
  {
    return;
  }
  
  //Exibe o ID da tag.
  Serial.print("UID da tag:");
  String IDtag= "";
  byte letra;

  //Pega o ID da tag.
  for (byte i = 0; i < HSA.uid.size; i++)
  {
   Serial.print(HSA.uid.uidByte[i] < 0x10 ? " 0" : " ");
   Serial.print(HSA.uid.uidByte[i], HEX);
   IDtag.concat(String(HSA.uid.uidByte[i] < 0x10 ? " 0": ""));
   IDtag.concat(String(HSA.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  IDtag.toUpperCase();   

  //Compara o ID lido com o ID da tag do cartão liberado e compara o valor da variável que controla o estado da fechadura.
  if(IDtag.substring(1) == "0665BF12" && liberado == 0)
  {
    //Servo se move para posição zero permitindo abertura da porta.
    for (control = 90; control >= 0; control -= 1) {
    tranca.write(control);
    }
    
    //Exibe a mensagem no LCD
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

    //Exibe a mensagem no Monitor serial
    Serial.println("Acesso Liberado");
    liberado = 1;
    delay(1500);
    
   //Compara o ID lido com o ID da tag do cartão liberado e compara o valor da variável que controla o estado da fechadura.
  }else if(IDtag.substring(1) == "0665BF12" && liberado == 1){

    //Servo se move para posição de 90 graus fechando a porta.
    for (control = 0; control <= 90; control += 1) {
    tranca.write(control);
    }
    
    //Exibe a mensagem no LCD
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

    //Exibe a mensagem no monitor serial.
    Serial.println("Trancado");
    liberado = 0;
    delay(2000);
    
  //Nega o acesso para cartões que não corresponde com o cartão liberado.  
  }else{
    
    //Exibe a mensagem no LCD
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
    
    
    //Exibe a mensagem no monitor serial.
    Serial.println("Acesso Negado");
    delay(1000);
  } 
}
