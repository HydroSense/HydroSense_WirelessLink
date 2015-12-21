/* ###################################################################
**     Filename    : rtos_main_task.c
**     Project     : Test_FreeRTOS
**     Processor   : MKL26Z64VLH4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-12-20, 15:16, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         main_task - void main_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file rtos_main_task.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup rtos_main_task_module rtos_main_task module documentation
**  @{
*/         
/* MODULE rtos_main_task */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

#include "rfsettings.h"
#include "components/common/hal_defs.h"
#include "components/devices/cc112x/cc112x_spi.h"
#include "components/targets/teensy/hal_spi_rf_trxeb.h"

union cc1120_status {
	struct {
	  uint8_t reserved : 4;
	  uint8_t STATE : 3;
	  uint8_t CHIP_RDY : 1;
	} fields;
	uint8 data;
};

/* Initialization of Processor Expert components function prototype */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
extern void PEX_components_init(void);
#endif 

/*
** ===================================================================
**     Callback    : main_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void main_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
  
  /* Initialization of Processor Expert components (when some RTOS is active). DON'T REMOVE THIS CODE!!! */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
  PEX_components_init(); 
#endif 
  /* End of Processor Expert components initialization.  */

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */

	  /*
	   * cc1120 test code.
	   * used a TeensyLC on SPI0 with
	   *   pin 10 = SPI chip select
	   *   pin 7  = cc1120 reset pin
	   * Copyright 2015 Alan Marchiori
	   */
	  configureRadio();

	  uint8_t i;
	  union cc1120_status status;
	  for(;;) {
	    //Serial.print("Loop: ");
	    //Serial.print(i);
	    //Serial.print("\n");

	    // put the radio into frequency on state
	    status.data = trxSpiCmdStrobe(CC112X_SNOP);
	    //Serial.print("Top of loop state: ");
	    //Serial.print(status.fields.STATE);
	    //Serial.print("Chip ready: ");
	    //Serial.print(status.fields.CHIP_RDY);
	    //Serial.print("\n");

	    // strobe to on
	    //Serial.print("After SFSTXON: ");
	    status.data = trxSpiCmdStrobe(CC112X_STX);
	    //Serial.print(status.fields.STATE);
	    //Serial.print("\n");
	    // put the radio into frequency on state
	    status.data = trxSpiCmdStrobe(CC112X_SNOP);
	    //Serial.print("---: ");
	    //Serial.print(status.fields.STATE);
	    //Serial.print("\n");

	    // load up FIFO
	    char dataToSend[] = "\05hello";
	    cc112xSpiWriteTxFifo((uint8*)dataToSend, 6);

	    // send an STX
	    status.data = trxSpiCmdStrobe(CC112X_STX);
	    //Serial.print("After STX: ");
	    //Serial.print(status.fields.STATE);
	    //Serial.print("\n");

	    // flush the transmit strobe if there is an error
	    if (status.fields.STATE == 7) {
	      //Serial.print("ERROR: Transmit FIFO error\n");
	      trxSpiCmdStrobe(CC112X_SFTX);
	    }

	    i++;
	    OSA_TimeDelay(500);
	  }
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END rtos_main_task */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
