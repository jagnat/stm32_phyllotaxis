/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_LEDS 89
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

const uint8_t small_spirals_lens[] = {5, 4, 4, 4, 4, 5, 4, 4, 5, 4, 4, 4, 4, 5, 4, 4, 5, 4, 4, 4, 4};
const uint8_t small_spirals[][5] = {
  {88, 87, 86, 85, 84,},
  {80, 81, 82, 83, 0xff,},
  {79, 78, 77, 76, 0xff,},
  {72, 73, 74, 75, 0xff,},
  {71, 70, 69, 68, 0xff,},
  {63, 64, 65, 66, 67,},
  {62, 61, 60, 59, 0xff,},
  {55, 56, 57, 58, 0xff,},
  {54, 53, 52, 51, 50,},
  {46, 47, 48, 49, 0xff,},
  {45, 44, 43, 42, 0xff,},
  {38, 39, 40, 41, 0xff,},
  {37, 36, 35, 34, 0xff,},
  {29, 30, 31, 32, 33,},
  {28, 27, 26, 25, 0xff,},
  {21, 22, 23, 24, 0xff,},
  {20, 19, 18, 17, 16,},
  {12, 13, 14, 15, 0xff,},
  {11, 10,  9,  8, 0xff,},
  {4,  5,   6,  7, 0xff,},
  {3,  2,   1,  0, 0xff,},
};

const uint8_t large_spirals_lens[] = {7, 7, 7, 7, 6, 7, 7, 7, 7, 6, 7, 7, 7,};
const uint8_t large_spirals[][7] = {
  {72, 79, 81, 86,  1,  7,  8,},
  {80, 87,  2,  6,  9, 15, 16,},
  {88,  3,  5, 10, 14, 17, 24,},
  { 4, 11, 13, 18, 23, 25, 33,},
  {12, 19, 22, 26, 32, 34, 0xff,},
  {20, 21, 27, 31, 35, 41, 42,},
  {28, 30, 36, 40, 43, 49, 50,},
  {29, 37, 39, 44, 48, 51, 58,},
  {38, 45, 47, 52, 57, 59, 67,},
  {46, 53, 56, 60, 66, 68, 0xff,},
  {54, 55, 61, 65, 69, 75, 76,},
  {62, 64, 70, 74, 77, 83, 84,},
  {63, 71, 73, 78, 82, 85,  0,},
};

