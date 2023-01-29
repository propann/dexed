#include "filemanager.h"

FLASHMEM void sd_filemanager() {
  char command[10];
  char path[128];

  size_t read = Serial.readBytesUntil('!', command, 10);
  command[read] = '\0';

  read = Serial.readBytesUntil('!', path, 128);
  path[read] = '\0';

  if (Serial.read() == '%') {  // check command correct
    if(!strcmp("dir", command)) {
      // list SD content for path
      sd_sendDirectory(path);
    }
    else if(!strcmp("get", command)) {
      // send file asked by remote
      sd_sendFile(path);
    }
    else if(!strcmp("put", command)) {
      // write file received from remote
      sd_receiveFile(path);
    }
    else if(!strcmp("delete", command)) {
      // delete file on SD
      sd_deleteFile(path);
    }
    else if(!strcmp("rename", command)) {
      // rename file on SD
      sd_renameFile(path); // path will be splitted
    }
  }
}

FLASHMEM void sd_sendDirectory(const char *path) {
#ifdef DEBUG
  LOG.printf("SENDDIR [%s]\n", path);
#endif

  char filename[255];
  File dir = SD.open(path);

  Serial.write(99);
  Serial.write(FM_START);
  Serial.write(88);

  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }
    if (strcmp("System Volume Information", entry.name()) == 0) {
      continue;
    }
    Serial.write(99);
    Serial.write(FM_DIR);

    strcpy(filename, entry.name());
    if (entry.isDirectory()) {
      // sd_sendDirectory(entry);
      strcat(filename, "§§");
    }
    Serial.write(strlen(filename));
    Serial.write(filename, strlen(filename));
#ifdef DEBUG
    LOG.printf("FM dir file [%s]\n", filename);
#endif
    Serial.write(88);
    entry.close();
  }
  dir.close();

  Serial.write(99);
  Serial.write(FM_END);
  Serial.write(88);

#ifdef DEBUG
  LOG.println("SENDDIR done.");
#endif
}

// send file on SD card to PC by Serial
FLASHMEM void sd_sendFile(const char *path) {
#ifdef DEBUG
  LOG.printf("SENDFILE [%s]\n", path);
#endif

  Serial.write(99);
  Serial.write(FM_START);
  Serial.write(88);

  File myFile = SD.open(path);
  if (myFile) {
    Serial.write(99);
    Serial.write(FM_SEND);
    unsigned long filesize = myFile.size();
    Serial.write(filesize);
    Serial.write(filesize >> 8);
    Serial.write(filesize >> 16);
    Serial.write(filesize >> 24);
    Serial.write(88);
    Serial.flush();

    // read from the file until there's nothing else in it:
    char buf[256];
    unsigned int n;

    while (myFile.available()) {
      Serial.write(99);
      Serial.write(FM_SEND_CHUNK);
      n = myFile.read(buf, 256);
      Serial.write(n / 256);
      Serial.write(n % 256);
      Serial.write(buf, n);
      Serial.write(88);
      Serial.flush();
    }

    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
#ifdef DEBUG
    LOG.printf("error opening [%s]\n", path);
#endif
  }

  Serial.write(99);
  Serial.write(FM_END);
  Serial.write(88);

#ifdef DEBUG
  LOG.println("SENDFILE done.");
#endif
}


// receive file from PC by Serial and write it to SD card
#define RX_BUFFER_SIZE 512

