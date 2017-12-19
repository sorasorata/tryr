#include "rigcelllite.h"


#define GSM_STATUS 6
#define GSM_PWRKEY 7
#define GSM_TXD 4
#define GSM_RXD 2

SoftwareSerial sim(GSM_TXD, GSM_RXD);

void RIGCELLLITE::init(){
  
  sim.begin(9600);
  response_done = false;
  pinMode(GSM_PWRKEY,OUTPUT);
  pinMode(GSM_STATUS,INPUT);
  
  digitalWrite(GSM_PWRKEY,LOW);
  if(!digitalRead(GSM_STATUS))
  {
    digitalWrite(GSM_PWRKEY,HIGH);
    while(1)
    {
      if(digitalRead(GSM_STATUS))
      {
        digitalWrite(GSM_PWRKEY,LOW);
        break;
      }
    }
  }

}

char RIGCELLLITE::response(){

  if(sim.available()>0)
  {
    char c = sim.read();
    return c;
  }
  else
  {
    return 0x00;
  }
}


String RIGCELLLITE::signals(){
  String res;
  sim.println("AT+CBC\r");
  String im;
  while(1)
  {
    while(!sim.available());
    if(sim.find("CBC:"))
    {
      im = sim.readStringUntil('\n');
      im = find_between_string(im,',',',');
      res= im;
      break;
    }  
    else 
    {
      res="ERROR";
      break;  
    }
  }
  return res;
}

String RIGCELLLITE::ip(){
  String res;

  String Data = "";
  sim.println("AT+SAPBR=2,1\r");//turn on GPRS
  while(1)
  {
      if(sim.available())
      {
        char c = sim.read();
        Data += c;
        if (c == '\n')
        {
          if(finding(Data,"1,1"))
          {
            res = find_between_string(Data,'"','"');
            break;
          }
          else if(finding(Data,"1,3"))
          {
            res = "OFFLINE";
            break;
          }
        }
      }   

  }
  return res;
}

String RIGCELLLITE::loctime(){
  String res;
  String Data = "";
  sim.println("AT+CIPGSMLOC=1,1\r");//turn on GPRS
  while(1)
  {
      if(sim.available())
      {
        char c = sim.read();
        Data += c;
        if (c == '\n')
        {
          if(finding(Data,"+CIPGSMLOC:"))
          {
            res = find_between_string(Data,',','\r');
            break;
          }
          else
          {
            Data="";
          }
        }
      }   

  }
  return res; 

}

String RIGCELLLITE::iottest()
{
  String res;
  String sig = signals();
  String loct = loctime();

  sim.println("AT+HTTPINIT\r");//set gprs to GSM 
  delay(500);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  String uri = "AT+HTTPPARA=\"URL\",\"http://rig.reka.com.my/test.php\"\r";
  sim.println(uri);//set gprs server
  delay(500);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  String Data = "";
  boolean ok = false;
  sim.println("AT+HTTPACTION=0\r");//turn on GPRS
  while(1)
  {
      if(sim.available())
      {
        char c = sim.read();
        Data += c;
        if (c == '\n')
        {
          if(finding(Data,"+HTTPACTION:"))
          {
              if(finding(Data,"200"))
              {
                ok = true;
                break;
              }
              else
              {
                ok = false;
                break;
              }
          }
          else
          {
            Data = "";    
          }
        }
      }   

  }
  if(ok)
  {
    Data="";
    sim.println("AT+HTTPREAD\r");//turn on GPRS
    while(1)
    {
        if(sim.available())
        {
          char c = sim.read();
          Data += c;
          if (c == '\n')
          {
              if(finding(Data,"HTTPREAD"))
              {
                Data = "";
              }
              else
              {
                res = Data;
                break;
              }
          }
        }   
    }
  }

  sim.println("AT+HTTPTERM\r");//set gprs to GSM 
  delay(500);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  return res;
}


