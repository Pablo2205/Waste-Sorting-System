/**
 * @file main.c
 * @brief Smart Waste Manager - STM32F410RB
 * @author Pablo Coria
 * @date 2025
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
#include "config.h"
#include "sensors.h"
#include "classifier.h"
#include "actuators.h"
#include "display.h"
#include "statistics.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
Statistics stats = {0};

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint16_t adc_buffer[ADC_BUFFER_SIZE];
SystemState current_state = STATE_IDLE;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private user code ---------------------------------------------------------*/

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();        // TIM5 en lugar de TIM4
  MX_USART1_UART_Init(); // USART1 en lugar de USART2

  /* USER CODE BEGIN 2 */
  
  // Iniciar ADC con DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, ADC_BUFFER_SIZE);

  // Iniciar PWM para servos
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);  // PA8 - Servo Plataforma
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);  // PA9 - Servo Metal
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);  // PA10 - Servo Papel
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);  // PB6 - Servo Plástico
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);  // PB7 - Servo Vidrio

  // Inicializar módulos del sistema
  sensors_init();
  classifier_init();
  actuators_init();
  display_init();
  statistics_init(&stats);

  // Mostrar mensaje de bienvenida
  display_show_welcome();
  HAL_Delay(2000);

  printf("Smart Waste Manager STM32F410RB - Iniciado\r\n");
  printf("Materiales: Metal, Papel, Plástico, Vidrio\r\n");
  printf("Servos: TIM1 (3) + TIM5 (2)\r\n");
  printf("UART: USART1 para debug\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // 1. Esperar detección
    if (sensors_detect_presence()) {
      display_show_detecting();

      // 2. Leer sensores
      SensorDigitalData digital = sensors_read_digital();
      SensorAnalogData analog = sensors_read_analog(adc_buffer);

      // 3. Clasificar
      ClassificationResult result = classifier_classify(digital, analog);

      // 4. Validar
      if (result.isValid && result.confidence > 60.0) {
        // 5. Actuar
        actuators_deposit_material(result.material);

        // 6. Actualizar estadísticas
        statistics_update(&stats, result);

        // 7. Mostrar
        display_show_result(result);
        display_show_statistics(&stats);
      } else {
        display_show_error("No identificado");
      }
    }

    HAL_Delay(100);
  }
  /* USER CODE END WHILE */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// Redirigir printf a USART1
int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
  return len;
}

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