FLASHMEM void sd_receiveFile(const char *path) {

#ifdef DEBUG
  LOG.printf("RECV file, dest: [%s]\n", path);
#endif
  if (SD.exists(path)) {
    SD.remove(path);
  }

  File myFile = SD.open(path, FILE_WRITE);

  if (!myFile) {
#ifdef DEBUG
    LOG.printf("error opening %s\n", path);
#endif
  } else {

#ifdef DEBUG
    LOG.printf("Writing to %s\n", path);
#endif
    char buffer[RX_BUFFER_SIZE];
    uint32_t fileSize = 0;

    // read file size on 4 bytes
    Serial.readBytes((char *)&fileSize, 4);
#ifdef DEBUG
    LOG.printf("RECV file size : %d\n", fileSize);
#endif

    // Process with chunks
    uint32_t currentChunk = 0;
    uint32_t countTotal = 0;

    uint32_t totalChunks = ceil((float)fileSize / (float)RX_BUFFER_SIZE);
#ifdef DEBUG
    LOG.printf("number of chunks : [%d]\n", totalChunks);
#endif

    while (currentChunk < totalChunks) {
      if (Serial.available()) {
        if (currentChunk == totalChunks - 1) {
          // read last chunk
          uint16_t lastChunkSize = fileSize - (totalChunks - 1) * RX_BUFFER_SIZE;
#ifdef DEBUG
          LOG.printf("last chunk #%d: %d bytes\n", currentChunk, lastChunkSize);
#endif
          char* lastBuffer = (char*) malloc(lastChunkSize * sizeof(char));
          Serial.readBytes(lastBuffer, lastChunkSize);

          myFile.seek(EOF);
          myFile.write(lastBuffer, lastChunkSize);

          free(lastBuffer);
          countTotal += lastChunkSize;
        } else {
          // read full size chunk
#ifdef DEBUG
          LOG.printf("chunk #%d: %d bytes\n", currentChunk, RX_BUFFER_SIZE);
#endif
          Serial.readBytes(buffer, RX_BUFFER_SIZE);

          myFile.seek(EOF);
          myFile.write(buffer, RX_BUFFER_SIZE);

          countTotal += RX_BUFFER_SIZE;
        }
        currentChunk++;
      }
    }

    // uint16_t count = 0;
    // while (countTotal < fileSize) {

    //       if (Serial.available()) {
    //         char c = Serial.read();
    //         buffer[count] = c;
    // #ifdef DEBUG
    //         // LOG.printf("char read #%d : [%c]\n", count, buffer[count]);
    // #endif
    //         count++;

    //         if ( (count == RX_BUFFER_SIZE) || (count == fileSize - countTotal))  {
    //           countTotal += count;
    //           myFile.seek(EOF);
    //           myFile.write(buffer, count);

    // #ifdef DEBUG
    //           // LOG.printf("chunk #%d ok\n", nbChunk);
    // #endif
    //           nbChunk++;
    //           count = 0;
    //         }
    //       }
    // }

#ifdef DEBUG
    if (countTotal == fileSize) {
      LOG.printf("RECV OK nb bytes: [%d]\n", countTotal);
    } else {
      LOG.printf("RECV KO !!! nb bytes: [%d]\n", countTotal);
    }
#endif

    // close the file:
    myFile.close();

#ifdef DEBUG
    LOG.println("RECV done.");
#endif
  }
}

FLASHMEM void sd_deleteFile(const char *path) {
#ifdef DEBUG
  LOG.printf("DELETE file: [%s]\n", path);
#endif
  if (SD.exists(path)) {
    SD.remove(path);

#ifdef DEBUG
    LOG.println("DELETE done.");
#endif
  }
}

FLASHMEM void sd_renameFile(const char *pathToSplit) {
  char arr[2][50];
  char *ptr = strtok(pathToSplit, "|");
  strcpy(arr[0], ptr);
  ptr = strtok(NULL, "|");
  strcpy(arr[1], ptr);

#ifdef DEBUG
  LOG.printf("RENAME file: [%s]\n", arr[0]);
  LOG.printf("RENAME dest file: [%s]\n", arr[1]);
#endif

  if (!SD.exists(arr[0]))
    return;

  size_t n;  
  uint8_t buf[64];

  File myOrigFile = SD.open(arr[0], FILE_READ);
  File myDestFile = SD.open(arr[1], FILE_WRITE);

  while ((n = myOrigFile.read(buf, sizeof(buf))) > 0) {
    myDestFile.write(buf, n);
  }
  myOrigFile.close();
  myDestFile.close();

  sd_deleteFile(arr[0]);

#ifdef DEBUG
  LOG.println("RENAME done.");
#endif
}