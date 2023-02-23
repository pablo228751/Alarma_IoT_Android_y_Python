#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>


const uint16_t port = 2121;
const char *host = "www.tu_hosting.com.ar";
int cuarentaMin=250000;
int vuelta=cuarentaMin;
bool sirena=false;
bool alarma=false;
const int rele=5;
const int entrada=12;
bool leerEntrada=0;
bool informo=false;
int contador=5;


WiFiClient client;
void setup()
{
    
    Serial.begin(9600);
    Serial.println("Conectando...\n");
    WiFi.mode(WIFI_STA);
    WiFi.begin("TU_RED_WIFI", "tuContraseña"); // ingresar datos de la red
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    pinMode(rele,OUTPUT);// pin D1(GPIO5) como Salida
    pinMode(entrada, INPUT);// pin D7(GPIO1) como entrada
}

void loop()
{
  String cadena="";
    if (!client.connect(host, port))
    {
        Serial.println("No se pudo conectar...");
        delay(1000);
        return;
    }
    Serial.println("Conectado al servidor");
    client.println("estado2");
    delay(250);
    /*
     * Si los Pines estan en LOW activar: "dispara=SI"
     */
    while (client.available() > 0)
    {        
        char c = client.read();
        //Serial.write(c);
        cadena +=c;    
    if(cadena == "AlarmActivada=NO"){
      Serial.println("La alarma esta Desactivada");
      alarma=false;
      informo=false;
      contador=5;
      cadena="";
      }
    else if (cadena == "AlarmActivada=SI"){
      Serial.println("La alarma esta ACTIVADA");
      alarma=true;
      cadena="";
      }
    if (cadena == "SirenaActivada=SI"){
      Serial.println("La Sirena esta sonando");
      sirena=true;      
      cadena="";
      digitalWrite(rele, HIGH);
    
      }
    else if (cadena == "SirenaActivada=NO"){
      sirena=false;
      Serial.println("La Sirena esta apagada");
      cadena="";
      }
        
    }

    if(sirena==true){
      vuelta -= 1;
      Serial.print("Vuelta Num: ");
      Serial.println(vuelta);
      }else{
        digitalWrite(rele, LOW);
        }

    
    //client.stop();
    delay(5000);    
    if (vuelta <= 0){
      vuelta=cuarentaMin;
      }
}