String RIGCELLLITE::iot(String id, String dat,String type)
{
  String res;
  String sig = signals();
  String loct = loctime();

  sim.println("AT+HTTPINIT\r");//set gprs to GSM 
  delay(500);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  String uri = "AT+HTTPPARA=\"URL\",\"http://rig.reka.com.my/dial/grab.php?id="+id+"&location="+loct+"&signal="+sig+"&sensor="+dat+"&type="+type+"\"\r";
  sim.println(uri);//set gprs server
  delay(500);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  String Data = "";
  boolean ok = false;
  sim.println("AT+HTTPACTION=0\r");//turn on GPRS
  while(1)
  {
      if(sim.available())
      {
        char c = sim.read();
        Data += c;
        if (c == '\n')
        {
          if(finding(Data,"+HTTPACTION:"))
          {
              if(finding(Data,"200"))
              {
                ok = true;
                break;
              }
              else
              {
                ok = false;
                break;
              }
          }
          else
          {
            Data = "";    
          }
        }
      }   

  }
  if(ok)
  {
    Data="";
    sim.println("AT+HTTPREAD\r");//turn on GPRS
    while(1)
    {
        if(sim.available())
        {
          char c = sim.read();
          Data += c;
          if (c == '\n')
          {
              if(finding(Data,"HTTPREAD"))
              {
                Data = "";
              }
              else
              {
                res = Data;
                break;
              }
          }
        }   
    }
  }

  sim.println("AT+HTTPTERM\r");//set gprs to GSM 
  delay(500);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  return res;
}




String RIGCELLLITE::online(){
  String res;

  sim.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");//set gprs to GSM 
  delay(1000);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  sim.println("AT+SAPBR=3,1,\"APN\",\"\"\r");//set gprs server
  delay(1000);
  while(sim.available())
  {
    String s=sim.readStringUntil('OK');
  }
  String Data = "";
  sim.println("AT+SAPBR=1,1\r");//turn on GPRS
  while(1)
  {
      if(sim.available())
      {
        char c = sim.read();
        Data += c;
        if (c == '\n')
        {
          if(finding(Data,"OK"))
          {
            res = "ONLINE";
            break;
          }
          else if(finding(Data,"ERROR"))
          {
            res = "ISONLINE";
            break;      
          }
        }
      }   

  }

  return res;
}


void RIGCELLLITE::push(char w){
  sim.write(w);
}

void RIGCELLLITE::clean(){
  sim.flush();
}

void RIGCELLLITE::wake(){
  
  for (int x = 0; x<10; x++)
  {
    sim.println("AT\r");
    delay(300);
  }
}

String RIGCELLLITE::command(String c){
  
  String res;
  c.trim();
  if(c==("hi,RIG"))
  {
    sim.println("AT\r");
    String Data = "";
    while(1)
    {
        if(sim.available())
        {
          char c = sim.read();
          Data += c;
          if (c == '\n')
          {
            if(finding(Data,"OK"))
            {
              res="Hi, I'm RIG. I'm ready";
              break;
            }
            else if(finding(Data,"ERROR"))
            {
              res="Hi, I'm NOT ready yet or I might have a prob. Try again";
              break;      
            }
          }
        }   

    }
  }
  else if(c==("iottest,RIG"))
  {
    String x = iottest();
    res = "Connect to http://rig.reka.com.my/test.php, replied: " + x;
  }
  else if(c==("signal,RIG"))
  {
      res = "My signal strength is " + signals() + " dB";
  }
  else if(c==("online,RIG"))
  {
      String is = online();   
      if (is == "ONLINE")
        res = "I'm now online";
      else
        res = "I'm already online";
  }
  else if(c==("ip,RIG"))
  {
      String ps = ip();   
      if (ps == "OFFLINE")
        res = "I'm still offline. Online first";
      else
        res = "My ip is " + ps;
  }
  else if(c==("loctime,RIG"))
  {
      String m = loctime();  
      if (m.length()< 10) 
        res = "Online first to get location";
      else
        res = m;
  }
  else
  {
    res="Unknown command, try again :)";
  }
  return res;
}


boolean RIGCELLLITE::finding(String object,String object_find)
{
  int sdf=0;
  if(object_find.length()>object.length())
  {
    return false;
  }
  for(int asd=0;asd<object.length();asd++)
  {
    if(object[asd]==object_find[sdf])
    {
      sdf++;
    }
    else
    {
      sdf=0;
    }
    if(sdf==object_find.length())
    {
      return true;
    }
  }
  return false;
}

//finding string from given char (st) to given char (en)
String RIGCELLLITE::find_between_string(String tgt,char st, char en)
{
  int stringData = 0;
  int stratidx = 0;
  boolean pass = true;
  int loc[50];
  
  for(int i = 0; i<tgt.length()-1; i++) { 

    if (pass){    
      if(tgt[i]==st) 
      {
          loc[stringData]=i;
          stringData++;
          pass=false;

       }
    }
    else
    {
      if(tgt[i]==en) 
      {
          loc[stringData]=i;
          stringData++;
          break;
       }          
    }
 
   }
   return tgt.substring(loc[0]+1,loc[1]);
  
}




