#include "stm32f10x.h"
#include "spi.h"
#include "oled.h"
#include "gpio.h"
#include "delay.h"
#include "date.h"

//***************************************字库**********************************************//

static const u8 number[][16]={
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//0 0
0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,//1 1
0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//2 2
0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,//3 3
0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,//4 4
0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,//5 5
0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00,//6 6
0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00,//7 7
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//8 8
0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00,//9 9
0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A 10
0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B 11
0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C 12
0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D 13
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E 14
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F 15
0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G 16
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H 17
0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I 18
0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J 19
0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K 20
0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L 21
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x01,0x3E,0x01,0x3F,0x20,0x00,//M 22
0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N 23
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O 24
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P 25
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x28,0x28,0x30,0x50,0x4F,0x00,//Q 26
0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R 27
0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S 28
0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T 29
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U 30
0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V 31
0x08,0xF8,0x00,0xF8,0x00,0xF8,0x08,0x00,0x00,0x03,0x3E,0x01,0x3E,0x03,0x00,0x00,//W 32
0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X 33
0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y 34
0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z 35
0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00,//a 36
0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b 37
0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c 38
0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//d 39
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00,//e 40
0x00,0x80,0x80,0xE0,0x90,0x90,0x20,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f 41
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g 42
0x10,0xF0,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h 43
0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i 44
0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j 45
0x10,0xF0,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x06,0x29,0x30,0x20,0x00,//k 46
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l 47
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m 48
0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n 49
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o 50
0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0x91,0x20,0x20,0x11,0x0E,0x00,//p 51
0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0x91,0xFF,0x80,//q 52
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r 53
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s 54
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x00,//t 55
0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u 56
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x03,0x0C,0x30,0x0C,0x03,0x00,0x00,//v 57
0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x01,0x0E,0x30,0x0C,0x07,0x38,0x06,0x01,//w 58
0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x20,0x31,0x0E,0x2E,0x31,0x20,0x00,//x 59
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x81,0x86,0x78,0x18,0x06,0x01,0x00,//y 60
0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z 61
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x70,0x00,0x00,0x00,0x00,0x00,//,	62
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//.	63
0x00,0x00,0x00,0x00,0xC0,0x38,0x04,0x00,0x00,0x60,0x18,0x07,0x00,0x00,0x00,0x00,///	64
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//:	65
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,//;	66
0x00,0x10,0x0C,0x02,0x10,0x0C,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//"	67
0x00,0x12,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//'	68
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//|	69
0x00,0x04,0x38,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\	70
0x00,0x02,0x01,0x02,0x02,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~	71
0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//`	72
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x00,0x00,0x00,0x00,//！73
0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x28,0x2F,0x28,0x17,0x00,//@ 74
0x00,0x40,0xC0,0x78,0x40,0xC0,0x78,0x00,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x00,//# 75
0x00,0x70,0x88,0x88,0xFC,0x08,0x30,0x00,0x00,0x18,0x20,0x20,0xFF,0x21,0x1E,0x00,//$ 76
0xF0,0x08,0xF0,0x80,0x60,0x18,0x00,0x00,0x00,0x31,0x0C,0x03,0x1E,0x21,0x1E,0x00,//% 77
0x00,0x00,0x04,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^ 78
0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x2C,0x19,0x27,0x21,0x10,//& 79
0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//* 80
0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//( 81
0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//) 82
0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x01,0x01,0x01,//+ 83
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,//- 84
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_ 85
0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x00 //= 86
};


