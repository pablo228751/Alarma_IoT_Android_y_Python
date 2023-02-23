//#include <avr/wdt.h>

//int pushSensor2 = 2;
int pushSensor3 = 3;
//int pushReleWIFI = 4;
int rele_5 = 5;
int rele_8 = 8;
//bool sensor2 = false;
bool sensor3 = false;
bool releWIFI = false;
bool dispara=false;
bool apagar_remotamente=false;
const int vueltasLOW=3; //Cantidad de LOW que tiene que acumular antes de disparar alarma
const int vueltasHIGH=5; //Cantidad de LOW que tiene que acumular antes de disparar alarma
int contadorLOW=vueltasLOW;
int contadorHIGH=vueltasHIGH;
const int v_sound=500000;
//const int v_sound=10000;
int vueltas_sonando=v_sound;
int contador_Alarma=3;
int contador_Tiempo=80;

void setup()
{
    //wdt_disable();
    delay (5000);
    Serial.begin(9600);
    pinMode(pushSensor3, INPUT);
    pinMode(rele_5, OUTPUT); // Para sirenas y luces
    pinMode(rele_8, OUTPUT); //Avisa cuanddo se dispara la alarma
    //wdt_enable(WDTO_8S);
}

void loop()
{
    sensor3 = digitalRead(pushSensor3);
    if(sensor3 == LOW)
    {
      Serial.print("Esta en lowwwwwww unoooooooooo ");
            Serial.println(contador_Alarma);
        while(contador_Tiempo > 0)
        {
            sensor3 = digitalRead(pushSensor3);
            if(sensor3 == LOW)
            {
                contador_Alarma --;
            }
            Serial.print("en WHILEEE Contador_Alarma: ");
            Serial.println(contador_Alarma);
            Serial.print("en WHILEEE Contador Tiempo: ");
            Serial.println(contador_Tiempo);

            contador_Tiempo --;

        }
        if (contador_Alarma < 0)
        {
          Serial.print("Entre aquiii se disparaa: ");
            Serial.println(contador_Alarma); 
            dispara=true;

        }
        contador_Alarma=3;
        contador_Tiempo=80;


    }    
    if(dispara == true)
    {
        
        digitalWrite(rele_8, HIGH);
        delay(100);
        digitalWrite(rele_5, HIGH);
        Serial.println("Se disparo alarma");
        delay(v_sound);
        dispara == false;

    }
    else
    {
        digitalWrite(rele_8, LOW);
        delay(100);
        digitalWrite(rele_5, LOW);
    }
    if (sensor3 == HIGH )
    {
        dispara=false;
    }
    //Serial.print("Estado dispara: ");
    //Serial.println(dispara);
    //Serial.print("Estado SENSOR 3: ");
    //Serial.println(sensor3);
    //Serial.println("");
    //wdt_reset();
    delay(50);
}
