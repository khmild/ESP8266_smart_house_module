

void FS_init(void) {
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    
    #ifdef DEBUG
      Serial.println();
      Serial.println("Configuration file containes:");
      printFile();
      Serial.println();
      Serial.println("Files in SPIFFS:");
    #endif
    
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      
      #ifdef DEBUG
        Serial.println(fileName);
      #endif
    }
  }
  
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


void save_json(const configurations &conf){

  SPIFFS.remove("/config.txt");
  
  File file = SPIFFS.open("/config.txt", "w");
  if (!file) {
    Serial.println("Config file open failed (write)");
  }

  StaticJsonDocument<512> doc;

  doc["ssid"] = conf.ssid;
  doc["password"] = conf.password;
  doc["mqttUser"] = conf.mqttUser;
  doc["mqttPassword"] = conf.mqttPassword;
  doc["mqttContrIP"] = conf.mqttContrIP;
  doc["mqttContrPort"] = conf.mqttContrPort;
  doc["mqttContrSub"] = conf.mqttContrSub;
  doc["mqttContrPub"] = conf.mqttContrPub;

  #ifdef DEBUG
    Serial.println();
    Serial.println("Saving this data to json:");
    Serial.println(settings.ssid);
    Serial.println(settings.password);
    Serial.println(settings.mqttUser);
    Serial.println(settings.mqttPassword);
    Serial.println(settings.mqttContrIP);
    Serial.println(settings.mqttContrPort);
    Serial.println(settings.mqttContrSub);
    Serial.println(settings.mqttContrPub);
    Serial.println();
  #endif

  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write to file");
  }

  file.close();
  
}

void load_json(configurations &conf) {
  
  File file = SPIFFS.open("/config.txt", "r");
  if (!file) {
    Serial.println("Config file open failed (read)");
  }

  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println("Failed to read file, using default configuration");

  const char *ssid = doc["ssid"];
  const char *password = doc["password"];
  const char *mqttUser = doc["mqttUser"];
  const char *mqttPassword = doc["mqttPassword"];
  const char *mqttContrIP = doc["mqttContrIP"];
  const char *mqttContrPort = doc["mqttContrPort"];
  const char *mqttContrSub = doc["mqttContrSub"];
  const char *mqttContrPub = doc["mqttContrPub"];

  conf.ssid = ssid;
  conf.password = password;
  conf.mqttUser = mqttUser;
  conf.mqttPassword = mqttPassword;
  conf.mqttContrIP = mqttContrIP;
  conf.mqttContrPort = mqttContrPort;
  conf.mqttContrSub = mqttContrSub;
  conf.mqttContrPub = mqttContrPub;

  #ifdef DEBUG
  Serial.println();
  Serial.println("Loading this data from json:");
  Serial.println(settings.ssid);
  Serial.println(settings.password);
  Serial.println(settings.mqttUser);
  Serial.println(settings.mqttPassword);
  Serial.println(settings.mqttContrIP);
  Serial.println(settings.mqttContrPort);
  Serial.println(settings.mqttContrSub);
  Serial.println(settings.mqttContrPub);
  Serial.println();
  #endif

  file.close();
}

#ifdef DEBUG
void printFile() {
  File file = SPIFFS.open("/config.txt", "r");
  if (!file) {
    Serial.println("Failed to read file");
    return;
  }

  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  file.close();
}
#endif
