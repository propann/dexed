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
        memcpy(scopebuffer, block->data, AUDIO_BLOCK_SAMPLES);
        __enable_irq();
      }
    }
    release(block);
  }
}

FLASHMEM void Realtime_Scope::draw_scope(uint16_t x, int y, uint8_t w, uint8_t h)
{
  scope_is_drawing = true;
  display.console = false;
  for(uint16_t i = 0; i < w; i++) {
    scopebuffer[i] = map(scopebuffer[i], 32767, -32768, h, 0);

    if (scopebuffer_old[i] != scopebuffer[i]) {
      display.drawPixel(x + i, scopebuffer_old[i] + y, COLOR_BACKGROUND);
      display.drawPixel(x + i, scopebuffer[i] + y, COLOR_SYSTEXT);
    }
    scopebuffer_old[i] = scopebuffer[i];
  }
  updateBuffer = true;
  scope_is_drawing = false;
}