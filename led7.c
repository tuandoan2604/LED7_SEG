#include "led7.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

uint8_t LED7_HEX[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
uint8_t LED7_DOT[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};
uint8_t bufferLED[4];
void LED7_Writenumhexyte(uint8_t numhex);

static void off7Seg()
{
	LED1_RESET();
	LED2_RESET();
	LED3_RESET();
	LED4_RESET();
}

void LED7_WriteByte(uint8_t numhex, uint8_t ledStat)
{
	scan7Seg(ledStat);
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOA, (0x00000001 << i), (numhex >> i) & 1);
	}
}

void scan7Seg(uint8_t ledStat)
{
	off7Seg();
	if (ledStat == 0)
	{
		LED1_RESET();
		LED2_RESET();
		LED3_RESET();
		LED4_SET();
	}
	else if (ledStat == 1)
	{
		LED1_RESET();
		LED2_RESET();
		LED3_SET();
		LED4_RESET();
	}
	else if (ledStat == 2)
	{
		LED1_RESET();
		LED2_SET();
		LED3_RESET();
		LED4_RESET();
	}
	else if (ledStat == 3)
	{
		LED1_SET();
		LED2_RESET();
		LED3_RESET();
		LED4_RESET();
	}
}

bool isInteger(double val)
{
	int truncated = (int)val;
	return (val == truncated);
}

static void reverse(char *str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x)
	{
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
		str[i++] = '0';

	reverse(str, i);
	str[i] = '\0';
	return i;
}

void ftoa(float n, char *res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;

	// Extract floating part
	float fpart = n - (float)ipart;

	// convert integer part to string
	int i = intToStr(ipart, res, 0);

	// check for display option after point
	if (afterpoint != 0)
	{
		res[i] = '.'; // add dot

		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter
		// is needed to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);

		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

void LED7_Display(double num, uint8_t cnt)
{
	if (isInteger(num))
	{
		char buf[5];
		sprintf(buf, "%d", (int)num);
		bufferLED[0] = LED7_HEX[(uint8_t)(buf[3] - 0x30)];
		bufferLED[1] = LED7_HEX[(uint8_t)(buf[2] - 0x30)];
		bufferLED[2] = LED7_HEX[(uint8_t)(buf[1] - 0x30)];
		bufferLED[3] = LED7_HEX[(uint8_t)(buf[0] - 0x30)];
	}
	else
	{
		char buf[6];
		int i = 0;
		ftoa(num, buf, 3);
		for (i = 0; i < 6; i++)
		{
			if (buf[i] == 0x2E)
				break;
		}
		switch (i)
		{
		case 1:
			bufferLED[0] = LED7_HEX[(uint8_t)(buf[4] - 0x30)];
			bufferLED[1] = LED7_HEX[(uint8_t)(buf[3] - 0x30)];
			bufferLED[2] = LED7_HEX[(uint8_t)(buf[2] - 0x30)];
			bufferLED[3] = LED7_DOT[(uint8_t)(buf[0] - 0x30)];
			break;
		case 2:
			bufferLED[0] = LED7_HEX[(uint8_t)(buf[4] - 0x30)];
			bufferLED[1] = LED7_HEX[(uint8_t)(buf[3] - 0x30)];
			bufferLED[2] = LED7_DOT[(uint8_t)(buf[1] - 0x30)];
			bufferLED[3] = LED7_HEX[(uint8_t)(buf[0] - 0x30)];
			break;
		case 3:
			bufferLED[0] = LED7_HEX[(uint8_t)(buf[4] - 0x30)];
			bufferLED[1] = LED7_DOT[(uint8_t)(buf[2] - 0x30)];
			bufferLED[2] = LED7_HEX[(uint8_t)(buf[1] - 0x30)];
			bufferLED[3] = LED7_HEX[(uint8_t)(buf[0] - 0x30)];
			break;
		}
	}
	LED7_WriteByte(bufferLED[cnt], cnt);
}
