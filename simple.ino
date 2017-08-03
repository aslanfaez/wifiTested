

#define TIMEOUT 5000 // mS
#define LED 13


void setup()
{
 pinMode(LED,OUTPUT);

 Serial.begin(115200);
 Serial3.begin(115200);
 SendCommand("AT+RST", "Ready");
 delay(5000);
 //SendCommand("AT+CIOBAUD=115200","OK");
//delay(3000);
 //SendCommand("AT+CWMODE=1","OK");
//delay(3000);
 //SendCommand("AT+CWJAP=\"aslan350\",\"9124598001\"" ,"OK");
//delay(3000);
 SendCommand("AT+CIFSR", "OK");
 SendCommand("AT+CIPMUX=1","OK");
 SendCommand("AT+CIPSERVER=1,8","OK");
}
 
void loop(){
 
 String IncomingString="";
 boolean StringReady = false;
 
 while (Serial3.available()){
   IncomingString=Serial3.readString();
   StringReady= true;
  }
 
  if (StringReady){
    Serial.println("Received String:");
    Serial.println(IncomingString);

  if (IncomingString.indexOf("+IPD") != -1) {
    int con;
    int cmd=-1;
    int arg1=-1;
    int arg2=-1;
    int arg3=-1;
     parser(IncomingString,&con,&cmd,&arg1,&arg2,&arg3);
     SendData("hooloo","SEND OK",con);
     Serial.print("con : ");
     Serial.print(con);
     Serial.println("");
     Serial.print("cmd : ");
     Serial.print(cmd);
     Serial.println("");
     Serial.print("arg1 : ");
     Serial.print(arg1);
     Serial.println("");
     Serial.print("arg2 : ");
     Serial.print(arg2);
     Serial.println("");
     Serial.print("arg3 : ");
     Serial.print(arg3);
     Serial.println("");

  }
 


    
  }
 }

void parser(String txt,int* _con,int* _cmd,int* _arg1,int* _arg2,int* _arg3)
{
    int len = txt.length();
    int index=0;
    bool insert=false;
    String ans = "";
    for(int i =0 ;i<len ; i++){
        bool spliter=txt[i]==','||txt[i]==':';
        if(spliter){
            insert = false;
            if(index == 1){
                *_con = atoi(ans.c_str());
            }
            if(index == 3){
                *_cmd = atoi(ans.c_str());
            }
            if(index == 4){
                *_arg1 = atoi(ans.c_str());
            }
            if(index == 5){
                *_arg2 = atoi(ans.c_str());
            }
            if(index == 6){
                *_arg3 = atoi(ans.c_str());
            }
            ans="";
        }
        if(insert)
        {
            ans += txt[i];
        }
        if(spliter){
            insert = true;
            index++;
        }
    }
}
boolean SendData(String cmd, String ack,int con){
  int len=cmd.length();
  len = len+1;
  String l = String(len);
  SendCommand("AT+CIPSEND="+String(con)+","+l,"OK");
  Serial3.println(cmd+"\n"); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}
boolean SendCommand(String cmd, String ack){
  Serial3.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}
 
boolean echoFind(String keyword){
 byte current_char = 0;
 byte keyword_length = keyword.length();
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline){
  if (Serial3.available()){
    char ch = Serial3.read();
    Serial.write(ch);
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       Serial.println();
       return true;
    }
   }
  }
 return false; // Timed out
}
