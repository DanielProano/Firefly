/* Page 91 of https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xb-c-and-stm32f401xd-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf*/

#include <stdint.h>
#include "rcc.h"

#define RCC                         ((rcc_struct*) RCC_BASE)
#define RCC_BASE                    0x40023800
#define RCC_CR_HSION_MASK           (1U << 0)
#define RCC_CR_HSIRDY_MASK          (1U << 1)
#define RCC_CR_PLLON_MASK           (1U << 24)
#define RCC_CR_PLLRDY_MASK          (1U << 25)

/* PLL CFGR */
#define RCC_PLLCFGR_PLLM_MASK       (16U)
#define RCC_PLLCFGR_PLLP_MASK       (1U << 16)
#define RCC_PLLCFGR_PLLN_MASK       (336U << 6)

void rcc_init(void) {
    /* Reset CR */
    RCC->CR &= 0x00000000;

    /* Turn on HSI */
    RCC->CR |= RCC_CR_HSION_MASK;

    /* Wait for HSIRDY bit*/
    while(!(RCC->CR & RCC_CR_HSIRDY_MASK));

    /* Configure the PLL to be 84MHz */
    RCC->PLLCFGR &= 0x00000000;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_MASK;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_MASK;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_MASK;

    /* Turn on PLL */
    RCC->CR |= RCC_CR_PLLON_MASK;

    /* Wait for Phase Locked Loop */
    while (!(RCC->CR & RCC_CR_PLLRDY_MASK));
}

void rcc_enable_gpioa(void) {

}