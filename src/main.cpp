/**
 * @file main.cpp
 * @brief software for controll temperature of oven.
 *
 * This software is used to control a drying oven for removing moisture from 3D printer filaments,
 * so it has some fixed parameters. You can find more details about the hardware
 * at: https://github.com/ricardocvel/drying_oven_controller/blob/master/outers/hardware/hardware.JPG."
 *
 * @author ricardo Campos ricardocampos_pr@live.com
 * @date   March 5, 2023
 */

#include <Arduino.h>
#include <Wire.h>                 
#include <LiquidCrystal_I2C.h>   
// Configure I2C address and display with 16 characters and 2 lines
LiquidCrystal_I2C lcd(0x20, 16, 2);  

//ports in
const int btUp = 11;
const int btDown= 10;
const int btRd = 8;
const int btLf = 9;
const int enter = 12;

//ports out
const int aquece = 7;
const int resfria = 6;
//sensor de temp
const int sensor=A0; 

int thisChar = 0;

float tempc; 
float sensorTemp; 
int temMax = 50;
int tolerancia = 2;

//exaustor 
bool exaustor = false;
bool sempreLigado = false;
bool intermitente = true;
bool statusFan = false; 
int fanHtzControl = 0;

struct MenuData {
  int itemMenu;
  int enterMenu;
  int controleMenu;
  MenuData() : itemMenu(0), enterMenu(0), controleMenu(0) {}
};

