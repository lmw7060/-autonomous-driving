/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "delay.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, 0xFFFF);

  return ch;
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t INC_Value11 = 0;
uint32_t INC_Value12 = 0;

uint32_t INC_Value21 = 0;
uint32_t INC_Value22= 0;

uint32_t INC_Value31= 0;
uint32_t INC_Value32 = 0;


uint32_t echoTime1 = 0;
uint32_t echoTime2 = 0;
uint32_t echoTime3 = 0;

uint8_t captureFlag1 = 0;
uint8_t captureFlag2 = 0;
uint8_t captureFlag3 = 0;

uint8_t distance1  = 0;
uint8_t distance2  = 0;
uint8_t distance3  = 0;

uint8_t rxData[1];

int mode = 1;
int SPEED = 300;

volatile uint32_t counter;

int set_back_flag = 0;
char flag[15] = "INIT";

//uint8_t Is_First_Captured = 0;  // is the first value captured ?



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim3)  // if the interrupt source is channel1
  {
    if (captureFlag1 == 0) // if the first value is not captured
    {
      INC_Value11 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
      captureFlag1 = 1;  // set the first captured as true

      // Now change the polarity to falling edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }

    else if (captureFlag1 == 1)   // if the first is already captured
    {
      INC_Value12 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
      __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

      if (INC_Value12 > INC_Value11)
      {
        echoTime1 = INC_Value12-INC_Value11;
      }

      else if (INC_Value11 > INC_Value12)
      {
        echoTime1 = (0xffff - INC_Value11) + INC_Value12;
      }

      //distance = echoTime * .034/2;
      distance1 = echoTime1 / 58;
      captureFlag1 = 0; // set it back to false

      // set polarity to rising edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
  }
  else if (htim == &htim4)  // if the interrupt source is channel1
  {
    if (captureFlag2 == 0) // if the first value is not captured
    {
      INC_Value21 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
      captureFlag2 = 1;  // set the first captured as true

      // Now change the polarity to falling edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }

    else if (captureFlag2 == 1)   // if the first is already captured
    {
      INC_Value22 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
      __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

      if (INC_Value22 > INC_Value21)
      {
        echoTime2 = INC_Value22 - INC_Value21;
      }

      else if (INC_Value21 > INC_Value22)
      {
        echoTime2 = (0xffff - INC_Value21) + INC_Value22;
      }

      //distance = echoTime * .034/2;
      distance2 = echoTime2 / 58;
      captureFlag2 = 0; // set it back to false

      // set polarity to rising edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
  }
  else if (htim == &htim5)  // if the interrupt source is channel1
  {
    if (captureFlag3 == 0) // if the first value is not captured
    {
      INC_Value31 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
      captureFlag3 = 1;  // set the first captured as true

      // Now change the polarity to falling edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }

    else if (captureFlag3 == 1)   // if the first is already captured
    {
      INC_Value32 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
      __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

      if (INC_Value32 > INC_Value31)
      {
        echoTime3 = INC_Value32-INC_Value31;
      }

      else if (INC_Value31 > INC_Value32)
      {
        echoTime3 = (0xffff - INC_Value31) + INC_Value32;
      }

      //distance = echoTime * .034/2;
      distance3 = echoTime3 / 58;
      captureFlag3 = 0; // set it back to false

      // set polarity to rising edge
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
  }
}

void HCSR04_Read (GPIO_TypeDef* port, uint8_t pin, TIM_HandleTypeDef* htim)
{
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
  delay_us(10);  // wait for 10 us
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);  // pull the TRIG pin low

  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  HAL_UART_Receive_DMA(&huart1, rxData, sizeof(rxData));
  //      HAL_UART_Receive_IT(&huart1, rxData, sizeof(rxData));
  //      HAL_UART_Transmit_IT(&huart2, rxData, sizeof(rxData));

  //  else if(huart->Instance == USART2)
  //  {
  //      HAL_UART_Receive_IT(&huart2, rxData, sizeof(rxData));
  //  }
  if(rxData[0] == 'M')
  {
    mode = 1;
  }
  else if(rxData[0] == 'A')
  {
    mode = 2;
  }

  if(rxData[0] == 'u')
  {
    SPEED += 100;

    if (SPEED > 1000)
    {
      SPEED = 999;
    }
  }
  else if(rxData[0] == 'i')
  {
    SPEED -= 100;

    if (SPEED < 0)
    {
      SPEED = 0;
    }
  }
}

void forward()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
}

void backward()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
}

void left()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
}
void right()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
}
void stop()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_DMA(&huart1, rxData, sizeof(rxData));

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);

  HAL_TIM_Base_Start_IT(&htim11);

  HAL_TIM_Base_Start(&htim10);

  //  HAL_UART_Receive_IT(&huart1, rxData, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //      counter = __HAL_TIM_GET_COUNTER(&htim10);
    //      if (counter == 5000)
    //      {
    //          printf("%d \r\n", SPEED);
    //      }
    mode=2;
    if(mode == 0)
    {
      HCSR04_Read(GPIOA, GPIO_PIN_7, &htim3);
      HCSR04_Read(GPIOC, GPIO_PIN_7, &htim4);
      HCSR04_Read(GPIOA, GPIO_PIN_1, &htim5);

      if (__HAL_TIM_GET_COUNTER(&htim10) % 100 == 0)
      {
        printf("%d || %d || %d             %s\r\n", distance2, distance1, distance3, flag);
      }
      //forward 1,0 backward 0,1 brake 1,1

      //if wall brake and set flag
      if(distance1 <= 15 && set_back_flag == 0)
      {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
        set_back_flag = 1;
        strcpy(flag, "Brake!!");
      }

      //back flag set
      if(set_back_flag == 1)
      {
        if(distance1 >= 20)
        {
          set_back_flag = 0;
          strcpy(flag, "BACKWARD OFF");
        }
        else
        {
          backward();
          TIM1->CCR1 = 250;
          TIM2->CCR1 = 250;
          strcpy(flag, "BACKWARD ON");
        }
      }
      else
      {
        strcpy(flag, "FORWARD ON");
        forward();

        if(distance2 <= 28 || distance3 <= 28)
        {
          if (distance2 < distance3) //right
          {
            strcpy(flag, "-------------->");
            TIM1->CCR1 = 250;
            TIM2->CCR1 = 0;
          }
          else //left
          {
            strcpy(flag, "<--------------");
            TIM1->CCR1 = 0;
            TIM2->CCR1 = 250;
          }
        }
        else
        {
          TIM1->CCR1 = 250;
          TIM2->CCR1 = 250;
        }
      }
    }
    else if(mode == 1)
    {
      if(rxData[0] == 'w')
      {
        forward();

        TIM1->CCR1 = SPEED;
        TIM2->CCR1 = SPEED;
      }
      else if(rxData[0] == 's')
      {
        backward();

        TIM1->CCR1 = SPEED;
        TIM2->CCR1 = SPEED;
      }
      else if(rxData[0] == 'a')
      {
        left();

        TIM1->CCR1 = SPEED;
        TIM2->CCR1 = SPEED;
      }
      else if(rxData[0] == 'd')
      {
        right();

        TIM1->CCR1 = SPEED;
        TIM2->CCR1 = SPEED;
      }
      else if(rxData[0] == 'k')
      {
        stop();
      }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    else if(mode == 2)
    {
      delay_us(50);
      HCSR04_Read(GPIOA, GPIO_PIN_7, &htim3);
      delay_us(50);
      HCSR04_Read(GPIOC, GPIO_PIN_7, &htim4);
      delay_us(50);
      HCSR04_Read(GPIOA, GPIO_PIN_1, &htim5);
      delay_us(50);

      //distance 40 over
      if(distance1>40)
      {
        forward();
        TIM1->CCR1 = 380;
        TIM2->CCR1 = 380;
        if(distance2<10 || distance3 <10)
        {
          if(distance2<distance3)
          {
            right();
            TIM1->CCR1 = 0;
            TIM2->CCR1 = 800;
          }
          else if(distance2>distance3)
          {
            left();
            TIM1->CCR1 = 0;
            TIM2->CCR1 = 800;
          }
          else
          {
            backward();
            TIM1->CCR1 = 360;
            TIM2->CCR1 = 360;
          }
        }
      }
      //distance 33over
      else if(distance1<=40 && distance1>33)
      {
        forward();
        TIM1->CCR1 = 360;
        TIM2->CCR1 = 360;
        if(distance2<20 || distance3 <20)
        {
          if(distance2<distance3)
          {
            right();
            TIM1->CCR1 = 700;
            TIM2->CCR1 = 700;
          }
          else if(distance2>distance3)
          {
            left();
            TIM1->CCR1 = 700;
            TIM2->CCR1 = 700;
          }
          else
          {
            backward();
            TIM1->CCR1 = 340;
            TIM2->CCR1 = 340;
          }
        }
      }
      //distance 25over
      else if(distance1<=33 && distance1>25)
      {
        forward();
        TIM1->CCR1 = 340;
        TIM2->CCR1 = 340;
        if(distance2<20 || distance3 <20)
        {
          if(distance2<distance3)
          {
            right();
            TIM1->CCR1 = 700;
            TIM2->CCR1 = 700;
          }
          else if(distance2>distance3)
          {
            left();
            TIM1->CCR1 = 700;
            TIM2->CCR1 = 700;
          }
          else
          {
            backward();
            TIM1->CCR1 = 320;
            TIM2->CCR1 = 320;
          }
        }
      }
      //distance 18over
      else if(distance1<=25 && distance1>18)
      {
        forward();
        TIM1->CCR1 = 320;
        TIM2->CCR1 = 320;
        if(distance2<20 || distance3 <20)
        {
          if(distance2<distance3)
          {
            right();
            TIM1->CCR1 = 600;
            TIM2->CCR1 = 600;
          }
          else if(distance2>distance3)
          {
            left();
            TIM1->CCR1 = 600;
            TIM2->CCR1 = 600;
          }
          else
          {
            backward();
            TIM1->CCR1 = 300;
            TIM2->CCR1 = 300;
          }
        }
      }
      //distance 10over
      else if(distance1<=18 && distance1>10)
      {
        if(distance2<20 || distance3 <20)
        {
          forward();
          TIM1->CCR1 = 300;
          TIM2->CCR1 = 300;
          if(distance2<distance3)
          {
            right();
            TIM1->CCR1 = 600;
            TIM2->CCR1 = 600;
          }
          else if(distance2>distance3)
          {
            left();
            TIM1->CCR1 = 600;
            TIM2->CCR1 = 600;
          }
          else
          {
            backward();
            TIM1->CCR1 = 300;
            TIM2->CCR1 = 300;
          }
        }
      }
      //distance 10under
      else if(distance1<=10)
      {
        backward();
        TIM1->CCR1 = 450;
        TIM2->CCR1 = 450;
      }
    }

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

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

#ifdef  USE_FULL_ASSERT
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
