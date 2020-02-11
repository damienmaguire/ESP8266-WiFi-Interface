# ESP8266-WiFi-Interface
A universal WiFi interface using the Olimex ESP8266 module for various open source EV projects e.g. Lexus GS450H VCU.

The current version is designed to accept data from the ISA CAN shunt. Extra parameters can be aded or removed as reqired. Aiming to make this easy to configure and customise.

The information is transmitted over serial to the esp8266 at 19200 baud 8n1 in the form :
vxxx,bxxx,nxxx,ixxx,pxxx,ahxxx,kwhxxx,txxx* where :
<br>
<br>
v=v1<br>
b=v2<br>
n=v3<br>
i=current<br>
p=power<br>
ah=amp hours<br>
kwh=kilowatt hours<br>
t=temp<br>
*=end of string<br>
xxx=three digit integer for each parameter eg p100 = 100kw.<br>
updates can be every 100ms or longer.<br>
