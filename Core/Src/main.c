/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <sys/time.h>

#include "max7219.h"
#include "led_functional.h"
#include "encoder_functional.h"
#include "buttons_functional.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INDICATORS_MODE 2
#define INDICATORS_MODULE 2
#define INDICATORS_POSITION 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t interrupt_status = 0;
volatile uint8_t rx_byte;

uint8_t indicator_data[INDICATORS_MODE][INDICATORS_MODULE][INDICATORS_POSITION] = {0}; // Mask position - [0][0] - 6, 2, 3, 4 ; [0][1] - 7, 5, 1, 0 ;

uint16_t ampere_data = 1661;
uint16_t voltage_data = 2323;

FlagsStatus periphery_status = {0};

static uint32_t last_press_btn1 = 0;
static uint32_t last_signal_edit_mode = 0;
static uint8_t blink_set_state = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// const uint8_t segmentMap[] = {
//   0b00111111, // 0
//   0b00000110, // 1
//   0b01011011, // 2
//   0b01001111, // 3
//   0b01100110, // 4
//   0b01101101, // 5
//   0b01111101, // 6
//   0b00000111, // 7
//   0b01111111, // 8
//   0b01101111  // 9
// };
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  //hello test !!!!!!
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  if (hspi3.State != HAL_SPI_STATE_READY) {
    // Если SPI не готов, зажигаем светодиод ошибки
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
    while(1);  // Останавливаемся здесь
  }

  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
  HAL_Delay(100);
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
  HAL_Delay(100);

  // Инициализация MAX7219
  MAX7219_Init(CS_GPIO_Port_A, CS_Pin_A15);
  MAX7219_Init(CS_GPIO_Port_C, CS_Pin_C11);

  //test
  MAX7219_Write(MAX7219_REG_DISPLAY_TEST, 0x01, CS_GPIO_Port_A, CS_Pin_A15);
  MAX7219_Write(MAX7219_REG_DISPLAY_TEST, 0x01, CS_GPIO_Port_C, CS_Pin_C11);
  HAL_Delay(2000);
  MAX7219_Write(MAX7219_REG_DISPLAY_TEST, 0x00, CS_GPIO_Port_A, CS_Pin_A15);
  MAX7219_Write(MAX7219_REG_DISPLAY_TEST, 0x00, CS_GPIO_Port_C, CS_Pin_C11);
  HAL_Delay(500);


  epos_startup();
  HAL_Delay(2000);
  MAX7219_Clear_not_decode(CS_GPIO_Port_A, CS_Pin_A15);
  MAX7219_Clear_decode(CS_GPIO_Port_C, CS_Pin_C11);
  MAX7219_Write(MAX7219_REG_DECODE_MODE, 0xFF, CS_GPIO_Port_A, CS_Pin_A15);

  HAL_Delay(1000);

  for (uint8_t i = 0; i < 8; i++) {
    MAX7219_SendDigit(i,0,CS_GPIO_Port_A,CS_Pin_A15);
    MAX7219_SendDigit(i,0,CS_GPIO_Port_C,CS_Pin_C11);
  }
  MAX7219_SendDigit(2, 0x80, CS_GPIO_Port_A, CS_Pin_A15);
  MAX7219_SendDigit(5, 0x80, CS_GPIO_Port_A, CS_Pin_A15);

