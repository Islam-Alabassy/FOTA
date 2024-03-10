#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"
#include "MCAL/RCC/RCC_int.h"
#include "MCAL/GPIO/GPIO_int.h"
#include "MCAL/GPIO/GPIO_cfg.h"
#include "MCAL/FMI/FMI_int.h"
#include "MCAL/STK/STK_int.h"
#include "MCAL/NVIC/NVIC_interface.h"
#include "APP/HexParser/HexParser_int.h"
#include "APP/Bootoader_services/Bootloader_services.h"
#include "MCAL/UART/UART_interface.h"
#include "MCAL/UART/UART_private.h"
#include "MCAL/UART/UART_configuration.h"



volatile u8 Global_pu8RxBuffer[][50]; // GLOBAL 2D ARRAY FOR CARRING HEX FILE
volatile s16 col=-1,row=0; // DIRECTION OF 2D ARRAY

volatile u32 Recive_Requset=0; // FISRT TIME RECIEVING REQUSET

volatile u32 CheckSecurty=0; // FOR CHECK SECURTY FROM ESP82
volatile u32 Random_Val=0; // CARRING RANDOM VALUE




void USART_RECEIVE_2D(void) // RECIVING FUNCTION (HEX FILE)
{
    col++;
    Global_pu8RxBuffer[row][col]= USART1->DR; // READING DATA OF HEX FILE
}

void USART_RECEIVE_REQUST(void)
{

    Recive_Requset= USART1->DR; // RECEIVING REQUSET FIRST AND CHECK SECURT IN SECOND

    // CHECK THE SECURT VALUE FROM ESP82 FOR ACCESS
    if (Recive_Requset=='5')
    {
    	MGPIO_vSetPinVal(GPIO_PORTA,1,GPIO_HIGH);

        MUSART_voidTransmitByte('k'); // SEND (K) TO ESP82 TO START SENDING RECORDS
        MUSART1_voidSetCallBack(USART_RECEIVE_2D); // SWITCH CALL_BACK FOR RECEIVING RECORDS IN 2D ARRAY

    }

}

void STK_CallBack_App(void) // the func that be used to call the app
{

    MSTK_vStoptimerAnd_Enterrupte(); // DISABLE SYSTEN AND ENTERRUPTE
    MUSARTAnd_Enterrupte_Disable(); // DIASBLE USART AND ENTERRUPTE
    MRCC_vDisableClock(RCC_AHB1LPENR,RCC_EN_FMI); // DISABLE FLASH SYSTEM CLOCK
    MRCC_vDisableClock(RCC_AHB1,GPIO_PORTA); // DISABLE GPIOA CLOCK
    MRCC_vDisableClock(RCC_APB2,RCC_UART_ENABLE); // DISABLE USART CLOCK
    MNVIC_VoidDisablePeripheral(USART_GLOBAL_ENABLE); // DISABLE GLOBAL ENTERRUPTE FOR UASRT

    // IF MARKER_AREA = 0X1111 GO TO APP 1 OR ERROR ACUOR AFTER APP1 CHANGE PATTERN
    if (Marker_Area==0x1111 || Marker_Area==0x5555)
    {

        SCB_VTOR = 0x08008000; // SET VECTOR TABLE TO NEW PLACE OF APP1
        u32 *address = (u32 *)(0x08008004); // ASSIGN ADDRESS OF APP1
        u32 instruction = *address;
        ((void (*)(void))instruction)(); // CALLING APP1
    }

    // IF MARKER_AREA = 0X2222 GO TO APP 2 OR ERROR ACUOR AFTER APP2 CHANGE PATTERN
    else if (Marker_Area==0x2222 || Marker_Area==0xAAAA)
    {

        SCB_VTOR =0x08020000; // SET VECTOR TABLE TO NEW PLACE OF APP2
        u32 *address = (u32 *)(0x08020004); // ASSIGN ADDRESS OF APP2
        u32 instruction = *address;
        ((void (*)(void))instruction)(); // CALLING APP2
    }


}



volatile u8 flage=0;

void f1(void)
{
	flage=1;
}



