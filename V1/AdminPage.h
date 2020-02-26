// Creates an admin page on a webserver which allows the user to update the SSID and Password
// Performs basic checks to ensure that the input values are valid

#ifndef AdminPage_h
#define AdminPage_h

//Holds the admin webpage in the program memory
const char adminPage[] PROGMEM = 
    "<html>"
      "<head>"
        "<style>input, select {font-size: 1.2em; width: 100%; max-width: 350px; display: block; margin: 5px auto; } .hidden {display:none;}</style>"
        "<script> function checkValid() { var password1 = document.getElementById('password1'); var password2 = document.getElementById('password2');"
        "if (password1.value != password2.value) { password2.setCustomValidity('Passwords must match'); } else { password2.setCustomValidity(''); }} </script>"
        "<script> function modeChange(sel) { var modeSelect = document.getElementById('mode'); var passwordConfirm = document.getElementById('password2');  var mode = modeSelect.options[sel.selectedIndex].value; if (mode == 'AP') {passwordConfirm.classList.remove('hidden')} else {passwordConfirm.classList.add('hidden')}} </script>"
      "</head>"
      "<body>"
        "<form id='form' action='/admin' method='post'>"
          "<select id='mode' name='mode' onchange='modeChange(this)'><option value='AP'>Access Point</option><option value='JOIN'>Join Network</option></select>"
          "<input name='newssid' type='text' minlength='8' maxlength='30' placeholder='New SSID'>"
          "<input name='newpassword' id='password1' type='password' minlength='8' maxlength='30' placeholder='New Password'>"
          "<input id='password2' type='password' minlength='8' maxlength='30' placeholder='Confirm Password' onchange='checkValid()'>"
          "<input type='submit' value='Update'>"
        "</form>"
      "</body>"
    "</html>";


//Creates a webpage that allows the user to change the SSID and Password from the browser
void serveAdmin(ESP8266WebServer *webServer) {
  String message;  
  Serial.println("Admin Page");

  // Check to see if we've been sent any arguments and instantly return if not
  if(webServer->args() == 0) {
    webServer->sendHeader("Content-Length", String(strlen(adminPage)));
    webServer->send(200, "text/html", adminPage);
  }
  else {      
    // Create a string containing all the arguments, send them out to the serial port
    // Check to see if there are new values (also doubles to check the length of the new value is long enough)

    if((webServer->arg("newssid").length() >= MIN_STR_LEN) &&
       (webServer->arg("newssid").length() < MAX_STR_LEN)) 
      webServer->arg("newssid").toCharArray(config.ssid, sizeof(config.ssid));

    if(webServer->arg("newpassword").length() < MAX_STR_LEN) 
      webServer->arg("newpassword").toCharArray(config.pass, sizeof(config.pass));

    if(webServer->arg("mode").length() < MAX_STR_LEN) 
      webServer->arg("mode").toCharArray(config.wifimode, sizeof(config.wifimode));

    Serial.println(config.wifimode);
    Serial.println(config.ssid);

    // Store the new settings to EEPROM
    SaveConfig();
    PrintConfig();

    // Construct a message to tell the user that the change worked
    message = "New settings will take effect after restart";     
    // Reply with a web page to indicate success or failure
    message = "<html><head><meta http-equiv='refresh' content='5;url=/' /></head><body>" + message;
    message += "<br />mode: " + String(config.wifimode);
    message += "<br />SSID: " + String(config.ssid);

    message += "<br/>Redirecting in 5 seconds...</body></html>";
    webServer->sendHeader("Content-Length", String(message.length()));
    webServer->send(200, "text/html", message);
  } 
}
#endif
    
