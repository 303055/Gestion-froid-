#include "EEPROM.h" //Librairie Eeprom
#include <Encoder.h> //Librairie Encodeur
#include <OneWire.h> //Librairie du bus OneWire pour le Capteur
#include <DallasTemperature.h> //Librairie du Capteur
#include <Wire.h> //Librairie du Capteur
#include <LiquidCrystal_I2C.h>//Librairie I2C
#include <avr/wdt.h> // Librairie watchdog
DallasTemperature sensorsint(& oneWire); // Utilistion du bus Onewire pour les capteurs
DeviceAddress sensorDeviceAddress; // Vérifie la compatibilité des capteurs avec la librairie
LiquidCrystal_I2C lcd(0x3F, 16, 2);//0x27 regarder la doc constructeur du lcd 


/*Entrée*/
Encoder myEnc(2, 3); //pinS pour l'encodeur
OneWire oneWire(7); // Bus One Wire sur la pin 7 de l'arduino
byte bp = 6; //Déclaration du bouton poussoir
byte bp = 6; //Déclaration du bouton poussoir


/*Sortie*/
byte compresseur = 5;//Déclaration de la pin pour la compresseur



/**/

int a = 20; // adresse de la case Eeprom
float consigne; //consigne de temperature
float temperature; //Déclaration de la T°
float hysteresy = 0.50; //Déclaration de l'hysteresy
byte etatbp; //Déclaration pour l'état du bouton poussoir
float positionEndodeur;




void setup()

{
	//EEPROM.write(a, 80);// pur initialisé au programme a 20°
	consigne = EEPROM.read(a);
	myEnc.write(consigne*4);
	pinMode(bp, INPUT); // 
	pinMode(compresseur, OUTPUT);
	digitalWrite(compresseur, 0);
	lcd.init(); //initialisation du lcd 
	lcd.backlight(); //initialisationdu lcd 
	Serial.begin(9600); // Permet la communication en serial
	sensorsint.begin(); // Activation des capteurs
	sensorsint.getAddress(sensorDeviceAddress, 0); // Demande l'adresse du capteur à l'index 0 du bus
	sensorsint.setResolution(sensorDeviceAddress, 10); // Résolutions possibles: 9,10,11,12 respectivement  0.5C, 0.25C, 0.125C or 0.0625C
	wdt_enable(WDTO_8S); // watchdog fixé à a 8s (8s maxi possible)
}


float oldPosition = -999;

void loop()
{
  
	wdt_reset(); 

	etatbp = digitalRead(bp); 
	positionEndodeur = myEnc.read()/4;


		sensorsint.requestTemperatures(); // Demande la température aux capteurs
		temperature = sensorsint.getTempCByIndex(0);

	consigne = positionEndodeur/2;



	if (etatbp == 0) 
	{
		EEPROM.write(a, positionEndodeur);
   while(digitalRead(bp)==0){};

		
   
	}  
	if (temperature <= consigne - hysteresy) 
	{
		digitalWrite(compresseur, 0);
		lcd.setCursor(13, 1); 
		lcd.print("off");
	}

	if (temperature >= consigne + hysteresy)
	{
		digitalWrite(compresseur, 1);
		lcd.setCursor(13, 1); 
		lcd.print("on ");
	}

	lcd.print("  ");
	lcd.setCursor(0, 0);
	lcd.print("T:");
	lcd.print(temperature);
	lcd.print("  ");
  
	lcd.setCursor(0, 1); 
  lcd.print("C:");
	lcd.print(consigne);
	lcd.print("  ");
  
	delay(10);
}

void reboot()
{
	while (1);
}
