
#include "system_tests.h"
#include "ILI9341_t3n.h"
extern ILI9341_t3n display;
extern void setCursor_textGrid_small(uint8_t pos_x, uint8_t pos_y);
extern uint16_t COLOR_BACKGROUND;
extern uint16_t COLOR_SYSTEXT;
uint8_t psram_test_dline = 7;

void fill_up_with_spaces_psram()
{
  do {
    display.print(" ");
  } while (display.getCursorX() < 52 * CHAR_width_small);
}

FLASHMEM void psram_test()
{
  char text1[40];
  psram_test_dline = 7;
  uint8_t size = external_psram_size;
  setCursor_textGrid_small(2, 5);
  display.setTextColor(GREEN, COLOR_BACKGROUND);
  sprintf(text1, "FOUND %d MB CHIP", size);
  display.print(text1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if (size == 0) return;
  const float clocks[4] = {396.0f, 720.0f, 664.62f, 528.0f};
  const float frequency = clocks[(CCM_CBCMR >> 8) & 3] / (float)(((CCM_CBCMR >> 29) & 7) + 1);
  sprintf(text1, " (%03d MHz)", (int)frequency);
  display.print(text1);
  fill_up_with_spaces_psram();
  memory_begin = (uint32_t *)(0x70000000);
  memory_end = (uint32_t *)(0x70000000 + size * 1048576);
  elapsedMillis msec = 0;
  if (!psram_check_fixed_pattern(0x5A698421)) return;
  if (!psram_check_lfsr_pattern(2976674124ul)) return;
  if (!psram_check_lfsr_pattern(1438200953ul)) return;
  if (!psram_check_lfsr_pattern(3413783263ul)) return;
  if (!psram_check_lfsr_pattern(1900517911ul)) return;
  if (!psram_check_lfsr_pattern(1227909400ul)) return;
  if (!psram_check_lfsr_pattern(276562754ul)) return;
  if (!psram_check_lfsr_pattern(146878114ul)) return;
  if (!psram_check_lfsr_pattern(615545407ul)) return;
  if (!psram_check_lfsr_pattern(110497896ul)) return;
  if (!psram_check_lfsr_pattern(74539250ul)) return;
  if (!psram_check_lfsr_pattern(4197336575ul)) return;
  if (!psram_check_lfsr_pattern(2280382233ul)) return;
  if (!psram_check_lfsr_pattern(542894183ul)) return;
  if (!psram_check_lfsr_pattern(3978544245ul)) return;
  if (!psram_check_lfsr_pattern(2315909796ul)) return;
  if (!psram_check_lfsr_pattern(3736286001ul)) return;
  if (!psram_check_lfsr_pattern(2876690683ul)) return;
  if (!psram_check_lfsr_pattern(215559886ul)) return;
  if (!psram_check_lfsr_pattern(539179291ul)) return;
  if (!psram_check_lfsr_pattern(537678650ul)) return;
  if (!psram_check_lfsr_pattern(4001405270ul)) return;
  if (!psram_check_lfsr_pattern(2169216599ul)) return;
  if (!psram_check_lfsr_pattern(4036891097ul)) return;
  if (!psram_check_lfsr_pattern(1535452389ul)) return;
  if (!psram_check_lfsr_pattern(2959727213ul)) return;
  if (!psram_check_lfsr_pattern(4219363395ul)) return;
  if (!psram_check_lfsr_pattern(1036929753ul)) return;
  if (!psram_check_lfsr_pattern(2125248865ul)) return;
  if (!psram_check_lfsr_pattern(3177905864ul)) return;
  if (!psram_check_lfsr_pattern(2399307098ul)) return;
  if (!psram_check_lfsr_pattern(3847634607ul)) return;
  if (!psram_check_lfsr_pattern(27467969ul)) return;
  if (!psram_check_lfsr_pattern(520563506ul)) return;
  if (!psram_check_lfsr_pattern(381313790ul)) return;
  if (!psram_check_lfsr_pattern(4174769276ul)) return;
  if (!psram_check_lfsr_pattern(3932189449ul)) return;
  if (!psram_check_lfsr_pattern(4079717394ul)) return;
  if (!psram_check_lfsr_pattern(868357076ul)) return;
  if (!psram_check_lfsr_pattern(2474062993ul)) return;
  if (!psram_check_lfsr_pattern(1502682190ul)) return;
  if (!psram_check_lfsr_pattern(2471230478ul)) return;
  if (!psram_check_lfsr_pattern(85016565ul)) return;
  if (!psram_check_lfsr_pattern(1427530695ul)) return;
  if (!psram_check_lfsr_pattern(1100533073ul)) return;
  if (!psram_check_fixed_pattern(0x55555555)) return;
  if (!psram_check_fixed_pattern(0x33333333)) return;
  if (!psram_check_fixed_pattern(0x0F0F0F0F)) return;
  if (!psram_check_fixed_pattern(0x00FF00FF)) return;
  if (!psram_check_fixed_pattern(0x0000FFFF)) return;
  if (!psram_check_fixed_pattern(0xAAAAAAAA)) return;
  if (!psram_check_fixed_pattern(0xCCCCCCCC)) return;
  if (!psram_check_fixed_pattern(0xF0F0F0F0)) return;
  if (!psram_check_fixed_pattern(0xFF00FF00)) return;
  if (!psram_check_fixed_pattern(0xFFFF0000)) return;
  if (!psram_check_fixed_pattern(0xFFFFFFFF)) return;
  if (!psram_check_fixed_pattern(0x00000000)) return;
  display.setTextColor(GREEN, COLOR_BACKGROUND);
  psram_test_dline++;
  setCursor_textGrid_small(2, psram_test_dline);
  display.print(" ");
  fill_up_with_spaces_psram();
  psram_test_dline++;
  setCursor_textGrid_small(2, psram_test_dline);
  sprintf(text1, "Test ran for %lu seconds", msec / 1000);
  display.print(text1);
  fill_up_with_spaces_psram();
  psram_test_dline++;
  setCursor_textGrid_small(2, psram_test_dline);
  display.print("All memory tests passed :-)");
  fill_up_with_spaces_psram();
  psram_memory_ok = true;
  psram_test_dline++;
  display.fillRect(1, psram_test_dline * 10, DISPLAY_WIDTH - 1, (DISPLAY_HEIGHT - (psram_test_dline * 10) - CHAR_height_small), COLOR_BACKGROUND );
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

FLASHMEM bool psram_fail_message(volatile uint32_t *location, uint32_t actual, uint32_t expected)
{
  char text1[40];
  psram_test_dline++;
  setCursor_textGrid_small(2, psram_test_dline);
  display.setTextColor(RED, COLOR_BACKGROUND);
  sprintf(text1, "Error at %lu, read %lu but expected %lu", (uint32_t)location, actual, expected);
  display.print(text1);
  fill_up_with_spaces_psram();
  psram_test_dline++;
  display.fillRect(1, psram_test_dline * 10, DISPLAY_WIDTH - 1, (DISPLAY_HEIGHT - (psram_test_dline * 10) - CHAR_height_small), COLOR_BACKGROUND);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  return false;
}

// fill the entire RAM with a fixed pattern, then check it
FLASHMEM bool psram_check_fixed_pattern(uint32_t pattern)
{
  char text1[40];
  setCursor_textGrid_small(2, psram_test_dline);
  volatile uint32_t *p;
  sprintf(text1, "testing with fixed pattern %lu", pattern);
  display.print(text1);
  fill_up_with_spaces_psram();
  for (p = memory_begin; p < memory_end; p++) {
    *p = pattern;
  }
  arm_dcache_flush_delete((void *)memory_begin, (uint32_t)memory_end - (uint32_t)memory_begin);
  for (p = memory_begin; p < memory_end; p++) {
    uint32_t actual = *p;
    if (actual != pattern) return psram_fail_message(p, actual, pattern);
  }
  psram_test_dline++;
  if (psram_test_dline > 20)
    psram_test_dline = 7;
  return true;
}

// fill the entire RAM with a pseudo-random sequence, then check it
FLASHMEM bool psram_check_lfsr_pattern(uint32_t seed)
{
  char text1[40];
  setCursor_textGrid_small(2, psram_test_dline);
  volatile uint32_t *p;
  uint32_t reg;
  sprintf(text1, "Testing pseudo-random sequence, seed=%lu", seed);
  display.print(text1);
  fill_up_with_spaces_psram();
  reg = seed;
  for (p = memory_begin; p < memory_end; p++) {
    *p = reg;
    for (int i = 0; i < 3; i++) {
      if (reg & 1) {
        reg >>= 1;
        reg ^= 0x7A5BC2E3;
      } else {
        reg >>= 1;
      }
    }
  }
  arm_dcache_flush_delete((void *)memory_begin, (uint32_t)memory_end - (uint32_t)memory_begin);
  reg = seed;
  for (p = memory_begin; p < memory_end; p++) {
    uint32_t actual = *p;
    if (actual != reg) return psram_fail_message(p, actual, reg);
    //Serial.printf(" reg=%08X\n", reg);
    for (int i = 0; i < 3; i++) {
      if (reg & 1) {
        reg >>= 1;
        reg ^= 0x7A5BC2E3;
      } else {
        reg >>= 1;
      }
    }
  }
  psram_test_dline++;
  if (psram_test_dline > 20)
    psram_test_dline = 7;
  return true;
}