static const u8 Hanzi[][32]={
0x08,0x08,0x89,0xEA,0x18,0x88,0x00,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x02,0x01,0x00,0xFF,0x01,0x86,0x40,0x20,0x18,0x07,0x40,0x80,0x40,0x3F,0x00,0x00,//初	0
0x10,0x10,0xF0,0x1F,0x10,0xF0,0x00,0x40,0xE0,0x58,0x47,0x40,0x50,0x60,0xC0,0x00,
0x40,0x22,0x15,0x08,0x16,0x21,0x00,0x00,0xFE,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,//始	1
0x00,0x80,0x60,0xF8,0x07,0x00,0x00,0x00,0xFF,0x40,0x20,0x10,0x08,0x04,0x00,0x00,
0x01,0x00,0x00,0xFF,0x00,0x04,0x02,0x01,0x3F,0x40,0x40,0x40,0x40,0x40,0x78,0x00,//化	2
0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00,
0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00,//中	3
0x00,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//…		4
0x10,0x10,0xD0,0xFF,0x90,0x14,0xE4,0xAF,0xA4,0xA4,0xA4,0xAF,0xE4,0x04,0x00,0x00,
0x04,0x03,0x00,0xFF,0x00,0x89,0x4B,0x2A,0x1A,0x0E,0x1A,0x2A,0x4B,0x88,0x80,0x00,//模	5
0x10,0x10,0x90,0x90,0x90,0x90,0x90,0x10,0x10,0xFF,0x10,0x10,0x11,0x16,0x10,0x00,
0x00,0x20,0x60,0x20,0x3F,0x10,0x10,0x10,0x00,0x03,0x0C,0x10,0x20,0x40,0xF8,0x00,//式	6
0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00,
0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00,0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00,//时	7
0x00,0xF8,0x01,0x06,0x00,0xF0,0x12,0x12,0x12,0xF2,0x02,0x02,0x02,0xFE,0x00,0x00,
0x00,0xFF,0x00,0x00,0x00,0x1F,0x11,0x11,0x11,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00,//间	8
0x10,0x0C,0x44,0x44,0x44,0x44,0x45,0xC6,0x44,0x44,0x44,0x44,0x44,0x14,0x0C,0x00,
0x80,0x40,0x20,0x1E,0x20,0x40,0x40,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00,//定	9
0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00,
0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00,0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00,//时	10
0x04,0x84,0xE4,0x5C,0x44,0xC4,0x20,0x10,0xE8,0x27,0x24,0xE4,0x34,0x2C,0xE0,0x00,
0x02,0x01,0x7F,0x10,0x10,0x3F,0x80,0x60,0x1F,0x09,0x09,0x3F,0x49,0x89,0x7F,0x00,//确	11
0x10,0x0C,0x44,0x44,0x44,0x44,0x45,0xC6,0x44,0x44,0x44,0x44,0x44,0x14,0x0C,0x00,
0x80,0x40,0x20,0x1E,0x20,0x40,0x40,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00,//定	12
0x02,0x02,0xFE,0x92,0x92,0x92,0xFE,0x02,0x06,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x08,0x18,0x0F,0x08,0x08,0x04,0xFF,0x04,0x84,0x40,0x27,0x18,0x27,0x40,0x80,0x00,//取	13
0x10,0x60,0x02,0x0C,0xC0,0x00,0xE2,0x2C,0x20,0x3F,0x20,0x28,0xE6,0x00,0x00,0x00,
0x04,0x04,0x7C,0x03,0x00,0x00,0xFF,0x09,0x09,0x09,0x49,0x89,0x7F,0x00,0x00,0x00,//消	14
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x90,0xA0,0xC0,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x01,0x00,//箭头	15
0x02,0xE2,0x22,0x22,0x3E,0x00,0x80,0x9E,0x92,0x92,0xF2,0x92,0x92,0x9E,0x80,0x00,
0x00,0x43,0x82,0x42,0x3E,0x40,0x47,0x44,0x44,0x44,0x7F,0x44,0x44,0x54,0xE7,0x00,//强	16
0x40,0x50,0x4E,0x48,0x48,0xFF,0x48,0x48,0x48,0x40,0xF8,0x00,0x00,0xFF,0x00,0x00,
0x00,0x00,0x3E,0x02,0x02,0xFF,0x12,0x22,0x1E,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,//制	17
0x00,0x00,0x00,0xF8,0x88,0x8C,0x8A,0x89,0x88,0x88,0x88,0xF8,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0xFF,0x00,0x00,0x00,0x00,//自	18
0x40,0x44,0xC4,0x44,0x44,0x44,0x40,0x10,0x10,0xFF,0x10,0x10,0x10,0xF0,0x00,0x00,
0x10,0x3C,0x13,0x10,0x14,0xB8,0x40,0x30,0x0E,0x01,0x40,0x80,0x40,0x3F,0x00,0x00,//动	19
0x40,0x40,0x42,0xCC,0x00,0x40,0xA0,0x9E,0x82,0x82,0x82,0x9E,0xA0,0x20,0x20,0x00,
0x00,0x00,0x00,0x3F,0x90,0x88,0x40,0x43,0x2C,0x10,0x28,0x46,0x41,0x80,0x80,0x00,//设	20
0x00,0x17,0x15,0xD5,0x55,0x57,0x55,0x7D,0x55,0x57,0x55,0xD5,0x15,0x17,0x00,0x00,
0x40,0x40,0x40,0x7F,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x7F,0x40,0x40,0x40,0x00,//置	21
0x80,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x80,0x00,
0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,//开	22
0x80,0x70,0x00,0xFF,0x20,0x10,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00,
0x80,0x60,0x18,0x07,0x08,0x30,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,//灯	23
0x00,0x00,0x10,0x11,0x16,0x10,0x10,0xF0,0x10,0x10,0x14,0x13,0x10,0x00,0x00,0x00,
0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03,0x0D,0x11,0x21,0x41,0x41,0x81,0x81,0x00,//关	24





	
};

