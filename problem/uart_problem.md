# 串口波特率的计算问题

```c
  RCC_GetClocksFreq(&RCC_ClocksStatus);

  if (usartxbase == USART1_BASE)
  {
    apbclock = RCC_ClocksStatus.APB2CLK_Freq;
  }
  else
  {
    apbclock = RCC_ClocksStatus.APB1CLK_Freq;
  } 
/* Determine the integer part */
  if ((USARTx->CTRL1 & CTRL1_OVER8_Set) != 0)
  {
    /* Integer part computing in case Oversampling mode is 8 Samples */
    integerdivider = ((25 * apbclock) / (2 * (USART_InitStruct->USART_BaudRate)));
  }
  else /* if ((USARTx->CTRL1 & CTRL1_OVER8_Set) == 0) */
  {
    /* Integer part computing in case Oversampling mode is 16 Samples */
    integerdivider = ((25 * apbclock) / (4 * (USART_InitStruct->USART_BaudRate)));
  }

  tmpreg = (integerdivider / 100) << 4;

  /* Determine the fractional part */
  fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  /* Implement the fractional part in the register */
  if ((USARTx->CTRL1 & CTRL1_OVER8_Set) != 0)
  {
    tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
  }
  else /* if ((USARTx->CTRL1 & CTRL1_OVER8_Set) == 0) */
  {
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
  }

//integerdivider = ((25 * apbclock) / (4 * (USART_InitStruct->USART_BaudRate)));
//apblock的值是时钟频率，我个人采用的方法是时钟除以baud*16，得到div值。
//然后div值乘以16，然后把他和0xf相与，得到小数部分。
//然后把div的值右移4位，得到整数值
//但是官方库的算法，我有点晕，25*apbclock，如果apbclock的值超过(2^32-1)/25时，会造成溢出。
//有点晕。
```

