// temp senzor: https://randomnerdtutorials.com/arduino-lm35-lm335-lm34-temperature-sensor/



// Taski:
//  mer temperaturo
//  spreme regulacijo
//  prdob podatke iz 


void setup(){
    pinMode(sensorPin, INPUT);
    Serial.begin(115200);
}

int readTempInCel(){
    return analogRead(sensorPin)/10 -273;
}

int temp_wanted = INITIAL_TEMPERATURE;

int TOO_HOT = false;

void loop(){
    int temp_floor_inlet    = getFloorInletTemp();
    int temp_floor_outlet   = getFloorOutletTemp();
    int temp_furnice        = getFurniceTemp();

    control(temp_wanted, temp_floor_inlet, temp_floor_outlet, temp_furnice);

    int serialData[2];
    getFromSerial(&serialData);

    if(getFloorInletTemp() >= MAX_INLET_TEMP){
        TOO_HOT = true;
    }

    /*int tempOnLine = readTempInCel();
    
    // prižgi pumpo ko je temperatura manjša od nastavljene
    if(tempOnLine < temperatureSet){
        digitalWrite(pumpPin,1);
    }


    if(abs(temperatureSet-tempOnLine) <= 3){
        // smo v območju histereze, dela naj samo pumpa
        digitalWrite(lowerTempPin, 0);
        digitalWrite(increaseTempPin, 0);
    }else if(tempOnLine < temperatureSet) {
        // premau je tkoda je treba odpret ventil
        digitalWrite(lowerTempPin,0);
        digitalWrite(increaseTempPin,1);
    }else if(tempOnLine > temperatureSet){
        if(tempOnLine > temperatureSet+4){
            digitalWrite(pumpPin,0);
        }
        digitalWrite(increaseTempPin,0);
        digitalWrite(lowerTempPin,1);
    }
    
    Serial.print("Temperature (°C): ");
    Serial.println(tempOnLine);
    Serial.print("pin to lower temp set to: ");
    Serial.println(digitalRead(lowerTempPin));
    Serial.print("pin to increase temp set to: ");
    Serial.println(digitalRead(increaseTempPin));
    Serial.print("pin for pump set to: ");
    Serial.println(digitalRead(pumpPin));

    if(digitalRead(lowerTempPin) && digitalRead(increaseTempPin)){
        digitalWrite(lowerTempPin,1);
        digitalWrite(increaseTempPin,0);
    }
    char savingChars[3];
    while (Serial.available() > 0) {
    // read the incoming byte:
        int incomingByte = Serial.read();
        if(incomingByte == 'T'){
            tempRecieving = 1;
        }else if(tempRecieving == 1){
            savingChars[0] = incomingByte;
            tempRecieving++;
        }else if(tempRecieving == 2){
            savingChars[1] = incomingByte;
            savingChars[2] = 0;
            tempRecieving++;
        }else{
            tempRecieving = -1;
        }

    }
    if(tempRecieving == 3){
        String str(savingChars);
        str = savingChars;
        temperatureSet = str.toInt();
        tempRecieving = 0;
    }else if(tempRecieving == 0){
        savingChars[0] = 0;
        savingChars[1] = 0;
        savingChars[2] = 0;
    }else{
        Serial.println("Za nastavitev temperature vpišite: T{temp} npr.: T20");
        tempRecieving=0;
    }
    delay(10);*/




}
