
#include <Arduino.h>
#include <Wire.h>                 
#include <LiquidCrystal_I2C.h>   
// Configura endereço I2C e display com 16 caracteres e 2 linhas 
LiquidCrystal_I2C lcd(0x20, 16, 2);  

//ports in
const int btUp = 8;
const int btDown= 9;
const int btRd = 10;
const int btLf = 11;
const int enter = 12;

//ports out
const int aquece = 7;
const int resfria = 6;
//sensor de temperatura
const int sensor=A0; 

int thisChar = 0;

float tempc; //variable to store temperature in degree Celsius
float sensorTemp; //temporary variable to hold sensor reading
int temMax = 50;
int tolerancia = 2;

//exaustor 
bool exaustor = false;
bool sempreLigado = false;
bool intermitente = true;
bool statusFan = false; 
int fanHtzControl = 0;

void lerVerticaButtons(int* itemMenu, int* enterMenu, int maiorValor);
void lerHorizontaisButons(int* itemMenu, int* enterMenu, int maiorValor);
char* conversorFloat(float valor);
void verificaTemperatura();
void mostrarTemperatura();
void clenerConfigFan();
void defineTemp();
void menuTemp();
void menuFan();
void menu();

void setup()
{
  pinMode(btUp, INPUT);
  pinMode(btDown, INPUT);
  pinMode(btRd, INPUT);
  pinMode(btLf, INPUT);
  pinMode(enter, INPUT);
  pinMode(aquece, OUTPUT);
  pinMode(resfria, OUTPUT);
  pinMode(sensor,INPUT); 

  lcd.init();                      
  lcd.backlight();  //backlight
  if(sempreLigado){
    digitalWrite(resfria, HIGH);
  }
}

void loop()
{
  if ( digitalRead(enter) == true)  
    menu();  
  
  verificaTemperatura();
  mostrarTemperatura();
  delay(500);
  lcd.clear();  
}

void verificaTemperatura(){
  sensorTemp=analogRead(sensor);
  sensorTemp=(sensorTemp*500)/1023;
  tempc=sensorTemp; 
  if(tempc + tolerancia < temMax){
    digitalWrite(aquece, HIGH);
  } else if (tempc > temMax){
    digitalWrite(aquece, LOW);
  }

  /** função fan liga conforme temperatura*/
  if (exaustor){
    if (tempc + tolerancia > temMax){
      digitalWrite(resfria, HIGH);
      digitalWrite(aquece, LOW);
    } else {
      digitalWrite(resfria, LOW);
    }
  /** função fan sempre ligado*/
  } else if(sempreLigado){
    if(!digitalRead(resfria)){
      digitalWrite(resfria, HIGH);
    }
  /** função fan intermitente*/
  } else if(intermitente){
    if(fanHtzControl > 1000 && digitalRead(resfria)){
      digitalWrite(resfria, LOW);
      fanHtzControl = 0;
    } else if(fanHtzControl > 1000 && !digitalRead(resfria)){
      digitalWrite(resfria, HIGH);
      fanHtzControl = 0;
    } else {
      fanHtzControl++;
    }
  }
}

void menu(){
  bool menu = true;
  int itemMenu = 0;
  int enterMenu = 0;
  int controleMenu = 0;
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("use o botão ↓ ↑");
  lcd.setCursor(2, 1);
  lcd.print("Escolha c/ OK");
  while (menu)
  {
    lerVerticaButtons(&itemMenu, &enterMenu, 2);
    if(enterMenu == 1){
      controleMenu = itemMenu + 1;
      switch (itemMenu)
      {
      case 0:
        menu = false;
        return;
      case 1:
        menuTemp();
        break;
      case 2:
        menuFan();
        break;
      default:
        break;
      }
    }
    if(itemMenu != controleMenu){
      controleMenu = itemMenu;
      switch(itemMenu)
      {
      case 0 :
        //sair
        lcd.clear();  
        lcd.setCursor(0, 0);
        lcd.print("SAIR");
        lcd.setCursor(2, 1);
        lcd.print("pressione OK");
        break;
      case 1 :
        //temp
        lcd.clear();  
        lcd.setCursor(0, 0);
        lcd.print("MENU TEMP.");
        lcd.setCursor(2, 1);
        lcd.print("pressione OK");
        break;
      case 2 :
        //fan
        lcd.clear();  
        lcd.setCursor(0, 0);
        lcd.print("MENU FAN.");
        lcd.setCursor(2, 1);
        lcd.print("pressione OK");
        break;
      default:
        break;
      }
    }
    delay(50);
  }
}

void menuTemp(){
  int itemMenu = 0;
  int enterMenu = 0;
  int controleMenu = 0;
  while (true)
  {
    lerHorizontaisButons(&itemMenu, &enterMenu, 2);
    if(enterMenu == 1){
      controleMenu = itemMenu + 1;
      switch (itemMenu)
      {
      case 0:
        return;
      case 1:
        defineTemp();
        break;
      case 2:
        menuFan();
        break;
      default:
        break;
      }
    }
    if(itemMenu != controleMenu){
    controleMenu = itemMenu;
      switch (itemMenu)
      {
        case 0 :
            //sair
            lcd.clear();  
            lcd.setCursor(0, 0);
            lcd.print("SAIR");
            lcd.setCursor(2, 1);
            lcd.print("pressione OK");
            break;
          case 1 :
            //temp
            lcd.clear();  
            lcd.setCursor(0, 0);
            lcd.print("DEFINIR TEMP.");
            lcd.setCursor(2, 1);
            lcd.print("pressione OK");
            break;
          case 2 :
            //fan
            lcd.clear();  
            lcd.setCursor(0, 0);
            lcd.print("DEFINIR TOLE.");
            lcd.setCursor(2, 1);
            lcd.print("pressione OK");
            break;
          default:
            break;
      }
    }
    delay(50);
  }
}