//********************************************************初始化函数***********************************************************************//

void OLED_Init(void)
{
	uint8_t i;
	OLED_SPI_RES_OFF;//复位一次
	for(i=0;i<100;i++);
	OLED_SPI_RES_ON;
	
	OLED_WRITE(0xAE,OLED_COM);		//关闭显示
	OLED_WRITE(0xD5,OLED_COM);		//设置时钟分频因子,震荡频率
	OLED_WRITE(0x50,OLED_COM);		//[3:0],分频因子;[7:4],震荡频率
	OLED_WRITE(0xA8,OLED_COM);		//设置驱动路数
	OLED_WRITE(0x3F,OLED_COM);		//默认0X3F(1/64)
	
	OLED_WRITE(0xD3,OLED_COM);		//设置显示偏移
	OLED_WRITE(0x00,OLED_COM);		//默认为0
	OLED_WRITE(0x40,OLED_COM);		//设置显示开始行 [5:0],行数.
	OLED_WRITE(0x8D,OLED_COM);		//电荷泵设置
	OLED_WRITE(0x14,OLED_COM);		//bit2，开启/关闭
	
	OLED_WRITE(0x20,OLED_COM);		//设置内存寻址模式
	OLED_WRITE(0x02,OLED_COM);		//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;	
	OLED_WRITE(0xA1,OLED_COM);		//段重定义设置,bit0: 0,column 0->SEG 0;  1,column 0->SEG 127;
	OLED_WRITE(0xC8,OLED_COM);		//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WRITE(0xDA,OLED_COM);		//设置COM硬件引脚配置  [5:4]配置  配置COM（SSD1306）与ROW（液晶）的连接关系
	OLED_WRITE(0x12,OLED_COM);		//[5:4]配置  配置COM（SSD1306）与ROW（液晶）的连接关系
	
	OLED_WRITE(0x81,OLED_COM);		//对比度设置
	OLED_WRITE(0xFE,OLED_COM);		//1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WRITE(0xD9,OLED_COM);		//设置预充电周期
	OLED_WRITE(0xF1,OLED_COM);		//[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WRITE(0xDB,OLED_COM);		//设置VCOMH 电压倍率
	OLED_WRITE(0x30,OLED_COM);		//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WRITE(0xA4,OLED_COM);		//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WRITE(0xA6,OLED_COM);		//设置显示方式;bit0:1,反相显示;0,正常显示
	OLED_WRITE(0xAF,OLED_COM);		//开启显示

}

/****************************************************************通用设置函数****************************************************************/

void OLED_Display_Off(void)
{
	OLED_WRITE(0x8D,OLED_COM);		//SET DCDC命令
	OLED_WRITE(0x10,OLED_COM);		//DCDC OFF
	OLED_WRITE(0xAE,OLED_COM);		//DISPLAY OFF
}