// инициализация данных для get_ampere, запись в буфер


  // indicate_ampere_get_init(test_ampere);
  // indicate_voltage_get_init(test_voltage);
  indicate_set_init(indicator_data, ampere_data, voltage_data);
  MAX7219_UpdateDisplay(indicator_data, MODE_SET, AMPERE);
  MAX7219_UpdateDisplay(indicator_data, MODE_SET, VOLTAGE);

  HAL_Delay(500); // timeout before interrup ON
  interrupt_status = 1; // interrupt ON
  // HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // HAL_Delay не позволительно


  while (1)
  {

    // if (status_mode == 1) {
    //
    //   if (position_on_mode_edit == 1) {
    //     MAX7219_SendDigit(set_number_for_get_ampere(), buffer_decode_variables_ampere[position_on_mode_edit], CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //     MAX7219_SendDigit(set_number_for_get_ampere(), 0x0F, CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //   } else {
    //     MAX7219_SendDigit(set_number_for_get_ampere(), buffer_decode_variables_ampere[get_number_for_get_ampere()], CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //     MAX7219_SendDigit(set_number_for_get_ampere(), 0x0F, CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //   }
    // } else {
    //   if (position_on_mode_edit == 1) {
    //     MAX7219_SendDigitWithDP(set_number_for_get_ampere(), buffer_decode_variables_ampere[get_number_for_get_ampere()], CS_GPIO_Port_A, CS_Pin_A15);
    //   } else {
    //     MAX7219_SendDigit(set_number_for_get_ampere(), buffer_decode_variables_ampere[get_number_for_get_ampere()], CS_GPIO_Port_A, CS_Pin_A15);
    //   }
    // }



    if (periphery_status.status_encoder_1) {
      uint32_t current_time_sig = HAL_GetTick();

      led1_set_state(3);

      if (current_time_sig - last_signal_edit_mode >= 350) {
        last_signal_edit_mode = current_time_sig;
        blink_set_state = !blink_set_state;
      }


      if (blink_set_state) {
       MAX7219_SendDigit(MAX7219_DetectedPosition(AMPERE, position_on_mode_edit), 0x0F, CS_GPIO_Port_A, CS_Pin_A15);
      } else {
        MAX7219_UpdateDisplay(indicator_data, MODE_SET, AMPERE);
      }

    } else {
      led1_set_state(4);

      MAX7219_UpdateDisplay(indicator_data, MODE_SET, AMPERE);
    }

    if (periphery_status.status_encoder_2) {
      uint32_t current_time_sig = HAL_GetTick();

      led1_set_state(3);
      if (current_time_sig - last_signal_edit_mode >= 350) {
        last_signal_edit_mode = current_time_sig;
        blink_set_state = !blink_set_state;
      }

      if (blink_set_state) {
        MAX7219_SendDigit(MAX7219_DetectedPosition(VOLTAGE, position_on_mode_edit), 0x0F, CS_GPIO_Port_A, CS_Pin_A15);
      } else {
        MAX7219_UpdateDisplay(indicator_data, MODE_SET, VOLTAGE);
      }

    } else {
      led1_set_state(4);

      MAX7219_UpdateDisplay(indicator_data, MODE_SET, VOLTAGE);
    }

    // if (status_mode == 2) {
    //
    //   if (position_on_mode_edit == 1) {
    //     MAX7219_SendDigit(set_number_for_get_voltage(), buffer_decode_variables_voltage[position_on_mode_edit], CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //     MAX7219_SendDigit(set_number_for_get_voltage(), 0x0F, CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //   } else {
    //     MAX7219_SendDigit(set_number_for_get_voltage(), buffer_decode_variables_voltage[get_number_for_get_voltage()], CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //     MAX7219_SendDigit(set_number_for_get_voltage(), 0x0F, CS_GPIO_Port_A, CS_Pin_A15);
    //     HAL_Delay(300);
    //   }
    //
    // } else {
    //   if (position_on_mode_edit == 1) {
    //     MAX7219_SendDigitWithDP(set_number_for_get_voltage(), buffer_decode_variables_voltage[get_number_for_get_voltage()], CS_GPIO_Port_A, CS_Pin_A15);
    //   } else {
    //     MAX7219_SendDigit(set_number_for_get_voltage(), buffer_decode_variables_voltage[get_number_for_get_voltage()], CS_GPIO_Port_A, CS_Pin_A15);
    //   }
    // }

    led2_set_state(2);
    led3_set_state(3);
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (interrupt_status == 1) {

    if (GPIO_Pin == ENCODER_S1_BTN_Pin) {
      // encoder_1_button_pressed(&status_mode, &position_on_mode_edit);
      periphery_status.status_press_encder_1 = !periphery_status.status_press_encder_1;
      periphery_status.status_encoder_1 = !periphery_status.status_encoder_1;

      if (periphery_status.status_press_encder_2) {
        periphery_status.status_encoder_2 = !periphery_status.status_encoder_2;
        periphery_status.status_press_encder_2 = 0;
      }

      position_on_mode_edit = 0;
    }

    if (GPIO_Pin == ENCODER_S2_BTN_Pin) {
      periphery_status.status_press_encder_2 = !periphery_status.status_press_encder_2;
      periphery_status.status_encoder_2 = !periphery_status.status_encoder_2;

      if (periphery_status.status_press_encder_1) {
        periphery_status.status_encoder_1 = !periphery_status.status_encoder_1;
        periphery_status.status_press_encder_1 = 0;
      }

      position_on_mode_edit = 0;
      // encoder_2_button_pressed(&status_mode, &position_on_mode_edit);
    }

// In Work !!!
    if (GPIO_Pin == ENC_A1_Pin && periphery_status.status_encoder_1) {
      encoder_1_init(indicator_data, MODE_SET, AMPERE);
      // MAX7219_UpdateDisplay(indicator_data, MODE_SET, AMPERE);
    }
    if (GPIO_Pin == ENC_A2_Pin && periphery_status.status_encoder_2) {
      encoder_2_init(indicator_data, MODE_SET, VOLTAGE);
      // MAX7219_UpdateDisplay(indicator_data, MODE_SET, VOLTAGE);
    }

    if (GPIO_Pin == BTN_S1_PE0_Pin && (periphery_status.status_encoder_1 || periphery_status.status_encoder_2)) {
      // switch_position_for_set_ampere();
      uint32_t current_time = HAL_GetTick();

      if (current_time - last_press_btn1 > 100) {
        last_press_btn1 = current_time;


        position_on_mode_edit++;

        if (position_on_mode_edit >= 4) {
          position_on_mode_edit = 0;
        }
      }
    } //else if (GPIO_Pin == BTN_S1_PE0_Pin && status_mode == 2) {
      // switch_position_for_set_voltage();
    // }

  }

}

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//   if (huart->Instance == USART1) {
//
//
//
//     HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
//   }
// }
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
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
