#include "scope.h"
#include "ILI9341_t3n.h"
extern ILI9341_t3n display;

FLASHMEM void Realtime_Scope::update(void)
{
  audio_block_t *block = receiveReadOnly(0);
  if (block) {
    if(updateBuffer) {
      updateBuffer = false;

      if (scope_is_drawing == false) {
        __disable_irq();
        memcpy(scopebuffer, block->data, AUDIO_BLOCK_SAMPLES * 2);
        __enable_irq();
      }
    }
    release(block);
  }
}

/*void Realtime_Scope::testdata() {
  DBG_LOG(printf("SCOPE TEST DATA:\n"));
  for(int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    
    DBG_LOG(printf("%i ", scopebuffer[i]));
  }
}*/

FLASHMEM void Realtime_Scope::draw_scope(uint16_t x, int y, uint8_t w, uint8_t h)
{
  scope_is_drawing = true;
  display.console = false;

  //display.drawRect(x, y, w, h, DX_DARKCYAN); // comment in to fit scope into screen
  const int16_t halfHeight = h/2;

  for(uint16_t i = 0; i < w; i++) {
    const int16_t signFactor = (scopebuffer[i] & 0x8000) ? -halfHeight : halfHeight;
    const uint16_t value = abs(scopebuffer[i]);
    
    scopebuffer[i] = signFactor * log10f(1.0 + (9 * value / 32768.0)) + halfHeight; // log plot
    //scopebuffer[i] = map(scopebuffer[i], 32767, -32768, h, 0); // linear plot
    //scopebuffer[i] = 512 * i - 32767; // test data
    //DBG_LOG(printf("s: %i\ti: %i\to: %i\n", signFactor, value, scopebuffer[i]));

    if (scopebuffer_old[i] != scopebuffer[i]) {
      display.drawPixel(x + i, scopebuffer_old[i] + y, COLOR_BACKGROUND);
      display.drawPixel(x + i, scopebuffer[i] + y, COLOR_SYSTEXT);
    }
    scopebuffer_old[i] = scopebuffer[i];
  }
  updateBuffer = true;
  scope_is_drawing = false;
}