void OLED_Display_On(void)
{
	OLED_WRITE(0x8D,OLED_COM);		//SET DCDC命令
	OLED_WRITE(0x14,OLED_COM);		//DCDC ON
	OLED_WRITE(0xAF,OLED_COM);		//DISPLAY ON
}

void OLED_SetPos(u8 x, u8 y) 
{   
	//以下3个寄存器只在页寻址的模式下有效	  
	OLED_WRITE(0xb0+y, OLED_COM);				//页地址设置     0xb0~0xb7
	OLED_WRITE(((x&0xf0)>>4)|0x10, OLED_COM);	//列高位地址设置
	OLED_WRITE((x&0x0f), OLED_COM);				//列低位地址设置
} 

void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WRITE (0xb0+i, OLED_COM);			//设置页地址（0~7）
		OLED_WRITE (0x00, OLED_COM);			//设置显示位置—列低地址
		OLED_WRITE (0x10, OLED_COM);			//设置显示位置—列高地址
		for(n=0;n<129;n++)
			OLED_WRITE(0, OLED_DATA);
	}											//更新显示
}

void OLED_Display(u8 x, u8 y,u8 c,u8 size,u8 underline)
{
	int i;
	if(size == 8)
	{
		if(underline == 0)
		{
			OLED_SetPos(x,y);
			for(i=0;i<8;i++)
			{
				OLED_WRITE(number[c][i], OLED_DATA);			//先写上半部分
			}
			OLED_SetPos(x,y+1);
			for(i=0;i<8;i++)
			{
				OLED_WRITE(number[c][i+8], OLED_DATA);			//后写下半部分
			}
		}
		else
		{
			OLED_SetPos(x,y);
			for(i=0;i<8;i++)
			{
				OLED_WRITE(number[c][i], OLED_DATA);			//先写上半部分
			}
			OLED_SetPos(x,y+1);
			for(i=0;i<8;i++)
			{
				OLED_WRITE(number[c][i+8] | 0x80, OLED_DATA);	//后写下半部分
			}
		}
	}
	else
	{
		OLED_SetPos(x,y);
		for(i=0;i<16;i++)
		{
			OLED_WRITE(Hanzi[c][i], OLED_DATA);					//先写上半部分
		}
		OLED_SetPos(x,y+1);
		for(i=0;i<16;i++)
		{
			OLED_WRITE(Hanzi[c][i+16], OLED_DATA);				//后写下半部分
		}
	}
}

/********************************************************功能显示函数**********************************************************************/

////////////////////////////////////////////////显示“初始化中……”函数

void Display_Init(void)
{
	int x=16,y=4;
	OLED_Display(x=x+0,y,0,16,0);			//初
	OLED_Display(x=x+16,y,1,16,0);			//始
	OLED_Display(x=x+16,y,2,16,0);			//化
	OLED_Display(x=x+16,y,3,16,0);			//中
	OLED_Display(x=x+16,y,4,16,0);			//…
	OLED_Display(x=x+16,y,4,16,0);			//…
}

///////////////////////////////////////////////时间显示函数

