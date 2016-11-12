#include "LCD.H"
//#include "workingLCD.h"

void LCD_Init()
{
	int i;
    mywriteCmd(0xCB);
    mywriteDat(0x39);
    mywriteDat(0x2C);
    mywriteDat(0x00);
    mywriteDat(0x34);
    mywriteDat(0x02);

    mywriteCmd(0xCF);
    mywriteDat(0x00);
    mywriteDat(0XC1);
    mywriteDat(0X30);

    mywriteCmd(0xE8);
    mywriteDat(0x85);
    mywriteDat(0x00);
    mywriteDat(0x78);

    mywriteCmd(0xEA);
    mywriteDat(0x00);
    mywriteDat(0x00);

    mywriteCmd(0xED);
    mywriteDat(0x64);
    mywriteDat(0x03);
    mywriteDat(0X12);
    mywriteDat(0X81);

    mywriteCmd(0xF7);
    mywriteDat(0x20);

    mywriteCmd(0xC0);    //Power control
    mywriteDat(0x23);   //VRH[5:0]

    mywriteCmd(0xC1);    //Power control
    mywriteDat(0x10);   //SAP[2:0];BT[3:0]

    mywriteCmd(0xC5);    //VCM control
    mywriteDat(0x3e); //¶Ô±È¶Èµ÷½Ú
    mywriteDat(0x28);

    mywriteCmd(0xC7);    //VCM control2
    mywriteDat(0x86);  //--

    mywriteCmd(0x36);    // Memory Access Control
    mywriteDat(0x48); //C8       //48 68ÊúÆÁ//28 E8 ºáÆÁ

    mywriteCmd(0x3A);
    mywriteDat(0x55);

    mywriteCmd(0xB1);
    mywriteDat(0x00);
    mywriteDat(0x18);

    mywriteCmd(0xB6);    // Display Function Control
    mywriteDat(0x08);
    mywriteDat(0x82);
    mywriteDat(0x27);

    mywriteCmd(0xF2);    // 3Gamma Function Disable
    mywriteDat(0x00);

    mywriteCmd(0x26);    //Gamma curve selected
    mywriteDat(0x01);

    mywriteCmd(0xE0);    //Set Gamma
    mywriteDat(0x0F);
    mywriteDat(0x31);
    mywriteDat(0x2B);
    mywriteDat(0x0C);
    mywriteDat(0x0E);
    mywriteDat(0x08);
    mywriteDat(0x4E);
    mywriteDat(0xF1);
    mywriteDat(0x37);
    mywriteDat(0x07);
    mywriteDat(0x10);
    mywriteDat(0x03);
    mywriteDat(0x0E);
    mywriteDat(0x09);
    mywriteDat(0x00);

    mywriteCmd(0XE1);    //Set Gamma
    mywriteDat(0x00);
    mywriteDat(0x0E);
    mywriteDat(0x14);
    mywriteDat(0x03);
    mywriteDat(0x11);
    mywriteDat(0x07);
    mywriteDat(0x31);
    mywriteDat(0xC1);
    mywriteDat(0x48);
    mywriteDat(0x08);
    mywriteDat(0x0F);
    mywriteDat(0x0C);
    mywriteDat(0x31);
    mywriteDat(0x36);
    mywriteDat(0x0F);

    mywriteCmd(0x11);    //Exit Sleep
		for( i = 0; i < 20000; i++) { i++;}

    mywriteCmd(0x29);    //Display on
    //mywriteCmd(0x2c);




}
