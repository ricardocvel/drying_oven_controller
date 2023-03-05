# Drying Oven Controller

- This is a controller for an oven used to dry 3D printer filaments.
- Esse é um controlador para estufa utilizada para secagem de filamentos de impressora 3D.

    ![control](https://raw.githubusercontent.com/ricardocvel/drying_oven_controller/master/outers/images/controller_for_ovens.png)
# Hardware 

- For this controller, I used materials that I had at home. It is based on Arduino Nano and LM35 sensor, and it has a 16x2 LCD display with driver.
However, it can be directly connected to the controller's ports and have its code adapted. Feel free to improve the code.

- Para esse controlador utilizei materiais que eu tinha em casa, ele é baseado em arduino nano e Sensor LM35, possui display de LCD 16x7 com driver,
porem pode ser ligado direto nas portas do controlador e ter seu código adaptado, fique a vontade para melhorar o código.

# A complete list of materials and the project can be found at:
- Project

  ![Project](https://raw.githubusercontent.com/ricardocvel/drying_oven_controller/master/outers/hardware/hardware.JPG)

- List of materials:

    | Quantity | Description |
    |------|-------|
    |1 | Arduino Nano|
    |4 | 10k resistors|
    |5 | PushButton|
    |1 | LM35|
    |1 | LCD16x2 with PCF8574 driver|
    |2 | PC81C|
    |2 | 330R resistors|
    |2 | 12v coil relays|
    |4 | OUT terminals (\FAN-\FAN+\RES+RES-)    |
    |4 | IN terminals (5v +\5v -\12v +\12v-)|
    OBS	In the next version, I will remove the 5V input and add a voltage regulator.