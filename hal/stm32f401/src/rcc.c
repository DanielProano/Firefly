/* Page 91 of https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xb-c-and-stm32f401xd-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf*/

#include <stdint.h>
#include <stm32f401xc.h>

void rcc_init(void) {
    /* Turn on HSI */
    RCC->CR |= RCC_CR_HSION_Msk;

    /* Wait for HSIRDY bit*/
    while(!(RCC->CR & RCC_CR_HSIRDY_Msk));

    /* Select HSI clock */
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC_Msk);

    /* Reset PLL CFGR*/
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk);
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN_Msk);
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP_Msk);

    /* Configure the PLL to be 84MHz */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_Msk;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_Msk;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_Msk;

    /* Turn on PLL */
    RCC->CR |= RCC_CR_PLLON_Msk;

    /* Wait for Phase Locked Loop */
    while (!(RCC->CR & RCC_CR_PLLRDY_Msk));
}

void rcc_enable_gpioa(void) {

}