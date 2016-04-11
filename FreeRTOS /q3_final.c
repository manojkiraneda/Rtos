/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"

/* Standard include. */
#include <stdio.h>
#include <stdlib.h>

//extern tskTCB;
/* The ITM port is used to direct the printf() output to the serial window in 
the Keil simulator IDE. */
#define mainITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define mainITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define mainDEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define mainTRCENA          0x01000000

/*-----------------------------------------------------------*/
int fputc( int iChar, FILE *pxNotUsed );


/*-----------------------------------------------------------*/

int fputc( int iChar, FILE *pxNotUsed ) 
{
	/* Just to avoid compiler warnings. */
	( void ) pxNotUsed;

	if( mainDEMCR & mainTRCENA ) 
	{
		while( mainITM_Port32( 0 ) == 0 );
		mainITM_Port8( 0 ) = iChar;
  	}

  	return( iChar );
}

/*This is for showing tasks running in logic analyzer*/

unsigned long ulTaskNumber[ configEXPECTED_NO_RUNNING_TASKS ];

static xQueueHandle xQueue;

/*defining a structure that will be passed to the queue*/
typedef struct {
	int val;
	int src;
}xData;
xTaskHandle t1,t2,t3;

xData sendData[3] = {{10,1},{20,2},{30,3}};

void vTaskGet(xQueueHandle pxQueue);

static void SenderTask1(void *pvParameters)
{
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 200/portTICK_RATE_MS;
	for(;;)
	{
		xStatus = xQueueSend(xQueue, &sendData[0], xTicksToWait);
		if(xStatus != pdPASS)
		printf("Could not send to the queue.\r\n");
		taskYIELD();
	}
}	

static void SenderTask2(void *pvParameters)
{
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 200/portTICK_RATE_MS;
	for(;;)
	{
		xStatus = xQueueSend(xQueue, &sendData[1], xTicksToWait);
		if(xStatus != pdPASS)
		printf("Could not send to the queue.\r\n");
		taskYIELD();
	}
}	
static void SenderTask3(void *pvParameters)
{
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 200/portTICK_RATE_MS;
	for(;;)
	{
		
		
		xStatus = xQueueSend(xQueue, &sendData[2], xTicksToWait);
		if(xStatus != pdPASS)
		printf("Could not send to the queue.\r\n");
		taskYIELD();
	}
}	

static void ReceiverTask(void *pvParameters)
{
	xData Received_struct;
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 200 / portTICK_RATE_MS;
	for(;;)
	{
		vTaskGet(xQueue);
		if(uxQueueMessagesWaiting(xQueue) !=3)
		printf("Queue is not full.!\r\n");
		xStatus = xQueueReceive(xQueue, &Received_struct, xTicksToWait);
		printf("Message Length : %d\n",sizeof(Received_struct));
		if(xStatus == pdPASS)
		{
			printf("From Sender %d = %d\n",Received_struct.src,Received_struct.val);
		}
		else
			printf("Could not receive from the queue\r\n");
	}
}

int main(void)
{
	xQueue = xQueueCreate(3, sizeof(xData));
	if(xQueue != NULL)
	{
	
		xTaskCreate(SenderTask1, (signed char*)"Sender1" , configMINIMAL_STACK_SIZE, 0,2,&t1);
		xTaskCreate(SenderTask2, (signed char*)"Sender2" , configMINIMAL_STACK_SIZE, 0,2,&t2);
		xTaskCreate(SenderTask3, (signed char*)"Sender3" , configMINIMAL_STACK_SIZE, 0,2,&t3);
		xTaskCreate(ReceiverTask, (signed char*)"Receiver" , configMINIMAL_STACK_SIZE, NULL,1,NULL);
		vTaskSetTaskNumber( t1, 1 );/*This function will set the Task Number to a Task */
		vTaskSetTaskNumber( t2, 2 );/*This function will set the Task Number to a Task */
		vTaskSetTaskNumber( t3, 3 );/*This function will set the Task Number to a Task */
		vTaskStartScheduler();
		for(;;);
	}
		
}

	void vTaskGet(xQueueHandle pxQueue)
	{
		xList xlist = getList(pxQueue);
		printf("Number of items in the Queue : %ld\n",xlist.uxNumberOfItems);
		printTaskNames(xlist.pxIndex);
		vTaskGetWaitingToSend(&xlist);
			
	}
