#include <adi_processor.h>
#include <drivers/pwr/adi_pwr.h>
#include <drivers/gpio/adi_gpio.h>

typedef struct {
    ADI_GPIO_PORT Port;
    ADI_GPIO_DATA Pins;
} PinMap;

PinMap MSB = {ADI_GPIO_PORT2, ADI_GPIO_PIN_10};  /*   Red LED on GPIO42 (DS4) */
PinMap LSB = {ADI_GPIO_PORT2, ADI_GPIO_PIN_2};   /* Green LED on GPIO34 (DS3) */

int main() {
    uint8_t         gpioMemory[ADI_GPIO_MEMORY_SIZE] = {0};
    uint32_t        count = 0;

    adi_pwr_Init();
    adi_pwr_SetClockDivider(ADI_CLOCK_HCLK, 1u);
    adi_pwr_SetClockDivider(ADI_CLOCK_PCLK, 1u);
   
    /* Initialize GPIO driver */
    adi_gpio_Init(gpioMemory, ADI_GPIO_MEMORY_SIZE);
    
    /* Enable MSB output */
    adi_gpio_OutputEnable(MSB.Port, MSB.Pins, true);
    
    /* Enable LSB output */
    adi_gpio_OutputEnable(LSB.Port, LSB.Pins, true);

    while (true)  {

        /* Delay between iterations */
        for (volatile uint32_t i = 0; i < 1000000; i++);

        /* Blink count (mod4) on the LEDs */

        switch (count%4) {
        case 3:
            adi_gpio_SetHigh(MSB.Port, MSB.Pins);
            adi_gpio_SetHigh(LSB.Port,  LSB.Pins);
            break;

        case 2:
            adi_gpio_SetHigh (MSB.Port, MSB.Pins);
            adi_gpio_SetLow(LSB.Port,  LSB.Pins);
            break;

        case 1:
            adi_gpio_SetLow (MSB.Port, MSB.Pins);
            adi_gpio_SetHigh(LSB.Port,  LSB.Pins);
            break;

        case 0:
            adi_gpio_SetLow (MSB.Port, MSB.Pins);
            adi_gpio_SetLow(LSB.Port,  LSB.Pins);
            break;

        }

        count++;
        
    }

}