void Date_Display(Date_InitTypeDef* Date, u8 i)
{
	int x,y;
	OLED_Display(x=24, y=0, ((Date -> year)/1000)%10, 8, 0);
	OLED_Display(x=x+8, y, ((Date -> year)/100)%10, 8, 0);
	OLED_Display(x=x+8, y, ((Date -> year)/10)%10, 8, 0);
	OLED_Display(x=x+8, y, (Date -> year)%10, 8, 0);
	OLED_Display(x=x+8, y, 64, 8, 0);
	OLED_Display(x=x+8, y, ((Date -> month)/10)%10, 8, 0);
	OLED_Display(x=x+8, y, (Date -> month)%10, 8, 0);
	OLED_Display(x=x+8, y, 64, 8, 0);
	OLED_Display(x=x+8, y, ((Date -> date)/10)%10, 8, 0);
	OLED_Display(x=x+8, y, (Date -> date)%10, 8, 0);
	OLED_Display(x=32, y=2, ((Date -> hour)/10)%10, 8, 0);
	OLED_Display(x=x+8, y, (Date -> hour)%10, 8, 0);
	OLED_Display(x=x+8, y, 65, 8, 0);
	OLED_Display(x=x+8, y, ((Date -> minute)/10)%10, 8, 0);
	OLED_Display(x=x+8, y, (Date -> minute)%10, 8, 0);
	OLED_Display(x=x+8, y, 65, 8, 0);
	OLED_Display(x=x+8, y, ((Date -> second)/10)%10, 8, 0);
	OLED_Display(x=x+8, y, (Date -> second)%10, 8, 0);
	switch(i)
	{
		case 0:
			break;
		case 1:
			OLED_Display(24, 0, ((Date -> year)/1000)%10, 8, 1);
			break;
		case 2:
			OLED_Display(32, 0, ((Date -> year)/100)%10, 8, 1);
			break;
		case 3:
			OLED_Display(40, 0, ((Date -> year)/10)%10, 8, 1);
			break;
		case 4:
			OLED_Display(48, 0, (Date -> year)%10, 8, 1);
			break;
		case 5:
			OLED_Display(64, 0, ((Date -> month)/10)%10, 8, 1);
			break;
		case 6:
			OLED_Display(72, 0, (Date -> month)%10, 8, 1);
			break;
		case 7:
			OLED_Display(88, 0, ((Date -> date)/10)%10, 8, 1);
			break;
		case 8:
			OLED_Display(96, 0, (Date -> date)%10, 8, 1);
			break;
		case 9:
			OLED_Display(32, 2, ((Date -> hour)/10)%10, 8, 1);
			break;
		case 10:
			OLED_Display(40, 2, (Date -> hour)%10, 8, 1);
			break;
		case 11:
			OLED_Display(56, 2, ((Date -> minute)/10)%10, 8, 1);
			break;
		case 12:
			OLED_Display(64, 2, (Date -> minute)%10, 8, 1);
			break;
		case 13:
			OLED_Display(80, 2, ((Date -> second)/10)%10, 8, 1);
			break;
		case 14:
			OLED_Display(88, 2, (Date -> second)%10, 8, 1);
			break;
	}
}

//////////////////////////////////////////////////////箭头显示函数

void Display_Arrows(s8 x)
{
	OLED_Display(0, 2*x, 15, 16, 0);
}

//////////////////////////////////////////////////////界面1 显示函数

void Interface_1_Display(s8 Mode)
{
	OLED_Display(0, 6, 11, 16, 0);		//确
	OLED_Display(16, 6, 12, 16, 0);		//定
	OLED_Display(96, 6, 13, 16, 0);		//取
	OLED_Display(112, 6, 14, 16, 0);	//消
	switch(Mode)
	{
		case 0:
			OLED_Display(48, 2, 7, 16, 0);
			OLED_Display(64, 2, 8, 16, 0);
			OLED_Display(48, 4, 9, 16, 0);
			OLED_Display(64, 4, 10, 16, 0);
		case 1:
			OLED_Display(48, 0, 5, 16, 0);
			OLED_Display(64, 0, 6, 16, 0);
	}
}

/////////////////////////////////////////////////////界面2——模式设定 显示函数

void Interface_2_Mode_Set_Display(void)
{
	OLED_Display(0, 6, 11, 16, 0);			//确
	OLED_Display(16, 6, 12, 16, 0);			//定
	
	OLED_Display(96, 6, 13, 16, 0);			//取
	OLED_Display(112, 6, 14, 16, 0);		//消
	
	OLED_Display(32, 2, 16, 16, 0);			//强
	OLED_Display(48, 2, 17, 16, 0);			//制
	OLED_Display(64, 2, 5, 16, 0);			//模
	OLED_Display(80, 2, 6, 16, 0);			//式
	
	OLED_Display(32, 0, 18, 16, 0);			//自
	OLED_Display(48, 0, 19, 16, 0);			//动
	OLED_Display(64, 0, 5, 16, 0);			//模
	OLED_Display(80, 0, 6, 16, 0);			//式
}
	
