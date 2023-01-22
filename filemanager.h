#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <SD.h>
#include "config.h"
#include <math.h>

enum { FM_START=10, FM_DIR=11, FM_SEND=12, FM_SEND_CHUNK=13, FM_RECEIVE=14, FM_DELETE=15, FM_END=20 };

void sd_filemanager();
void sd_sendDirectory(const char *path);
void sd_sendFile(const char *path);
void sd_receiveFile(const char *path);
void sd_deleteFile(const char *path);

#endif
