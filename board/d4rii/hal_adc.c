#include "hal_adc.h"
#include "debug.h"
#include "wdt.h"
#include "pin_config.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"

// adc results
volatile uint16_t hal_adc_data[2];

void hal_adc_init(void) {
    hal_adc_init_rcc();
    hal_adc_init_gpio();
    hal_adc_init_mode();
    hal_adc_init_dma();
}

static void hal_adc_init_rcc(void) {
    //ADC CLOCK = 24 / 4 = 6MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    // enable ADC clock
    RCC_APBxPeriphClockCmd(ADC_CLK_RCC, ADC_CLK, ENABLE);

    // enable dma clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //periph clock enable for port
    RCC_APBxPeriphClockCmd(ADC_GPIO_CLK_RCC, ADC_GPIO_CLK, ENABLE);
}

static void hal_adc_init_gpio(void) {
    GPIO_InitTypeDef gpio_init;

    // set up analog inputs
    gpio_init.GPIO_Pin  = ADC_IN1_PIN | ADC_IN2_PIN;
    gpio_init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(ADC_GPIO, &gpio_init);
}

static void hal_adc_init_mode(void) {
    ADC_InitTypeDef adc_init;

    //ADC configuration
    adc_init.ADC_Mode                 = ADC_Mode_Independent;
    adc_init.ADC_ScanConvMode         = ENABLE;//We will convert multiple channels
    adc_init.ADC_ContinuousConvMode   = ENABLE; //! select continuous conversion mode
    adc_init.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_None; //select no external triggering
    adc_init.ADC_DataAlign            = ADC_DataAlign_Right; //right 12-bit data alignment in ADC data register
    adc_init.ADC_NbrOfChannel         = 2; //2 channels conversion

    //load structure values to control and status registers
    ADC_Init(ADC, &adc_init);

    //configure each channel
    ADC_RegularChannelConfig(ADC, ADC_Channel_1, 1, ADC_SampleTime_41Cycles5);
    ADC_RegularChannelConfig(ADC, ADC_Channel_2, 2, ADC_SampleTime_41Cycles5);

    //Enable ADC
    ADC_Cmd(ADC, ENABLE);

    //enable DMA for ADC
    ADC_DMACmd(ADC, ENABLE);

    //Enable ADC1 reset calibration register
    ADC_ResetCalibration(ADC);

    //Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC)){}

    //Start ADC calibration
    ADC_StartCalibration(ADC);

    //Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC)){}
}

static void hal_adc_init_dma(void) {
    DMA_InitTypeDef  dma_init;

    //reset DMA1 channe1 to default values
    DMA_DeInit(ADC_DMA_CHANNEL);

    // set up dma to convert 2 adc channels to two mem locations:
    dma_init.DMA_M2M                 = DMA_M2M_Disable; //channel will be used for memory to memory transfer
    dma_init.DMA_Mode                = DMA_Mode_Circular; //setting normal mode (non circular)
    dma_init.DMA_Priority            = DMA_Priority_High; //medium priority
    dma_init.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord; //source and destination 16bit
    dma_init.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;
    dma_init.DMA_MemoryInc           = DMA_MemoryInc_Enable; //automatic memory destination increment enable.
    dma_init.DMA_PeripheralInc       = DMA_PeripheralInc_Disable; //source address increment disable
    dma_init.DMA_DIR                 = DMA_DIR_PeripheralSRC; //Location assigned to peripheral register will be source
    dma_init.DMA_BufferSize          = 2; //chunk of data to be transfered
    dma_init.DMA_PeripheralBaseAddr  = (uint32_t)&ADC->DR; //source and destination start addresses
    dma_init.DMA_MemoryBaseAddr      = (uint32_t)hal_adc_data;
    //send values to DMA registers
    DMA_Init(ADC_DMA_CHANNEL, &dma_init);

    //Enable the DMA1 - Channel1
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);

    //start conversion:
    hal_adc_dma_arm();


    //TEST ADC
    /*while(1){
        debug_putc('A');
        wdt_reset();
        if (ADC_GetFlagStatus(ADC, ADC_FLAG_EOC) == SET){
            uint16_t res = ADC_GetConversionValue(ADC);
            debug("ADC = "); debug_put_uint16(res); debug_put_newline(); debug_flush();

            ADC_ClearFlag(ADC, ADC_FLAG_EOC);
            //start next ADC Software Conversion
            ADC_SoftwareStartConvCmd(ADC, ENABLE);
        }
    }*/
}

static void hal_adc_dma_arm(void) {
    ADC_SoftwareStartConvCmd(ADC, ENABLE);
}

void hal_adc_process(void) {
    //adc dma finished?
    if(DMA_GetITStatus(ADC_DMA_TC_FLAG)){
        //fine, arm DMA again:
        hal_adc_dma_arm();
    }else{
        //oops this should not happen
        debug_putc('D');
        //cancel and re arm dma ???
    }
}

uint8_t hal_adc_get_scaled(uint8_t ch) {
    if (ch < 2){
        //12 bit adc -> scale to 8 bit -> shift by 4
        return hal_adc_data[ch]>>4;
    }else{
        debug("hal_adc: channel index out of bounds ");
        debug_put_uint8(ch);
        debug ("allowed 0,1)\n");
        debug_flush();
        return 0;
    }
}