void lerVerticaButtons(MenuData* data, int maiorValor);
void lerHorizontaisButons(MenuData* data, int maiorValor);
char* conversorFloat(float valor);
void verificaTemperatura();
void mostrarTemperatura();
void clenerConfigFan();
void defineTemp();
void menuTemp();
void menuFan();
void pDebugger(int valor);
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
  lcd.backlight();
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
  delay(100);
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

  /** fan function start for temp*/
  if (exaustor){
    if (tempc + tolerancia > temMax){
      digitalWrite(resfria, HIGH);
      digitalWrite(aquece, LOW);
    } else {
      digitalWrite(resfria, LOW);
    }
  /** fan function always on*/
  } else if(sempreLigado){
    if(digitalRead(resfria) == false){
      digitalWrite(resfria, HIGH);
    }
  /** fan function intermittent*/
  } else if(intermitente){
    if(fanHtzControl > 100 && digitalRead(resfria) == true){
      digitalWrite(resfria, LOW);
      fanHtzControl = 0;
    } else if(fanHtzControl > 100 && digitalRead(resfria) == false){
      digitalWrite(resfria, HIGH);
      fanHtzControl = 0;
    } else {
      fanHtzControl++;
    }
  }
}
void menu(){
  MenuData data;
  bool menu = true;
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("use o botao ^");
  lcd.setCursor(2, 1);
  lcd.print("Escolha c/ OK");
  data.enterMenu = 1;
  while (menu)
  {
    lerVerticaButtons(&data, 3);
    if(data.enterMenu == 1){
      data.controleMenu = data.itemMenu + 1;
      switch (data.itemMenu)
      {
      case 3:
        menu = false;
        break;
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
    if(data.itemMenu != data.controleMenu){
      data.controleMenu = data.itemMenu;
      switch(data.itemMenu)
      {
      case 3 :
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
    delay(100);
  }
}

void menuTemp(){
  MenuData dataTemp;
  // bool menu = true;
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("use o botao ^");
  lcd.setCursor(2, 1);
  lcd.print("Escolha c/ OK");
  dataTemp.enterMenu = 1;

  while (true)
  {
    lerVerticaButtons(&dataTemp, 3);
    if(dataTemp.enterMenu == 1){
      dataTemp.controleMenu = dataTemp.itemMenu + 1;
      switch (dataTemp.itemMenu)
      {
      case 3:
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
    if(dataTemp.itemMenu != dataTemp.controleMenu){
    dataTemp.controleMenu = dataTemp.itemMenu;
      switch (dataTemp.itemMenu)
      {
        case 3 :
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
    delay(100);
  }
}

void defineTemp(){
  MenuData dataDefineTemp;
  dataDefineTemp.enterMenu = 1;
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Temperatura Max");
  lcd.setCursor(2, 1);
  lcd.print("<");
  lcd.setCursor(5, 1);
  lcd.print(dataDefineTemp.itemMenu);
  lcd.setCursor(8, 1);
  lcd.print(".00° >");
  while (true)
  {
    lerHorizontaisButons(&dataDefineTemp, 70);
    if( dataDefineTemp.enterMenu){
      temMax = dataDefineTemp.itemMenu;
      return;
    }

    lcd.setCursor(5, 1);
    lcd.print(dataDefineTemp.itemMenu);
    delay(100);
  }
}

void defineTolerancia(){
  MenuData dataTol;
  dataTol.enterMenu = 1;

  int itemTol = tolerancia;
  int enterTol = 0;
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Tolerancia Max");
  lcd.rightToLeft();    
  lcd.setCursor(2, 1);
  lcd.print("<");
  lcd.setCursor(5, 1);
  lcd.print(tolerancia);
  lcd.setCursor(7, 1);
  lcd.print(".00° >");
  while (true)
  {
    lerHorizontaisButons(&dataTol, 10);
    if(dataTol.enterMenu){
      tolerancia = dataTol.itemMenu;
      return;
    }
    lcd.setCursor(5, 1);
    lcd.print(dataTol.itemMenu);
    delay(100);
  }
}

void menuFan(){
  MenuData datafan;

  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("use o botão ^");
  lcd.setCursor(2, 1);
  lcd.print("Escolha c/ OK");
  while (true)
  {
    lerVerticaButtons(&datafan, 2);
    if(datafan.enterMenu){
      switch (datafan.itemMenu)
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
    if(datafan.itemMenu != datafan.controleMenu){
    datafan.controleMenu = datafan.itemMenu;
      switch (datafan.itemMenu)
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
    delay(100);
  }
}

void clenerConfigFan(){
  exaustor = false;
  sempreLigado = false;
  intermitente = false;
}

void lerVerticaButtons(MenuData* data, int maiorValor){
  if(digitalRead(enter) == true){
    data->enterMenu = 1;
    return;
  } else {
    data->enterMenu = 0;
  }
  if(digitalRead(btDown) == true){
    if(data->itemMenu < maiorValor){
      data->itemMenu++;
    }
  }else if(digitalRead(btUp) == true){
    if(data->itemMenu > 1){
      data->itemMenu--;
    }    
  }
}
void lerHorizontaisButons(MenuData* data, int maiorValor){
  if(digitalRead(enter) == true){
    data->enterMenu = 1;
    return;
  } else {
    data->enterMenu = 0;
  }
  if(digitalRead(btLf) == true){
    if(data->itemMenu < maiorValor){
      data->itemMenu++;
    }
  }else if(digitalRead(btRd) == true){
    if(data->itemMenu > 1){
      data->itemMenu--;
    }    
  }
}

void mostrarTemperatura() {

  const char* statusFan = []() {
    if (digitalRead(resfria)) {
        return "Lig";
    } else {
        return "Des";
    }
  }();

  const char* statusRes = []() {
    if (digitalRead(aquece)) {
        return "Lig";
    } else {
        return "Des";
    }
  }();

  lcd.setCursor(0, 0);
  lcd.print( "Temp.:");
  lcd.setCursor(8, 0);
  lcd.print(tempc);
  lcd.setCursor(0, 1);
  lcd.print(fanHtzControl);
  // lcd.print("Fan");
  // lcd.setCursor(4, 1);
  // lcd.print(statusFan);
  // lcd.setCursor(7, 1);
  // lcd.print(" - Res");
  // lcd.setCursor(14, 1);
  // lcd.print(statusRes);

  // free(valor);
}

void pDebugger(int valor){
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Debbug");
  lcd.setCursor(2, 1);
  lcd.print(valor);
}

char* conversorFloat(float valor) {
    char* convertido = (char*) malloc(16 * sizeof(char)); // aloca o buffer de caracteres
    sprintf(convertido, "%.3f", valor); // Convertido para double aqui
    return convertido;
}