const float led_positions[NUM_LEDS][2] = {
  {-0.295600f, -0.881845f},
  {-0.147642f, -0.769175f},
  {-0.035599f, -0.635367f},
  {0.038057f, -0.488029f},
  {0.179926f, -0.394470f},
  {0.168341f, -0.555471f},
  {0.116204f, -0.717929f},
  {0.023421f, -0.873812f},
  {0.232423f, -0.936628f},
  {0.301129f, -0.760752f},
  {0.328102f, -0.585689f},
  {0.315025f, -0.419325f},
  {0.392868f, -0.255296f},
  {0.466666f, -0.401150f},
  {0.506617f, -0.569514f},
  {0.508307f, -0.753704f},
  {0.724054f, -0.689743f},
  {0.690701f, -0.500792f},
  {0.621962f, -0.334684f},
  {0.523603f, -0.197011f},
  {0.402082f, -0.092510f},
  {0.503473f, -0.004886f},
  {0.643665f, -0.093004f},
  {0.766763f, -0.218189f},
  {0.865428f, -0.377196f},
  {0.886471f, -0.053956f},
  {0.739284f, 0.054065f},
  {0.581749f, 0.122011f},
  {0.421872f, 0.149424f},
  {0.301882f, 0.249448f},
  {0.457132f, 0.284555f},
  {0.624881f, 0.281390f},
  {0.797553f, 0.237514f},
  {0.967186f, 0.151767f},
  {0.807515f, 0.447203f},
  {0.622814f, 0.463278f},
  {0.450418f, 0.439581f},
  {0.297278f, 0.380064f},
  {0.131620f, 0.405760f},
  {0.248079f, 0.516986f},
  {0.395414f, 0.602039f},
  {0.568461f, 0.654807f},
  {0.458040f, 0.841453f},
  {0.289416f, 0.757803f},
  {0.152117f, 0.646686f},
  {0.050236f, 0.515038f},
  {-0.115473f, 0.446860f},
  {-0.072279f, 0.604083f},
  {0.011016f, 0.755164f},
  {0.133363f, 0.892186f},
  {-0.067530f, 0.990708f},
  {-0.169640f, 0.828974f},
  {-0.230295f, 0.660292f},
  {-0.249830f, 0.492731f},
  {-0.230015f, 0.334066f},
  {-0.362822f, 0.338933f},
  {-0.407380f, 0.497945f},
  {-0.414354f, 0.672862f},
  {-0.380634f, 0.856274f},
  {-0.612366f, 0.633545f},
  {-0.576115f, 0.455234f},
  {-0.505354f, 0.299452f},
  {-0.405576f, 0.172048f},
  {-0.384114f, 0.019641f},
  {-0.522417f, 0.097668f},
  {-0.644195f, 0.212445f},
  {-0.742204f, 0.360620f},
  {-0.809812f, 0.537627f},
  {-0.890698f, 0.214167f},
  {-0.765219f, 0.078460f},
  {-0.622068f, -0.019637f},
  {-0.469039f, -0.078271f},
  {-0.365062f, -0.206826f},
  {-0.525650f, -0.211043f},
  {-0.691454f, -0.175127f},
  {-0.854531f, -0.098069f},
  {-0.899516f, -0.308812f},
  {-0.718989f, -0.360257f},
  {-0.543208f, -0.370169f},
  {-0.379756f, -0.341152f},
  {-0.213409f, -0.401333f},
  {-0.350636f, -0.488605f},
  {-0.513417f, -0.544320f},
  {-0.695552f, -0.563401f},
  {-0.615037f, -0.770683f},
  {-0.431352f, -0.719810f},
  {-0.273469f, -0.636007f},
  {-0.146724f, -0.525350f},
  {-0.055075f, -0.394778f},
};

const uint8_t gamma8[] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
 37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
 69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
 90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
 115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
 144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
 177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
 215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// Translate from 4 bit nibbles from color channels
// into SPI neopixel bits
// 0 -> 100
// 1 -> 110
// Starting from MSB
const uint16_t SPI_NIBBLE_LUT[] = {
  04444, // 0
  04446, // 1
  04464, // 2
  04466, // 3
  04644, // 4
  04646, // 5
  04664, // 6
  04666, // 7
  06444, // 8
  06446, // 9
  06464, // A
  06466, // B
  06644, // C
  06646, // D
  06664, // E
  06666, // F
};

uint32_t urgb_buffer[NUM_LEDS];
uint8_t spi_buffer[9 * NUM_LEDS + 1];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

static void write_urgb(uint32_t rgb, uint8_t *data_ptr);
static uint32_t make_urgb(uint8_t r, uint8_t g, uint8_t b);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void write_urgb(uint32_t rgb, uint8_t *data_ptr) {
  uint16_t g_hi = SPI_NIBBLE_LUT[(rgb >> 20) & 0xf];
  uint16_t g_lo = SPI_NIBBLE_LUT[(rgb >> 16) & 0xf];

  uint16_t r_hi = SPI_NIBBLE_LUT[(rgb >> 12) & 0xf];
  uint16_t r_lo = SPI_NIBBLE_LUT[(rgb >> 8) & 0xf];

  uint16_t b_hi = SPI_NIBBLE_LUT[(rgb >> 4) & 0xf];
  uint16_t b_lo = SPI_NIBBLE_LUT[(rgb >> 0) & 0xf];

  data_ptr[0] = g_hi >> 4;
  data_ptr[1] = (g_hi << 4) | (g_lo >> 8);
  data_ptr[2] = g_lo;

  data_ptr[3] = r_hi >> 4; 
  data_ptr[4] = (r_hi << 4) | (r_lo >> 8);
  data_ptr[5] = r_lo; 

  data_ptr[6] = b_hi >> 4;
  data_ptr[7] = (b_hi << 4) | (b_lo >> 8);
  data_ptr[8] = b_lo;
}