void Interface_2_Time_Set_Display(void)
{
	OLED_Display(0, 6, 11, 16, 0);			//确
	OLED_Display(16, 6, 12, 16, 0);			//定
	
	OLED_Display(96, 6, 13, 16, 0);			//取
	OLED_Display(112, 6, 14, 16, 0);		//消
}

void Interface_0_Display(void)
{
	OLED_Display(0, 6, 20, 16, 0);			//设
	OLED_Display(16, 6, 21, 16, 0);			//定
	
	if(Mode == 0)
	{
		OLED_Display(96, 6, 18, 16, 0);		//自
		OLED_Display(112, 6, 19, 16, 0);	//动
	}
	else
	{
		OLED_Display(96, 6, 16, 16, 0);		//强
		OLED_Display(112, 6, 17, 16, 0);	//制
	}
}

void Interface_2_Alarm_Set_Display(void)
{
	OLED_Display(16 * 3, 2, 22, 16, 0);		//开
	OLED_Display(16 * 4, 2, 23, 16, 0);		//灯
	
	OLED_Display(16 * 3, 0, 24, 16, 0);		//关
	OLED_Display(16 * 4, 0, 23, 16, 0);		//灯
	
	
	OLED_Display(0, 6, 11, 16, 0);			//确
	OLED_Display(16, 6, 12, 16, 0);			//定
	
	OLED_Display(96, 6, 13, 16, 0);			//取
	OLED_Display(112, 6, 14, 16, 0);		//消
}

void Interface_2_Alarm_Set_ON_OFF_Display(Alarm_InitTypeDef* Alarm_InitTypeDefStructer, u8 i)
{
	OLED_Display(32, 2, ((Alarm_InitTypeDefStructer -> hour)/10)%10, 8, 0);
	OLED_Display(40, 2, (Alarm_InitTypeDefStructer -> hour)%10, 8, 0);
	OLED_Display(48, 2, 65, 8, 0);
	OLED_Display(56, 2, ((Alarm_InitTypeDefStructer -> minute)/10)%10, 8, 0);
	OLED_Display(64, 2, (Alarm_InitTypeDefStructer -> minute)%10, 8, 0);
	OLED_Display(72, 2, 65, 8, 0);
	OLED_Display(80, 2, ((Alarm_InitTypeDefStructer -> second)/10)%10, 8, 0);
	OLED_Display(88, 2, (Alarm_InitTypeDefStructer -> second)%10, 8, 0);
	switch(i)
	{
		case 1:
			OLED_Display(32, 2, ((Alarm_InitTypeDefStructer -> hour)/10)%10, 8, 1);
			break;
		case 2:
			OLED_Display(40, 2, (Alarm_InitTypeDefStructer -> hour)%10, 8, 1);
			break;
		case 3:
			OLED_Display(56, 2, ((Alarm_InitTypeDefStructer -> minute)/10)%10, 8, 1);
			break;
		case 4:
			OLED_Display(64, 2, (Alarm_InitTypeDefStructer -> minute)%10, 8, 1);
			break;
		case 5:
			OLED_Display(80, 2, ((Alarm_InitTypeDefStructer -> second)/10)%10, 8, 1);
			break;
		case 6:
			OLED_Display(88, 2, (Alarm_InitTypeDefStructer -> second)%10, 8, 1);
			break;
	}
}

void ON_OFF_Display(u8 i)
{
	switch(i)
	{
		case 0:
			OLED_Display(16 * 3, 4, 24, 16, 0);		//关
			OLED_Display(16 * 4, 4, 23, 16, 0);		//灯
			break;
		case 1:
			OLED_Display(16 * 3, 4, 22, 16, 0);		//开
			OLED_Display(16 * 4, 4, 23, 16, 0);		//灯
			break;
	}
}

void Interface_2_Alarm_Set(void)
{
	OLED_Display(0, 6, 11, 16, 0);			//确
	OLED_Display(16, 6, 12, 16, 0);			//定
	
	OLED_Display(96, 6, 13, 16, 0);			//取
	OLED_Display(112, 6, 14, 16, 0);		//消
}
