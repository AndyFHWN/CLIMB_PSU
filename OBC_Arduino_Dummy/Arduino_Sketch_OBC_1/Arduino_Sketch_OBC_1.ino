#include<Wire.h>

uint8_t datavector;
uint8_t datavector2[]={1,2,3,4,5};
const int CC2_addr=0b1010101;
//const int CC2_addr=0xAA;
const int dummy=0b1000001; 
int x = 0;

void setup(){
  Wire.begin();
  Serial.begin(9600);
 
}

void loop(){
   //Serial.print("start\n");
   Wire.beginTransmission(CC2_addr);  //prepares data transmission to given adress
   Wire.write(x);                     //saves value/data to the send buffer
   Wire.endTransmission();            //send what is saved in buffer
   Wire.requestFrom((int)(CC2_addr), 1);  //Requests the specified number of bytes from the specified device
   datavector = Wire.read();          //reads vaule/data from receive buffer
   Serial.print(datavector);
   Serial.print("\n");
   
   if(x<65){   //Workaround because wire.write does not work in loop (except void loop())
     x++;
   }
   else{
     x=0;
   Serial.print("end\n");
   }
  
    /*for(int k=0; k<=5; k++){
     Wire.write(1);          //unfortunately wire.write does not work in loop -> maybe erase buffer
   }
    /* Wire.endTransmission();
     Wire.requestFrom((int)(CC2_addr), 1);
     Serial.print(k);
     Serial.print("\n");
     datavector2[k] = Wire.read();
     //Serial.print(datavector2[k]);
     delay(1000);
   }*/
  
 }
  delay(10);
}
