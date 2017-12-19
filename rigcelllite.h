#ifndef RIGCELLLITE_h
#define RIGCELLLITE_h

#include <Arduino.h>
#include <SoftwareSerial.h>



class RIGCELLLITE
{
public:
    void init();
    char response();
    void push(char w);
    void clean();
    String command(String c);
    boolean finding(String object,String object_find);
    String find_between_string(String tgt,char st, char en);
    String signals();
    String online();
    String ip();
    String loctime();
    String iottest();
    String iot(String id, String dat, String type);
    void wake();

    
    bool response_done;
    

};


#endif
