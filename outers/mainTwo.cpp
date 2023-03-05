void Flash ()
{
  lcd.noDisplay();                // desliga display
  delay(1000);                    // atraso de meio segundo
  lcd.display();                  // liga display
  delay(1000);                    // atraso de meio segundo
  lcd.clear();                    // limpa a tela
  delay(1000);                    // atraso de 1 segundo
}
void Blink ()                     // teste do cursor
{
  lcd.noBlink();                  // apaga cursor
  delay(1000);                    // atraso de 1 segundo
  lcd.blink();                    // acende cursor
  delay(1000);                    // atraso de 1 segundo
  lcd.clear();                    // limpa a tela
  delay(1000);                    // atraso de 1 segundo
}
void AutoScroll ()                // teste de rolagem de mensagem
{
  lcd.setCursor(16, 1);           // selecionando coluna 16 e linha 1
  lcd.autoscroll();               // configura rolagem automatica de mensagem
  for (thisChar = 0; thisChar < 10; thisChar++)     // imprime de 0 a 9
  {
    lcd.print(thisChar);          // imprime o numero
    delay(350);                   // atraso de 350 ms
  }
  lcd.noAutoscroll();             // desliga rolagem autoamtica
  lcd.clear();                    // limpa a tela
  delay(1000);                    // atraso de 1 segundo
}
void dirText ()                   // teste de direcao de mensagem
{
  lcd.clear();                    // limpa a tela
  lcd.cursor();                   // liga o cursor
  lcd.setCursor(10, 0);           // selecionando coluna 10 e linha 1
  for (thisChar = 1; thisChar < 10; thisChar++)     // imprime de 1 a 9
  {
    lcd.rightToLeft();            // imprime da direita para a esquerda
    lcd.print(thisChar);          // imprime o numero
    delay(350);                   // atraso de 350 ms
  }
  for (thisChar = 1; thisChar < 10; thisChar++)     // imprime de 1 a 9
  {
    lcd.leftToRight();            // imprime da esquerda para a direita
    lcd.print(thisChar);          // imprime o numero
    delay(350);                   // atraso de 350 ms
  }
  lcd.noCursor();                 // desliga o cursor
}