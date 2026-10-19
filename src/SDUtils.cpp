#include "Arduino.h"
#include "SDUtils.h"
#include "config.h"

double lng2tilex(double lon) {
  return ( (lon + 180.0) / 360.0 * (1 << 15) );
}

double lat2tiley(double lat) {
  double lat_rad = lat * M_PI / 180.0;
  return ( (1.0 - log( tan(lat_rad) + 1.0 / cos(lat_rad) ) / M_PI) / 2.0 * (1 << 15) );
}

uint8_t reverseByte(uint8_t b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

boolean loadBMP(int x, int y, uint8_t *buffer, TileCache& cache) {

  // If unchanged, do nothing
  if (cache.valid && x == cache.lastX && y == cache.lastY) {
    return true;
  }
  
  String filename = "/tiles/15/" + String(y) + "/" + String(x) + ".bmp";
  Serial.print(F("Loading "));
  Serial.println(filename);

  File f = SD.open(filename.c_str());
  if (!f) return false;

  // --- Read BMP header ---
  uint16_t bfType;
  f.read((uint8_t*)&bfType, 2);
  if (bfType != 0x4D42) { // "BM"
    f.close();
    return false;
  }

  f.seek(10);
  uint32_t pixelOffset;
  f.read((uint8_t*)&pixelOffset, 4);

  f.seek(18);
  int32_t width, height;
  f.read((uint8_t*)&width, 4);
  f.read((uint8_t*)&height, 4);

  f.seek(28);
  uint16_t bpp;
  f.read((uint8_t*)&bpp, 2);

  if (width != TILE_W || height != TILE_H || bpp != 1) {
    f.close();
    return false;
  }

  // Skip palette (2 colors × 4 bytes)
  f.seek(pixelOffset);

  // BMP rows are padded to 4-byte boundaries
  int rowBytes = ((TILE_W + 31) / 32) * 4;

  uint8_t rowBuf[rowBytes];

  // BMP is bottom-up, so we invert row order
  for (int y = 0; y < TILE_H; y++) {
    f.read(rowBuf, rowBytes);

    // FIX: reverse bit order in each byte
    for (uint16_t i = 0; i < rowBytes; i++) {
      rowBuf[i] = reverseByte(rowBuf[i]);
    }

    int dstRow = TILE_H - 1 - y;
    memcpy(&buffer[dstRow * (TILE_W / 8)], rowBuf, TILE_W / 8);
  }

  f.close();

  // Update cache
  cache.lastX = x;
  cache.lastY = y;
  cache.valid = true;

  return true;
}

// Function that returns a value from inside an SD card file
int readVal(const char* path){

  String readArray;

  File f = SD.open(path);  
  if (!f) {
    Serial.print(F("Could not read data from "));
    Serial.println(path);
    return INVALID_OPTION_VAL;
  }

  else {
    while (f.available()){
      char c = f.read();
      if (isPrintable(c)) readArray.concat(c);
      else if (c == '\n') break;
    }
    f.close();

    Serial.print(F("Read value from "));
    Serial.print(path);
    Serial.print(F(" : "));
    Serial.println(readArray.toInt());

    return readArray.toInt();
  }

}

void writeVal(const char* path, int value) {

  // Remove file if it already exists
  if (SD.exists(path)) {
    SD.remove(path);
  }

  // Create new file
  File f = SD.open(path, FILE_WRITE);
  if (!f) {
    Serial.print(F("Could not write value to "));
    Serial.println(path);
    return;
  }

  f.println(value);   // file now contains exactly one line
  f.close();
}

void listFiles(File dir, int numTabs) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) break;
        
        for (int i = 0; i < numTabs; i++) Serial.print("\t");
        Serial.print(entry.name());
        
        if (entry.isDirectory()) {
            Serial.println(F("/"));
            // listFiles(entry, numTabs + 1);
        } else {
            Serial.print(F(" - "));
            Serial.print(entry.size());
            Serial.println(F(" bytes"));
        }
        entry.close();
    }
}

