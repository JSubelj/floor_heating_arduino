

void getFromSerial(int * serialData){
    int new_temp;
    *serialData = 0;
    if(Serial.avalible()>0){
        String str = Serial.ReadString();
        
        if(str[0] == 't'){
            new_temp = atoi(str[1])*10+atoi(str[2]);
            *serialData = 1;
            *(serialData+1) = new_temp;
            if(str[3]!= ";"){
                *serialData = 0;
                Serial.writeln("Ukaz za spremebo temperature: 't{dvo mesta številka};'");
            }
            if(new_temp > 55){
                *serialData = 0;
                Serial.writeln("Temperatura ne sme biti višja kot 55 stopinj!");
            }
            if(*serialData){
                Serial.write("Temperatura je nastavljena na ");
                Serial.write(*(serialData+1));
                Serial.write(" °C");
            }
        }else{
            Serial.writeln("Ukaz za spremembo temperature: 't{dvo mesta številka};'");
        }
        
    }

}


