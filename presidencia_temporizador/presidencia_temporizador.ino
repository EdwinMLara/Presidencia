#include <Wire.h>
#include<string.h> 
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);

unsigned long startmillis;
unsigned long currentmillis;
unsigned long countTime;

unsigned long tiempo_encendido = 4;
unsigned long tiempo_apagado = 2;
unsigned long tiempo_encendido2 = 5;
unsigned long tiempo_apagado2 = 13;
boolean banderas[4];
boolean gt_val,status_rele,aux_status_rele;
unsigned long desfaseTime = 0;

void iniciar_banderas(boolean *banderas, uint8_t tam){
  uint8_t i;
  for(i=0;i<=tam-1;i++){
    banderas[i] = true;
  }
}

void config_lcd(){
  int error;

  while (!Serial);

  Serial.println("Dose: check for LCD");
  
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } 

  lcd.begin(16, 2 );
  lcd.setBacklight(100);
  lcd.clear();
}


void pintar_Pantalla(char str[]){
    
  String estado_rele;
  if(status_rele != aux_status_rele){
    aux_status_rele = status_rele;
    lcd.clear();
    if(status_rele) 
      estado_rele = "Encendido";
    else
      estado_rele = "Apagado";
  }

  lcd.setCursor(0, 0);
  lcd.print("Municipio  Uriangato");
  lcd.setCursor(0, 1);
  char fecha[20] = "Tiempo - ";
  strcat(fecha,str);
  lcd.print(fecha);
  
  estado_rele = "Estado: " + estado_rele;
  lcd.setCursor(0, 2);
  lcd.print(estado_rele);
  lcd.setCursor(0,3);
  lcd.print("Por: TIC's ");
}

void Seguntos2Time(unsigned long tiempo){
    char str[20] = "";
    char aux[20] = "";
    char *str_time;
    unsigned long reciduo,hr,minu,seg;
    seg = tiempo % 60;
    reciduo = tiempo / 60;
    minu = reciduo % 60;
    reciduo = reciduo / 60;
    hr = reciduo%60;
    itoa(hr,aux,10);
    strcat(str,aux);
    strcat(str," : ");
    itoa(minu,aux,10);
    strcat(str,aux);
    strcat(str," : ");
    itoa(seg,aux,10);
    strcat(str,aux);
    Serial.println(str);
   
    pintar_Pantalla(str);
    /*str_time = strdup(str);
    return str_time;*/
}
void setup() {
  config_lcd();
  Serial.begin(9600); 
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(7,INPUT_PULLUP);
  startmillis = millis();
  digitalWrite(3,HIGH);
  status_rele = true;
  aux_status_rele = false;
  digitalWrite(4,LOW);

  iniciar_banderas(banderas,4);
  
  tiempo_encendido = tiempo_encendido*60*1000;
  tiempo_apagado = tiempo_apagado*60*1000;
  tiempo_encendido2 = tiempo_encendido2*60*1000;
  tiempo_apagado2 = tiempo_apagado2*60*1000;
}

void loop() {
  currentmillis = millis(); 
  countTime = (currentmillis - startmillis) + desfaseTime*60*1000; 
     
  if((countTime >= tiempo_encendido) && banderas[0]){
    digitalWrite(3,LOW);
    status_rele = false;
    digitalWrite(4,HIGH);
    Serial.println("Entro en 1");
    banderas[0] = false;
  }
  if((countTime >= (tiempo_encendido + tiempo_apagado)) && banderas[1]){
    digitalWrite(3,HIGH);
    status_rele = true;
    digitalWrite(4,LOW);
    Serial.println("Entro en 2");
    banderas[1] = false;
  }
  if((countTime >= (tiempo_encendido + tiempo_apagado + tiempo_encendido2)) && banderas[2]){
    digitalWrite(3,LOW);
    status_rele = false;
    digitalWrite(4,HIGH);
    Serial.println("Entro en 3");
    banderas[2] = false;
  }
  if((countTime >= (tiempo_encendido + tiempo_apagado + tiempo_encendido2 + tiempo_apagado2)) && banderas[3]){
    digitalWrite(3,HIGH);
    status_rele = true;
    digitalWrite(4,LOW);
    banderas[3] = false;
    startmillis = millis();
    iniciar_banderas(banderas,4);
    desfaseTime = 0;
    Serial.println("Reinicio"); 
  }

  gt_val = digitalRead(7);
  if(!gt_val){
    desfaseTime += 5;
  }

  Serial.print(countTime/1000);
  Serial.print(" , ");
  Serial.println(desfaseTime);
  Seguntos2Time(countTime/1000);
  delay(1000);
}
