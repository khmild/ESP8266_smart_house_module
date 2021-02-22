// Инициализация FFS
void FS_init(void) {
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
    }
  }
  HTTP.on("/list", HTTP_GET, handleFileList);
  HTTP.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) HTTP.send(404, "text/plain", "FileNotFound");
  });
  HTTP.on("/edit", HTTP_PUT, handleFileCreate);
  HTTP.on("/edit", HTTP_DELETE, handleFileDelete);
  HTTP.on("/edit", HTTP_POST, []() {
    HTTP.send(200, "text/plain", "");
  }, handleFileUpload);
  HTTP.onNotFound([]() {
    if (!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "FileNotFound");
  });
}


String getContentType(String filename) {
  if (HTTP.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = HTTP.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}


void handleFileUpload() {
  if (HTTP.uri() != "/edit") return;
  HTTPUpload& upload = HTTP.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
  }
}

void handleFileDelete() {
  if (HTTP.args() == 0) return HTTP.send(500, "text/plain", "BAD ARGS");
  String path = HTTP.arg(0);
  if (path == "/")
    return HTTP.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return HTTP.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  HTTP.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate() {
  if (HTTP.args() == 0)
    return HTTP.send(500, "text/plain", "BAD ARGS");
  String path = HTTP.arg(0);
  if (path == "/")
    return HTTP.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return HTTP.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return HTTP.send(500, "text/plain", "CREATE FAILED");
  HTTP.send(200, "text/plain", "");
  path = String();

}

void handleFileList() {
  if (!HTTP.hasArg("dir")) {
    HTTP.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = HTTP.arg("dir");
  Dir dir = SPIFFS.openDir(path);
  path = String();
  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  output += "]";
  HTTP.send(200, "text/json", output);
}


void save_json(const char *filename, const configurations &conf){
  /*
  -
  -  TODO : 
  -       CODE FOR WORKING WITH SPIFFS
  -       OPEN/CREATE FILE
  -
  */

  StaticJsonDocument<512> doc;

  doc["ssid"] = conf.ssid;
  doc["password"] = conf.password;
  doc["mqttUser"] = conf.mqttUser;
  doc["mqttPassword"] = conf.mqttPassword;
  doc["mqttContrIP"] = conf.mqttContrIP;
  doc["mqttContrPort"] = conf.mqttContrPort;
  doc["mqttContrSub"] = conf.mqttContrSub;
  doc["mqttContrPub"] = conf.mqttContrPub;

  /*if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  file.close();*/
  
}




//================ EEPROM FUNCTIONS (TO BE RETIRED) ================//

void load_settings(){ // to be retired
  
    settings.ssid = read_eeprom(0);
    settings.password = read_eeprom(20);
    settings.mqttUser = read_eeprom(40);
    settings.mqttPassword = read_eeprom(60);
    settings.mqttContrIP = read_eeprom(80);
    settings.mqttContrPort = read_eeprom(100);
    settings.mqttContrSub = read_eeprom(120);
    settings.mqttContrPub = read_eeprom(180);
  
  }

void save_settings(){ // to be retired
  
  Serial.println(settings.ssid);
  Serial.println(settings.password);
  Serial.println(settings.mqttUser);
  Serial.println(settings.mqttPassword);
  Serial.println(settings.mqttContrIP);
  Serial.println(settings.mqttContrPort);
  Serial.println(settings.mqttContrSub);
  Serial.println(settings.mqttContrPub);


  write_eeprom(settings.ssid, 0);
  write_eeprom(settings.password, 20);
  write_eeprom(settings.mqttUser, 40);
  write_eeprom(settings.mqttPassword, 60);
  write_eeprom(settings.mqttContrIP, 80);
  write_eeprom(settings.mqttContrPort, 100);
  write_eeprom(settings.mqttContrSub, 120);
  write_eeprom(settings.mqttContrPub, 180);
  
  }
  
void write_eeprom(String buff, int addr){ // to be retired
  EEPROM.begin(512);
  delay(10);
  unsigned int len = buff.length();
  EEPROM.write(addr, len);
  
  for (int i = 0; i < len; ++i) 
    {
      EEPROM.write(addr + i + 1, buff[i]);
    }
  EEPROM.commit();
  }

String read_eeprom(int addr){ // to be retired
  EEPROM.begin(512); 
  delay(10); 
  unsigned int len = EEPROM.read(addr);
  String buff = "";
  for (int i = addr + 1; i < (addr + len + 1); ++i)
    {
          buff += char(EEPROM.read(i));
    }
  return buff;
  }

//================ EEPROM FUNCTIONS (TO BE RETIRED) ================//
