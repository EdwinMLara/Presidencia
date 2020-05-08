#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Pines
uint8_t bzz = 2;
uint8_t LED = 3;
uint8_t gt  = 4;

// Variables Loop
bool gt_val;
bool bandera = true; 

// Variables Temeperatura
float IRtemp,Atemp;
float RefTemp=37.6;

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

void setup() {
  Serial.begin(9600);
  
  pinMode(bzz,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(gt,INPUT_PULLUP);
  

  Serial.println("Adafruit MLX90614 test"); 
  config_lcd(); 
  inicio();
  
  mlx.begin();

  Atemp   = mlx.readAmbientTempC();
  IRtemp  = mlx.readObjectTempC();
}

float Vta[11], Vto[11];
float SumTa, SumTo;
byte  Ns  = 15;

void loop() {
  
    gt_val = digitalRead(gt);
     
  if(!gt_val){    
    digitalWrite(LED,1);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    String print_lcd_ambiente = "Ambiente " + String(Atemp) + char(223) + "C"; 
    lcd.print(print_lcd_ambiente);
    lcd.setCursor(0, 1);
    String print_lcd_object = "Persona   ...";
    lcd.print(print_lcd_object);
    bandera = true;
    
    for(byte n=0; n<Ns; n++){
      Vta[n] = mlx.readAmbientTempC();
      delay(100);
      digitalWrite(bzz,1);
      delay(50);
      digitalWrite(bzz,0);
      Vto[n] = mlx.readObjectTempC();
      delay(100);
    }
    
    for(byte n1=0; n1<Ns; n1++){
      Serial.print(Vta[n1]);
      Serial.print(" : ");
      Serial.println(Vto[n1]); 
    }
    
    for(byte n2=0; n2<Ns; n2++){
      SumTa+=Vta[n2];
      SumTo+=Vto[n2];
    }
    Atemp   = SumTa/Ns;
    IRtemp  = (SumTo/Ns) + 1.8;
    SumTa = 0;
    SumTo = 0;
    
    Serial.println(RefTemp);
    Serial.print("Ambient = "); Serial.print(Atemp); 
    Serial.print("*C\tObject = "); Serial.print(IRtemp); Serial.println("*C");
    

    if(IRtemp < RefTemp){
      Alerta1();
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      String print_lcd_ambiente = "Advertencia "; 
      lcd.print(print_lcd_ambiente);
      lcd.setCursor(0, 1);
      String print_lcd_object = "Revise bien";
      lcd.print(print_lcd_object);
      Alerta2();
      }
      digitalWrite(LED,0);
  }else{

    if(bandera){
      lcd.clear();
      lcd.setCursor(0, 0);
      String print_lcd_ambiente = "Ambiente " + String(Atemp) + char(223) + "C"; 
      lcd.print(print_lcd_ambiente);
      lcd.setCursor(0, 1);
      String print_lcd_object = "Persona " + String(IRtemp) + char(223) + "C";
      lcd.print(print_lcd_object);
      bandera = false;
    }
    
  }
  delay(10);
}

void Alerta1(){
  digitalWrite(bzz,HIGH);
  delay(500);
  digitalWrite(bzz,LOW);
}

void Alerta2(){
  digitalWrite(bzz,HIGH);delay(250);digitalWrite(bzz,LOW); delay(100);
  digitalWrite(bzz,HIGH);delay(250);digitalWrite(bzz,LOW); delay(100);
  digitalWrite(bzz,HIGH);delay(250);digitalWrite(bzz,LOW); delay(100);
  digitalWrite(bzz,HIGH);delay(250);digitalWrite(bzz,LOW); delay(100);
  digitalWrite(bzz,HIGH);delay(250);digitalWrite(bzz,LOW); delay(100);
  digitalWrite(bzz,HIGH);delay(250);digitalWrite(bzz,LOW); delay(100);
}

void inicio(){
      lcd.clear();
      lcd.setCursor(0, 0);
      String print_lcd_ambiente = "  MUNICIPIO DE  "; 
      lcd.print(print_lcd_ambiente);
      lcd.setCursor(0, 1);
      String print_lcd_object = "   URIANGATO    ";
      lcd.print(print_lcd_object);
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      print_lcd_ambiente = "     INSOEL     "; 
      lcd.print(print_lcd_ambiente);
      delay(700);
}