int main(void)
{


	//FMI_vFlash_MarkerWrite(0x5555);

	MRCC_vInit(); // ENABLE INTERNAL CLOCK AND DIV IT ON 16
    MSTK_vInit(); // USE SYSTICK WITH AHB BUS
    MRCC_vEnableClock(RCC_AHB1,GPIO_PORTA);        // ENABLE GPIOA CLOCK
    MRCC_vEnableClock(RCC_AHB1LPENR,RCC_EN_FMI);  // ENABLE FLASH SYSTEM CLOCK
    MRCC_vEnableClock(RCC_APB2,RCC_UART_ENABLE); // ENABLR USART CLOCK

    MNVIC_VoidEnablePeripheral(USART_GLOBAL_ENABLE); // ENABLE GLOBAL ENTERRUPTE FOR UASRT
    MUSART1_voidSetCallBack(USART_RECEIVE_2D); // SET CALL_BACK FOR USART RECEIVE
    MUSART_voidInit(&UART_Intial); // USART INIT
    MUSARTAnd_Enterrupte_Enable(); // ENEBLA USART AND RECEIVE ENTERRUPTE

    MGPIO_vInit(&TX);    // INIT TX PIN (PIN9)
    MGPIO_vInit(&RX);   // INIT RX PIN (PIN10)
    MGPIO_vInit(&led); // CONFIG PIN A1
	MGPIO_vSetPinVal(GPIO_PORTA,1,GPIO_HIGH);


	MSTK_vSetInterval_single(10000000,f1); // GENTARE 15 SECOND FOR CALL APP

	//Random_Val=CreateVRandom_val(); // GENERATE RANDOM VALUE FOR CHECK
    // MUSART_voidTransmitByte('1'); // SEND RANDOM VALUE TO ESP82
	//CheckSecurty=ApplyVsecurty(Random_Val); // APPLING SECURTY ON RANDOM VALUE







    while(1)
    {



    	if (flage==1)
    	{
            STK_CallBack_App(); // CALL THE APP

    	}
        // WHEN RECEING # TURN ON CHECK SUM IF RIGHT SEND OK FOE NEXT RECORD
        if (Global_pu8RxBuffer[row][col]=='#')
        {
            /*apply check sum*/
         //   if (check sum correct go to next record)

        	if (HexParser_u8VerifyCheckSum(&Global_pu8RxBuffer[row][0]))
        	{
        		Global_pu8RxBuffer[row][col]=14;
                MUSART_voidTransmitString("ok"); // SEND OK FOR NEXT RECORD
        	    col=-1; // RETUEN COL TO INTIAL VALUE
        	    row++; // INCREMENT ROW
        		MSTK_vResetTimer(); // RESET SYSTICK

        	}
        	else
        	{

        		MUSART_voidTransmitString("nok");
        		//STK_CallBack_App();

        	}


        }

        // WHEN Global_pu8RxBuffer[row][8]=='1' THAT MEANS END OF HEX FILE
        if (Global_pu8RxBuffer[row][8]=='1' && Global_pu8RxBuffer[row][9]=='F' && Global_pu8RxBuffer[row][10]=='F')
        {
        	 MUSART_voidTransmitString("ok");
        	 col=-1; // RETUEN COL TO INTIAL VALUE
        	 row=0; // INCREMENT ROW
            // WHEN MARKER = 0XFFFF THAT MEANS NO APP AND FLASHING ON APP 1
            if(Marker_Area==0xFFFF)
            {
            	MGPIO_vSetPinVal(GPIO_PORTA,1,GPIO_LOW);
                FMI_vFlash_MarkerWrite(0x1111); // CHANGE MARKER FOR FLASHING IN APP1
                FMI_vEraseAppArea(2,4); // EARSE SECTOR FROM 2 TO 4 SPECIFIC FOR APP2
                HexParser_vParseData_2D(Global_pu8RxBuffer); // PARSE THE HEX FILE IN RECOMENDETION AREA
                STK_CallBack_App(); // CALL THE APP
            }
            // WHEN MARKER = 0X5555 THAT MEANS REQUSET FROM APP1 AND FLASHING ON APP1
            else if (Marker_Area==0x5555)
            {
            	MGPIO_vSetPinVal(GPIO_PORTA,1,GPIO_LOW);
                FMI_vFlash_MarkerWrite(0x2222); // CHANGE MARKER FOR FLASHING IN APP2
                FMI_vSectorErase(5); // EARSE SECTOR FROM 5 TO 7 SPECIFIC FOR APP2
                HexParser_vParseData_2D(Global_pu8RxBuffer); // PARSE THE HEX FILE IN RECOMENDETION AREA
                STK_CallBack_App(); // CALL THE APP

            }
            // WHEN MARKER = 0XAAAA THAT MEANS REQUSET FROM APP2 AND FLASHING ON APP1
            else if(Marker_Area==0xAAAA)
            {
            	MGPIO_vSetPinVal(GPIO_PORTA,1,GPIO_LOW);
                FMI_vFlash_MarkerWrite(0x1111); // CHANGE MARKER FOR FLASHING IN APP1
                FMI_vEraseAppArea(2,4); // EARSE SECTOR FROM 2 TO 4 SPECIFIC FOR APP2
                HexParser_vParseData_2D(Global_pu8RxBuffer); // PARSE THE HEX FILE IN RECOMENDETION AREA
                STK_CallBack_App(); // CALL THE APP
            }


        }



    }
}




