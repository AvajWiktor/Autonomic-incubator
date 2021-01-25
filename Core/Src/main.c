/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp280.h"
#include "bmp280_defs.h"




#include "M_M_functions.h"
//#include "stdio.h"
//#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SPI_BUFFER_LEN 28
#define BMP280_DATA_INDEX 1
#define BMP280_ADDRESS_INDEX 2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Received message */
char Received_msg[9];

/* Set point values*/
uint8_t T_int= 18; //Temperatura
uint16_t L = 1000; //Natezenie swiatla w LUX
uint8_t H = 70; //Wilgotnosc w $

/* PRZYPISANIE WARTOSCI POSZCZEGOLNYCH CZLONOW REGULATORA */
float Kp = 0.25f; // cz³on proporcjonalny 
float Kd = 0.001f; // cz³on ca³kuj¹cy
float Ki = 0.08f; // cz³on ró¿niczkuj¹cy

/* INSTANCJA PID */
arm_pid_instance_f32 light_PID; // instancja PID œwiat³a 

//i2c variables*******************************
int BH1750_int = 0; // wartosc pomiaru swiatla
int Uart_data[12]; // zmienna przechowuj¹ca wiadomosc do wyslania portem szeregowym
char LCD_msg[16]; // wiadomosc do wyswietlenia na LCD
uint8_t i2c_size; // rozmiar wiadomosci
//********************************************
//ADC variables*******************************

uint16_t adc_value; //wartosc odczytu temperatury
uint16_t adc_data; //dane z czujnika


//********************************************

//spi variables*******************************
float temperatura = 0.0; //dane dla LCD

int8_t rslt; 
	   struct bmp280_dev bmp;
	   struct bmp280_config conf;
	   struct bmp280_uncomp_data ucomp_data;
	   int32_t temp32;
	   uint32_t press32;
	   double press;
	   double temp;
	   float pres1=0,temp1=0;
	   char buffer[40];
	   int timer =0;
	   uint8_t size;
//*********************************************
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

//funkcja odczytuj¹ca wartoœci z czujnika temperatury
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr , uint8_t *reg_data , uint16_t length)
{
  HAL_StatusTypeDef status = HAL_OK;
  int32_t iError = BMP280_OK;
  uint8_t txarray[SPI_BUFFER_LEN] = {0,};
  uint8_t rxarray[SPI_BUFFER_LEN] = {0,};
  uint8_t stringpos;
 txarray[0] = reg_addr;
 HAL_GPIO_WritePin(SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_RESET );
 status = HAL_SPI_TransmitReceive( &hspi4 , (uint8_t*)(&txarray), (uint8_t*)(&rxarray), length+1, 5);
 while( hspi4.State == HAL_SPI_STATE_BUSY ) {};
 HAL_GPIO_WritePin( SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_SET );
 for (stringpos = 0; stringpos < length; stringpos++)
 {
	 *(reg_data + stringpos) = rxarray[stringpos + BMP280_DATA_INDEX];
 }
 if (status != HAL_OK)
 {
	 iError = (-1);
 }
 return (int8_t)iError;
}

