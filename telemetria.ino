#include <Wire.h>
#include <HMC5883L.h>
HMC5883L brujula; //declaramos la variable
int error = 0; // 
char findString, rmc[100],data,hora[11],lat[10],lon[11];
char vel[6],fecha[7],senLN, senLT;
int contComa=0,cH=0,cLT=0,cLN=0,cVel=0,cFecha=0;
float gLat,sLat,gLon,sLon,convVel;

void setup()
{
  Serial.begin(9600);
  Wire.begin(); // iniciamos el interfaz I2C
 brujula = HMC5883L(); // iniciamos la instancia de la brujula
 error = brujula.SetScale(1.3); // indicamos la escala que vamos a usar
 if(error != 0){
   Serial.println(brujula.GetErrorText(error));
 }
 error = brujula.SetMeasurementMode(Measurement_Continuous);
 if(error != 0){
  Serial.println(brujula.GetErrorText(error));
 }
}
void loop()
{
     MagnetometerRaw raw = brujula.ReadRawAxis(); // recibimos el valor del RAW sin escalar
 MagnetometerScaled scaled = brujula.ReadScaledAxis(); // recibimos el valor escalado con la configuracion eleg


  stringRMC();    //funcion para tomar la cadena GPRMC
  cH=0;
  cLT=0;
  cLN=0;
  cVel=0;
  cFecha=0;
  contComa=0;
  
  while(1)
  {
 
    if(Serial.available()>0)    //Si recibimos algun valor de GPS
    {
      
      data=Serial.read();      //Leemos el caracter
      if(data==',')            //Detectamos las comas para saber que tipo de datos se toman 
      contComa++;              //Contamos el numero de comas
      else if(contComa==1)      //Despues de la coma uno tenemos Hora
      {
        hora[cH]=data;         //Hora se almacena como un vector
        cH++;
      }
      else if (contComa==3)    //Despues de la coma 3 tenemos Latitud
      {
        lat[cLT]=data;          //Latitud se almacena como un vector
        cLT++;
      }
      else if (contComa==4)    //Despues de la coma 4 tenemos Direccion
         senLT=data;
      else if (contComa==5)    //Despues de la coma 5 tenemos Longitud
      {
        lon[cLN]=data;         //Longitud se almacena como un vector
        cLN++;
      }
      else if (contComa==6)    //Despues de la coma 6 tenemos sentido de la Longitud
        senLT=data;  
      else if (contComa==7)    //Despues de la coma 7 tenemos Velocidad
      {
        vel[cVel]=data;        //Velocidad se almacena como un vector
        cVel++;
      }
      else if (contComa==9)    //Despues de la coma 9 tenemos Fecha
      {
        fecha[cFecha]=data;    //Fecha se almacena como un vector
        cFecha++;
      }
      if (data=='*')
      break;
    }
  }
  
  //Hora GTM-> formato hh:mm:ss.ss
     Serial.print("Hora: ");
     Serial.print(hora[0]);
     Serial.print(hora[1]);
     //Minutos
     Serial.print(":");
     Serial.print(hora[2]);
     Serial.print(hora[3]);
     //Segundos
     Serial.print(":");
     Serial.print(hora[4]);
     Serial.print(hora[5]);
     
   //Latitud->Formato gg°mm'ss"
   Serial.print("Latitud: ");
   //Obtenion de segundos
   gLat=atof(lat);
   sLat=(gLat-floor(gLat))*60;
   //Grados
   Serial.print(lat[0]);
   Serial.print(lat[1]);
   Serial.print(" ");
   //Minutos
   Serial.print(lat[2]);
   Serial.print(lat[3]);
   Serial.print("'");
   //Segundos
   Serial.print(sLat);
   Serial.print("''");
   //Sentido
   Serial.print(senLT);
     
   //Longitud->Formato gg°mm'ss"
   Serial.print("Longitud: ");
   //Obtenion de segundos
   gLon=atof(lon);
   sLon=(gLon-floor(gLon))*60;
   //Grados
   Serial.print(lon[0]);
   Serial.print(lon[1]);
   Serial.print(lon[2]);
   Serial.print(" ");
   //Minutos
   Serial.print(lon[3]);
   Serial.print(lon[4]);
   Serial.print("'");
   //Segundos
   Serial.print(sLon);
   Serial.print("''");
   //Sentido
   Serial.print(senLN);
   
   //Velocidad en Km/h
   Serial.print("Velocidad: ");
   //Conversion de nudos a Km/h, 1 nudo=1.852 Km/h
   convVel=atof(vel)*1.852;
   //Velocidad en nudos
   Serial.print(convVel);
   Serial.print("Km/h");
   
   //Fecha->DD/MM/AA
   Serial.print("Fecha: ");
   Serial.print(fecha[0]);
   Serial.print(fecha[1]);
   Serial.print("/");
   Serial.print(fecha[2]);
   Serial.print(fecha[3]);
   Serial.print("/");
   Serial.print(fecha[4]);
   Serial.print(fecha[5]);
   Serial.print("");
 float angulo = atan2(scaled.YAxis, scaled.XAxis) * (180 / 3.14159265) + 180; // arcotangete x y. sacamos el angulo en grado
 
    if((angulo < 22.5) || (angulo > 337.5 )){
   Serial.println("Sur");
 }
 if((angulo > 22.5) && (angulo < 67.5 )){
   Serial.println("Suroeste");
 }
 if((angulo > 67.5) && (angulo < 112.5 )){
   Serial.println("Oeste");
 }
 if((angulo > 112.5) && (angulo < 157.5 )){
   Serial.println("Noroeste");
 }
 if((angulo > 157.5) && (angulo < 202.5 )){
   Serial.println("Norte");
 }
 if((angulo > 202.5) && (angulo < 247.5 )){
   Serial.println("Noreste");
 }
 if((angulo > 247.5) && (angulo < 292.5 )){
   Serial.println("Este");
 }
 if((angulo > 292.5) && (angulo < 337.5 )){
   Serial.println("Sureste");
 }
   
}
//Funcin que detecta la cadena $GPRMC
void stringRMC()
{
  while(true)
  {
    if (Serial.available()>0)
    {
      findString=Serial.read();
      if(findString=='R')
      {
        while(Serial.available()==0);
        findString=Serial.read();
        if(findString=='M')
        {
          while(Serial.available()==0);
          findString=Serial.read();
          if(findString=='C')
          break;
        }
      }
    }
  }
}
