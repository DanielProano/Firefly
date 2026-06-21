/* Page 91 of https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xb-c-and-stm32f401xd-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf*/

#include <stdint.h>
#include<gpio.h>
#include <stm32f401xc.h>

void rcc_init(void) {
    /* Turn on HSI */
    RCC->CR |= RCC_CR_HSION;

    /* Wait for HSIRDY bit*/
    while(!(RCC->CR & RCC_CR_HSIRDY));

    /* Select HSI clock */
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);

    /* Reset PLL CFGR*/
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM
        | RCC_PLLCFGR_PLLN
        | RCC_PLLCFGR_PLLP
    );

    /* Configure the PLL to be 84MHz */
    RCC->PLLCFGR |= (16U << RCC_PLLCFGR_PLLM_Pos);
    RCC->PLLCFGR |= (336U << RCC_PLLCFGR_PLLN_Pos);
    RCC->PLLCFGR |= (1U << RCC_PLLCFGR_PLLP_Pos);

    /* Zero out AHB so system clock not divided*/
    RCC->CFGR &= ~(RCC_CFGR_HPRE);

    /* Reset APB1 */
    RCC->CFGR &= ~(RCC_CFGR_PPRE1);

    /*  APB1 cannot exceed 42 MHz, since
        we are running at 84, divide by 2 */
    RCC->CFGR |= (4U << RCC_CFGR_PPRE1_Pos);

    /*  Reset APB2 & keep zeroed bc. APB2 can
        handle 84MHz */
    RCC->CFGR &= ~(RCC_CFGR_PPRE2);

    /* Reset wait states*/
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_Msk);

    /* 2 wait states for flash at 3.3V & 84 MHz*/
    FLASH->ACR |= (2U << FLASH_ACR_LATENCY_Pos);

    /* Turn on PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait for Phase Locked Loop */
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* Reset CFGR*/
    RCC->CFGR &= ~(RCC_CFGR_SW_Msk);

    /* Select PLL as Sys clock*/
    RCC->CFGR |= (2U << RCC_CFGR_SW_Pos);

    /* Wait for System Clock Switch*/
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (2U << RCC_CFGR_SWS_Pos));
}

void rcc_enable_gpioc(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}