#include <stdint.h>

#include "stm32f103xb.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
  // Bật HSE
  RCC->CR |= RCC_CR_HSEON;
  // Đợi HSE ổn định
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;

  // Chọn HSE làm nguồn cho PLL
  RCC->CFGR |= RCC_CFGR_PLLSRC;

  // Bật PLL
  RCC->CR |= RCC_CR_PLLON;
  // Đợi PLL ổn định
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;

  // Bộ nhân tần x9 (8 x 9 = 72MHz -> Max)
  RCC->CFGR |= RCC_CFGR_PLLMULL9;

  // Chọn PLL làm nguồn xung nhịp hệ thống
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    ;

  // Thiết lập AHB Prescaler
  RCC->CFGR |= RCC_CFGR_HPRE_DIV2; // (72 / 2 = 36 MHz)

  // Thiết lập APB1 Prescaler
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // (36 / 4 = 9 MHz)

  // Thiết lập APB2 Prescaler
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // (36 / 2 = 18 MHz)
}
