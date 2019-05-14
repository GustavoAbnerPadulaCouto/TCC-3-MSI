//Incluindo as biblioteca para a conclusão do projeto.
#include <LiquidCrystal.h>
#include<Servo.h>
#include <MFRC522.h>

//Define as portas 10 e 9 para o RFID.
#define SS_PIN 10
#define RST_PIN 9

//Cria uma instância para o leitor RFID. 
MFRC522 HSA(SS_PIN, RST_PIN);

//Define os pinos do LCD.
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

//Cria um objeto para o servo.
Servo tranca;

//Variável que controla o estado da fechadura.
int liberado = 0;

//Variável que controla o buzzer.
const int buzzer = 6;
  
void setup() 
{ 
  // Define o pino do Buzzer como Saida 
  pinMode(buzzer,OUTPUT);
  
  tranca.attach(0); //Incia o servo.
  lcd.begin(16, 4); //Define o número de colunas e linhas do LCD.

  SPI.begin();//Inicia o SPI bus.
  HSA.PCD_Init();//Inicia o MFRC522.

  //Mensagem inicial.
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("HOME SECURITY");
  lcd.setCursor(4,1);
  lcd.print("AUTOMATED");
}

void loop() 
{
  //Procura tags a serem lidas.
  if (!HSA.PICC_IsNewCardPresent())
  {
    return; 
  }
  if(!HSA.PICC_ReadCardSerial())
  {
    return;  
  }

  //Exibe o ID da tag.
  String IDtag = "";
  byte letra;

  //Pega o ID da tag.
  for(byte i = 0; i < HSA.uid.size; i++)
  {
    IDtag.concat(String(HSA.uid.uidByte[i] < 0x10 ? " 0": ""));
    IDtag.concat(String(HSA.uid.uidByte[i], HEX));
  }
  IDtag.toUpperCase();

 //Compara o ID lido com o ID da tag do cartão liberado e compara o valor da variável que controla o estado da fechadura.
  if(IDtag.substring(1) == "0665BF12" && liberado == 0)
  {
    //Servo se move para posição 90ºGraus permitindo abertura da porta.
    tranca.write(90);

    //Exibe mensagem de acesso liberado no LCD.
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
     //Servo se move para posição zero permitindo abertura da porta.
     tranca.write(0);

     //Exibe a mensagem de trancado no LCD.
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
       //Exibe a mensagem de acesso negado no LCD.
       lcd.clear();
       lcd.setCursor(2, 0);
       lcd.print("Acesso Negado,");
       lcd.setCursor(4,1);
       lcd.print(IDtag);

       //Ligando o buzzer com uma frequencia de 1500 hz.
       tone(buzzer,1500);   
       delay(1000);
       //Desligando o buzzer.
       noTone(buzzer);
       delay(1000);
       
       lcd.clear();
       lcd.setCursor(2, 0);
       lcd.print("HOME SECURITY");
       lcd.setCursor(4,1);
       lcd.print("AUTOMATED");
    }
  }

 
