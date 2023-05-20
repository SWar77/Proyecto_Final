#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2          // Pin digital 2 del Arduino para el sensor DHT11
#define RELAYPIN 13       // Pin digital 13 del Arduino para el relé
#define DHTTYPE DHT11     // Tipo de sensor DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección del LCD y dimensiones (16 columnas, 2 filas)

const float HIGH_TEMPERATURE_THRESHOLD = 31.0;  // Umbral de temperatura alta en grados Celsius

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();         // Inicializar la pantalla LCD
  lcd.backlight();    // Encender la retroiluminación del LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  delay(2000);        // Esperar a que la pantalla LCD se inicialice completamente
  lcd.clear();        // Limpiar la pantalla LCD
  pinMode(RELAYPIN, OUTPUT); // Configurar el pin del relé como salida
}

void loop() {
  float temperature = dht.readTemperature(); // Leer temperatura en grados Celsius
  float humidity = dht.readHumidity();       // Leer humedad relativa
  
  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(7, 0);
    lcd.print("ERR");  // Mostrar "ERR" en el LCD si hay un error en la lectura del sensor
    lcd.setCursor(5, 1);
    lcd.print("ERR");
  } else {
    // Mostrar la temperatura en grados Fahrenheit y Celsius en el LCD
    lcd.setCursor(10, 0);
    lcd.print(temperature, 1);
    lcd.print("C   ");
    lcd.setCursor(0, 0);
    lcd.print((temperature * 9 / 5) + 32, 1);
    lcd.print("F   ");
    
    // Mostrar la humedad relativa en el LCD
    lcd.setCursor(5, 1);
    lcd.print(humidity, 1);
    lcd.print("%   ");
    
    // Verificar si la temperatura supera el umbral
    if (temperature > HIGH_TEMPERATURE_THRESHOLD) {
      digitalWrite(RELAYPIN, HIGH);  // Activar el relé si la temperatura es superior al umbral
    } else {
      digitalWrite(RELAYPIN, LOW);   // Desactivar el relé si la temperatura está por debajo del umbral
    }
  }
  
  // Mostrar la temperatura en el Monitor Serial
  Serial.print("Temperatura: ");
  Serial.print(temperature, 1);
  Serial.println(" °C");
  
  delay(2000);  // Esperar 2 segundos entre lecturas
}