uint32_t make_urgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t) (r) << 8) | ((uint32_t) (g) << 16) | (uint32_t) (b);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_2, LL_SPI_DMA_GetRegAddr(SPI1));
  LL_SPI_EnableDMAReq_TX(SPI1);
  LL_SPI_Enable(SPI1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    static int count = 0;

    for (int i = 0; i < 89; i++) {
      urgb_buffer[i] = 0;
    }

    static uint8_t level = 0;
    static bool ascending = true;

    level = ascending? (level + 1) : (level - 1);
    if (level == 0) {
      ascending = true;
    }
    if (level == 255) {
      ascending = false;
    }

    // static int ringLevel = 0;
    // int numSmallSpirals = sizeof(small_spirals_lens) / sizeof(small_spirals_lens[0]);

    // for (int i = 0; i < small_spirals_lens[ringLevel]; i++) {
    //   urgb_buffer[small_spirals[ringLevel][i]] = make_urgb(gamma8[level], 0,gamma8[level / 3]);
    // }

    // ringLevel += 1;
    // if (ringLevel >= numSmallSpirals) {
    //   ringLevel = 0;
    // }

    static int ringLevel = 0;
    int numLargeSpirals = sizeof(large_spirals_lens) / sizeof(large_spirals_lens[0]);

    for (int i = 0; i < large_spirals_lens[ringLevel]; i++) {
      urgb_buffer[large_spirals[ringLevel][i]] = make_urgb(gamma8[level], 0,gamma8[level / 3]);
    }
    int level2 = (ringLevel + numLargeSpirals / 2) % numLargeSpirals;
    for (int i = 0; i < large_spirals_lens[ringLevel]; i++) {
      urgb_buffer[large_spirals[level2][i]] = make_urgb(gamma8[level], 0,gamma8[level / 3]);
    }

    ringLevel += 1;
    if (ringLevel >= numLargeSpirals) {
      ringLevel = 0;
    }

    // for (int i = 0; i < 89; i++) {
    //   uint32_t col = i == count? make_urgb(1, 0, 0) : make_urgb(0x0, 0x0, 0x0);
    //   if (i == (count + 30) % NUM_LEDS) {
    //     col = make_urgb(255, 0, 0);
    //   }
    //   if (i == (count + 60) % NUM_LEDS) {
    //     col = make_urgb(5, 0, 0);
    //   }
    //   urgb_buffer[i] = col;
    // }

    uint8_t *walk = spi_buffer;
    for (int i = 0; i < 89; i++) {
      write_urgb(urgb_buffer[i], walk);
      walk += 9;
    }

    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
    while (LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_2));  // Wait for it to stop

    LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_2, (uint32_t)spi_buffer, LL_SPI_DMA_GetRegAddr(SPI1),
      LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_2));
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, 9 * NUM_LEDS + 1);

    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);

    while (!LL_DMA_IsActiveFlag_TC2(DMA2));
    while (!LL_SPI_IsActiveFlag_TXE(SPI1));
    while (LL_SPI_IsActiveFlag_BSY(SPI1));

    LL_DMA_ClearFlag_TC2(DMA2);

    LL_mDelay(85);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(96000000);
  LL_SetSystemCoreClock(96000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* SPI1 DMA Init */

  /* SPI1_TX Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_2, LL_DMA_CHANNEL_2);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_2, LL_DMA_PRIORITY_MEDIUM);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_2, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_2);

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV32;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  NVIC_SetPriority(DMA2_Stream2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);

  /**/
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
