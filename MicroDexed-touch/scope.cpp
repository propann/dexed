#include "scope.h"
#include "ILI9341_t3n.h"
extern ILI9341_t3n display;

FLASHMEM void Realtime_Scope::update(void)
{
  audio_block_t *block = receiveReadOnly(0);
  if (block) {
    if (scope_is_drawing == false) {
      __disable_irq();
      for(uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        scopebuffer[i] = map(block->data[i], 32767, -32768, 30, -30) + 32;
      }
      __enable_irq();
    }
    release(block);
  }
}

FLASHMEM void Realtime_Scope::draw_scope(uint16_t x, int y, uint8_t w)
{
  scope_is_drawing = true;
  display.console = false;
  for(uint16_t i = 0; i < w; i++) {
    if (scopebuffer_old[i] != scopebuffer[i]) {
      display.drawPixel(x + i, scopebuffer_old[i] + y, COLOR_BACKGROUND);
      display.drawPixel(x + i, scopebuffer[i] + y, COLOR_SYSTEXT);
    }
    scopebuffer_old[i] = scopebuffer[i];
  }
  scope_is_drawing = false;
}