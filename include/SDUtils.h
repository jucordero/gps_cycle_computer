#include <SD.h>

#define TILE_W 120
#define TILE_H 120

struct TileCache {
  int lastX = -1;
  int lastY = -1;
  bool valid = false;
};

double lng2tilex(double lon);
double lat2tiley(double lat);
uint8_t reverseByte(uint8_t b);
boolean loadBMP(int x, int y, uint8_t *buffer, TileCache& cache);
int readVal(const char* path);
void listFiles(File dir, int numTabs);
void writeVal(const char* path, int value);
