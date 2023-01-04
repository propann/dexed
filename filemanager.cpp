#include "filemanager.h"

FLASHMEM void sd_filemanager() {
    char path[50];

    char cmd = Serial.read();
    size_t read = Serial.readBytesUntil('!', path, 255);
    path[read] = '\0';

    if (Serial.read() == '%') { // check cmd correct
        switch(cmd) {
            case 'd': // list SD content for path 
                sd_sendDirectory(path);
                break;

            case 's': // send file asked by remote
                sd_sendFile(path);
                break;

            // case 'r': // receive file sent from remote
            //     sd_receiveFile(path);
            //     break;

            default:
                break;
        }
    }

}

FLASHMEM void sd_sendDirectory(const char *path) {
    char filename[50];
    File dir = SD.open(path);

    Serial.write(99);
    Serial.write(FM_START);
    Serial.write(88);

    while (true) {
        File entry =  dir.openNextFile();
        // if (! entry || strcmp("System Volume Information", entry.name()) == 0) {
        if (! entry) {
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
        // Serial.write(strlen(entry.name()));
        // Serial.write(entry.name(), strlen(entry.name()));
        Serial.write(strlen(filename));
        Serial.write(filename, strlen(filename));
#ifdef DEBUG
        // LOG.printf("FM dir file [%s]\n", entry.name());
        LOG.printf("FM dir file [%s]\n", filename);
#endif
        Serial.write(88);
        entry.close();
    }
    dir.close();

    Serial.write(99);
    Serial.write(FM_END);
    Serial.write(88);
}

FLASHMEM void sd_sendFile(const char *filename) {
    Serial.write(99);
    Serial.write(FM_START);
    Serial.write(88);

    File myFile = SD.open(filename);
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

        //   while (count < length) {
        //     char buf[256];
        //     unsigned int n;
        //     n = f.read(buf, 256);
        //     Serial.write(buf, n);  // write complete buffer to port
        //     count = count + n;
        //     if (count % 128 == 0)
        //       display.console = false;
        //     display.fillRect(181, 52, count / (f.size() / 132), 8, RED);
        //   }
        //   f.close();

        while (myFile.available()) {
            Serial.write(99);
            Serial.write(FM_SEND_CHUNK);
            n = myFile.read(buf, 256);
            Serial.write(n/256);
            Serial.write(n%256);
            Serial.write(buf, n);
            Serial.write(88);
            Serial.flush();
        }

        // close the file:
        myFile.close();
    } else {
        // if the file didn't open, print an error:
#ifdef DEBUG
        LOG.printf("error opening [%s]\n", filename);
#endif
    }

    Serial.write(99);
    Serial.write(FM_END);
    Serial.write(88);
}

FLASHMEM void sd_receiveFile(const char *filename) {
    // receive file by Serial and write it to SD card
    File myFile = SD.open("test.txt", FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile) {
#ifdef DEBUG
        LOG.println("Writing to test.txt...");
#endif
        myFile.write("testing 1, 2, 3.");
        // close the file:
        myFile.close();
        LOG.println("done.");
    } else {
        // if the file didn't open, print an error:
        LOG.println("error opening test.txt");
    }
}