int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr , uint8_t *reg_data , uint16_t length)
{
	HAL_StatusTypeDef status = HAL_OK;
	int32_t iError = BMP280_OK;
	uint8_t txarray[SPI_BUFFER_LEN * BMP280_ADDRESS_INDEX];
	uint8_t stringpos;
	txarray[0] = reg_addr;

	for (stringpos = 0; stringpos < length; stringpos++)
	{
		txarray[stringpos+BMP280_DATA_INDEX] = reg_data[stringpos];
	}

	HAL_GPIO_WritePin( SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_RESET );
	status = HAL_SPI_Transmit( &hspi4 , (uint8_t*)(&txarray), length*2, 100);
	while( hspi4.State == HAL_SPI_STATE_BUSY )
	{

	};
	HAL_GPIO_WritePin( SPI4_CS_GPIO_Port , SPI4_CS_Pin , GPIO_PIN_SET );

	if (status != HAL_OK)
	{
		iError = (-1);
	}
	return (int8_t)iError;
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

	/* PRZEKAZANIE WARTOSCI CZLONOW REGULATORA DO SAMEGO PID */
	  light_PID.Kp = Kp;
	  light_PID.Ki = Ki;
	  light_PID.Kd = Kd;



  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */


 // print_rslt(" bmp280_set_power_mode status", rslt);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI4_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  //bh1750 init*******************************************
  BH1750_Init(&hi2c1);
  BH1750_SetMode(CONTINUOUS_HIGH_RES_MODE);
  lcd_init();
  arm_pid_init_f32(&light_PID, 1);


  //******************************************************

  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_ADC_Start_DMA(&hadc1, &adc_data, 1);
  HAL_UART_Receive_IT(&huart3, &Received_msg,9);


  //bmp280 init*******************************************
  bmp.delay_ms = HAL_Delay;
   bmp.dev_id = 0;
   bmp.intf = BMP280_SPI_INTF;
   bmp.read = spi_reg_read;
   bmp.write = spi_reg_write;
   rslt = bmp280_init(&bmp);
   rslt = bmp280_get_config(&conf, &bmp);
   conf.filter = BMP280_FILTER_COEFF_2;
   conf.os_temp = BMP280_OS_4X;
   conf.os_pres = BMP280_OS_4X;
   conf.odr = BMP280_ODR_1000_MS;
   rslt = bmp280_set_config(&conf, &bmp);
   rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);
   //*****************************************************
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 

	  BH1750_int = ReadData();
	  SetValue(L, BH1750_int, &light_PID);

	  //za³¹czanie wiatraka gdy temp jest wy¿sza ni¿ zadana 
	  	    if(temp1 > T_int)
	  	    {
	  	    	
	  	    	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1000);
	  	    }
	  	    else
	  	    {
	  	    	
	  	    	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	  	    }
	//za³¹czanie pompy wody, gdy wilgotnoœæ jest zbyy ma³a
	  	    if(adc_value < H)
	  	    {
	  	    	
	  	    	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 1000);
	  	    }
	  	    else
	  	    {
	  	    	
	  	    	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
	  	    }


			//odczyt danych z czujnika temp
	         rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);
	         rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);

	         temp1 = temp;
	         temperatura = temp1;
	         

	         bmp.delay_ms(1000);
	         timer++;




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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef * hadc)
{
	

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{




}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
		{

			adc_value = 0.714*(4200.0 - adc_data)/21.0; //konwersja wartoœci analogowej na u¿yteczne dane 

			RefreshLCD_3v(temperatura, adc_value, BH1750_int); //odœwie¿enie danych na wyœwietlaczu

			i2c_size = sprintf(Uart_data, "L:%d,H:%d,T:%.2f\n\r", BH1750_int, adc_value, temperatura); 

			HAL_UART_Transmit_IT(&huart3, (uint8_t*)Uart_data, i2c_size); 
			//transmisja danych po porcie szeregowym 


		}

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	char value1[4]; //light
	char value2[3]; //humm
	char value3[2]; //temp
	int value_i1, value_i2 ,value_i3;

	value1[0] = Received_msg[0];
	value1[1] = Received_msg[1];
	value1[2] = Received_msg[2];
	value1[2] = Received_msg[3];

	value2[0] = Received_msg[4];
	value2[1] = Received_msg[5];
	value2[2] = Received_msg[6];

	value3[0] = Received_msg[7];
	value3[1] = Received_msg[8];


	value_i1 = 10*(atoi(value1));
	value_i2 = (atoi(value2));
	value_i3= (atoi(value3));

	L = value_i1;
	H = value_i2;
	T_int = value_i3;
	memset(Received_msg, 0, sizeof(Received_msg));

	HAL_UART_Receive_IT(&huart3, &Received_msg,9);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
