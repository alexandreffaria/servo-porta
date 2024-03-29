#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>


#define pinoServo 3
Servo Chave;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

bool aberto = false;

void setup() {
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Chave.attach(pinoServo);
  Chave.write(0);
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  if (conteudo.substring(1,3) == "08") //UID 1 - Chaveiro
  {
    Serial.println("Ola Meulindo !");
    
    if (aberto){
        Chave.writeMicroseconds(1600);  
        delay(4240);
        Chave.writeMicroseconds(1500);
        aberto = false;  
    }
    else{
        Chave.writeMicroseconds(1300);  
        delay(4240);
        Chave.writeMicroseconds(1500);
        aberto = true;
      }
    
    Serial.println(Chave.read());
    Serial.println(aberto);
    delay(1000);
  }
}
