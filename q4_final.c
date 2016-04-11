/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Standard include. */
#include <stdio.h>

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue, specified in milliseconds. */
#define mainQUEUE_SEND_FREQUENCY_MS			( 10 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/* The ITM port is used to direct the printf() output to the serial window in 
the Keil simulator IDE. */
#define mainITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define mainITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define mainDEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define mainTRCENA          0x01000000

/*-----------------------------------------------------------*/
/*
 * Redirects the printf() output to the serial window in the Keil simulator
 * IDE.
 */
int fputc(int iChar, FILE *pxNotUsed);

/*-----------------------------------------------------------*/
/* One array position is used for each task created by this demo.  The 
variables in this array are set and cleared by the trace macros within
FreeRTOS, and displayed on the logic analyzer window within the Keil IDE -
the result of which being that the logic analyzer shows which task is
running when. */
unsigned long ulTaskNumber[ configEXPECTED_NO_RUNNING_TASKS ];
/*Creating the Tasks t1,t2,t3*/
xTaskHandle t1,t2,t3;
/*Creating the Queues q1,q*/
xQueueHandle q1,q;
/*Defining a Strucutre to store the task_number as well as the Value*/
typedef struct
{
	int task_number;
	int value;
}xData;


/*-----------------------------------------------------------*/
/*xMyQueueSend is a wrapper functions to  the QueueSend which  store the task  ID of  the  sender  
also along  with the original message*/

void xMyQueueSend(xQueueHandle q,xData send_str,int Priority)
{
	printf("Sending Data to Queue from task ID : %d\n",send_str.task_number);
	xQueueSendToBack(q1,&send_str,0);
	/*xQueueSendToBack is used to send a message to end of Queue*/
}

/*xMyQueueReceive wrapper functions to  QueueReceive which can receiving tasks can find out the sender of the
message using your implementation of QueueReceive.*/
void xMyQueueReceive(xQueueHandle q,xData receive_str,int Priority)
{
	portBASE_TYPE xStatus;
	xStatus = xQueueReceive(q,&receive_str,0);
	/*xQueueReceive is used to receive a message from the Queue*/
	if(xStatus == pdPASS)/*If Something is received then executuon goes into this loop*/
	{
		printf("Task ID : %d and Value is : %d\n",receive_str.task_number,receive_str.value);
	}
	else
	{
		printf("Queue is Empty \n");
		vTaskDelay(10);
		/*vTaskDelay() is used to send the task into waiting state for a given
		  amount of ticks*/
	}
	taskYIELD();
}
/*vTask1 definition*/
void vTask1(void *pvParameters)
{
	xData str_task1;
	for(;;)
	{
		str_task1.task_number = uxTaskGetTaskNumber(t1);/*Get the Task Number*/
		str_task1.value = (int) pvParameters;
		xMyQueueSend(q1,str_task1,0);/*Calling the Wrapper Function xMyQueueSend*/
		vTaskDelay(10);
		taskYIELD();
	}
}
/*vTask2 definition*/
void vTask2(void *pvParameters)
{ xData str_task2;
	for(;;)
	{
	 xMyQueueReceive(q1,str_task2,0);/*Calling the Wrapper Function xMyQueueSend*/
  }
}
/*vTask2 definition*/
void vTask3(void *pvParameters)
{
	xData str_task3;
	for(;;)
	{
		str_task3.task_number = uxTaskGetTaskNumber(t3);
		str_task3.value = (int) pvParameters;
		xMyQueueSend(q1,str_task3,0);/*Calling the Wrapper Function xMyQueueSend*/
		vTaskDelay(10);
		taskYIELD();
	}
}
	
int main(void)
{
	/*Creating the Tasks t1,t2,t3*/
	xTaskCreate(vTask1,(const signed char *)"Task 1",100,(void *)100,1,&t1);
	xTaskCreate(vTask2,(const signed char *)"Task 2",100,NULL,1,&t2);
	xTaskCreate(vTask3,(const signed char *)"Task 3",100,(void *)200,1,&t3);
	vTaskSetTaskNumber( t1, 1 );/*This function will set the Task Number to a Task */
	vTaskSetTaskNumber( t2, 2 );/*This function will set the Task Number to a Task */
	vTaskSetTaskNumber( t3, 3 );/*This function will set the Task Number to a Task */
	q1 = xQueueCreate(5,sizeof(xData));
	vTaskStartScheduler();
	for(;;);
	}

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