void defineTemp(){
  int itemTemp = temMax;
  int enterTemp = 0;
  char* valor = conversorFloat(tempc);
  char bufferTemp[32];
  sprintf(bufferTemp, "modificar < %d.00° >", temMax);
  lcd.setCursor(0, 0);
  lcd.print("Temperatura Max");
  lcd.rightToLeft();    
  lcd.setCursor(2, 1);
  lcd.print(bufferTemp);
  free(valor);
  while (true)
  {
    lerHorizontaisButons(&itemTemp, &enterTemp, 70);
    if(enterTemp){
      temMax = itemTemp;
      return;
    }
    sprintf(bufferTemp, "modificar: < %d.00° >", itemTemp);
    lcd.setCursor(2, 1);
    lcd.print(bufferTemp);
    delay(50);
  }
}

void defineTolerancia(){
  int itemTol = tolerancia;
  int enterTol = 0;
  char* valor = conversorFloat(tempc);
  char bufferTol[32];
  sprintf(bufferTol, "modificar < %d.00° >", tolerancia);
  lcd.setCursor(0, 0);
  lcd.print("Tolerancia Max");
  lcd.rightToLeft();    
  lcd.setCursor(2, 1);
  lcd.print(bufferTol);
  free(valor);
  while (true)
  {
    lerHorizontaisButons(&itemTol, &enterTol, 70);
    if(enterTol){
      tolerancia = itemTol;
      return;
    }
    sprintf(bufferTol, "modificar: < %d.00° >", itemTol);
    lcd.setCursor(2, 1);
    lcd.print(bufferTol);
    delay(50);
  }
}

void menuFan(){
  int itemFan = 0;
  int enterFan = 0;
  int controleMenu = 0;
  // char* valor = conversorFloat(tempc);
  // char bufferFan[32];

  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("use o botão ↓ ↑");
  lcd.setCursor(2, 1);
  lcd.print("Escolha c/ OK");
  while (true)
  {
    lerVerticaButtons(&itemFan, &enterFan, 2);
    if(enterFan){
      switch (itemFan)
      {
      case 0:
        clenerConfigFan();
        intermitente = true;
        return;
      case 1:
        clenerConfigFan();
        exaustor = true;
        return;
      case 2:
        clenerConfigFan();
        sempreLigado = true;
        return;
      default:
        break;
      }
      return;
    }
    if(itemFan != controleMenu){
    controleMenu = itemFan;
      switch (itemFan)
      {
        case 0 :
            lcd.clear();  
            lcd.setCursor(0, 0);
            lcd.print("Intermitente.");
            lcd.setCursor(2, 1);
            lcd.print("pressione OK");
            break;
          case 1 :
            //temp
            lcd.clear();  
            lcd.setCursor(0, 0);
            lcd.print("Exaustor.");
            lcd.setCursor(2, 1);
            lcd.print("pressione OK");
            break;
          case 2 :
            //fan
            lcd.clear();  
            lcd.setCursor(0, 0);
            lcd.print("Sempre Ligado.");
            lcd.setCursor(2, 1);
            lcd.print("pressione OK");
            break;
          default:
            break;
      }
    }
    delay(50);
  }
}

void clenerConfigFan(){
  exaustor = false;
  sempreLigado = false;
  intermitente = false;
}

void lerVerticaButtons(int* itemMenu, int* enterMenu, int maiorValor){
    if(digitalRead(enter) == true){
      *enterMenu = 1;
    }
    if(digitalRead(btDown) == true){
      if(*itemMenu < maiorValor){
        itemMenu++; 
      }
    }else if(digitalRead(btUp) == true){
      if(*itemMenu > 0){
        itemMenu--;
      }
    }
}

void lerHorizontaisButons(int* itemMenu, int* enterMenu, int maiorValor){
    if(digitalRead(enter) == true){
      *enterMenu = 1;
    }
    if(digitalRead(btLf) == true){
      if(*itemMenu < maiorValor){
        itemMenu++; 
      }
    }else if(digitalRead(btRd) == true){
      if(*itemMenu > 0){
        itemMenu--;
      }
    }
}

void mostrarTemperatura() {
  char* valor = conversorFloat(tempc);
  char bufferTemp[32];
  char bufferStatus[32];
  const char* statusFan = (digitalRead(resfria) == HIGH) ? "Lig" : "Des";
  const char* statusRes = (digitalRead(aquece) == HIGH) ? "Lig" : "Des";
  sprintf(bufferTemp, "Temperatura: %s°", valor);
  sprintf(bufferStatus, "Fan: %s, Resist.: %s", statusFan,statusRes); // adiciona o ponto e vírgula aqui
  lcd.setCursor(0, 0);
  lcd.print(bufferTemp);
  lcd.rightToLeft();            // imprime da direita para a esquerda
  lcd.setCursor(2, 1);
  lcd.print(bufferStatus);
  free(valor);
}

char* conversorFloat(float valor) {
    char* convertido = (char*) malloc(16 * sizeof(char)); // aloca o buffer de caracteres
    sprintf(convertido, "%.3f", valor); // Convertido para double aqui
    return convertido;
}
