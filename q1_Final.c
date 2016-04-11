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
/*Creating the Tasks t1,t2,t3,t4*/
xTaskHandle t1,t2,t3,tp;
/*Creating the Queues q1,q2,q3,qp*/
xQueueHandle q1,q2,q3,qp;
/*Count is Used to iterate the loop from 1 to 3*/
signed int count = 0;
/*Temp is Used to count the Number of times the Task tp is Wake up*/

signed int temp = 0;
/*Defining a Strucutre to store the task_number as well as the Value*/
typedef struct
{
	int task_number;
	int value;
}xData;


/*-----------------------------------------------------------*/
/*vTask1 definition*/
void vTask1(void *pvParameters)
{
	xData str_task1;
	int i,final_value;
	portBASE_TYPE xStatus;
	signed int value;
	for(;;)/*Running the Task as Infinite Loop*/
	{
		if ((uxQueueMessagesWaiting(q1))!=0) /*This function will give > 1 based on the messages on the Queue*/ 
		{
			xStatus = xQueueReceive(q1,&value,0);
			/*xQueueReceive is used to receive a message from the Queue*/
			if(xStatus == pdPASS)/*If Something is received then executuon goes into this loop*/
			{
				i = uxTaskGetTaskNumber(t1);
				/*This function is used to get the Task Number of a Task*/
				str_task1.task_number = i;
				//printf("Task 1 : %d\n",value);
				final_value = i*value;
				str_task1.value = final_value;
				xQueueSendToBack(qp,&str_task1,0);
				/*xQueueSendToBack is used to send a message to end of Queue*/
			}
    }
		else
		{
			vTaskDelay(10);
			/*vTaskDelay() is used to send the task into waiting state for a given
			amount of ticks*/
		}
		taskYIELD();
	}
}
/*vTask2 definition*/
void vTask2(void *pvParameters)
{   xData str_task2;
	int i,final_value;
	portBASE_TYPE xStatus;
	signed int value;
	for(;;)/*Running the Task as Infinite Loop*/
	{
	if ((uxQueueMessagesWaiting(q2))!=0) /*This function will give > 1 based on the messages on the Queue*/
		{
			xStatus = xQueueReceive(q2,&value,0);
			/*xQueueReceive is used to receive a message from the Queue*/
			if(xStatus == pdPASS)/*If Something is received then executuon goes into this loop*/
			{
				i = uxTaskGetTaskNumber(t2);/*This function is used to get the Task Number of a Task*/
				//printf("Task 2 : %d\n",value);
				final_value = i*value;
				str_task2.task_number = i;
				str_task2.value = final_value;
				xQueueSendToBack(qp,&str_task2,0);
				/*xQueueSendToBack is used to send a message to end of Queue*/
			}
    }
		else
		{
			vTaskDelay(10);
			/*vTaskDelay() is used to send the task into waiting state for a given
			amount of ticks*/
		}
		taskYIELD();
	}
}
/*vTask3 definition*/
void vTask3(void *pvParameters)
{
	xData str_task3;
	int i,final_value;
	portBASE_TYPE xStatus;
	signed int value;
	for(;;)/*Running the Task as Infinite Loop*/
	{
	if ((uxQueueMessagesWaiting(q3))!=0) /*This function will give > 1 based on the messages on the Queue*/
		{
			xStatus = xQueueReceive(q3,&value,0);
			/*xQueueReceive is used to receive a message from the Queue*/
			if(xStatus == pdPASS)/*If Something is received then executuon goes into this loop*/
			{
				i = uxTaskGetTaskNumber(t3);/*This function is used to get the Task Number of a Task*/
				//printf("Task 3 : %d\n",value);
				final_value = i*value;
				str_task3.task_number = i;
				str_task3.value = final_value;
				xQueueSendToBack(qp,&str_task3,0);
				/*xQueueSendToBack is used to send a message to end of Queue*/
			}
    }
		else
		{
			vTaskDelay(10);
			/*vTaskDelay() is used to send the task into waiting state for a given
			amount of ticks*/
		}
		taskYIELD();
	}
}
/*vTasktp definition*/
void vTasktp(void *pvParameters)
	{	
		xData rx_str_task;
		int display_value;
		portBASE_TYPE xStatus;
		//const char *pcTaskName = "Task 4 is Running\r\n";
	for(;;)/*Running the Task as Infinite Loop*/
	{
		count = count + 1;
		temp = temp +1;
	  
		//printf("Periodic task");
		if (count == 1)
		{
			xQueueSendToBack(q1,&temp,0);/*xQueueSendToBack is used to send a message to end of Queue*/
		}
		else if (count == 2)
		{
			xQueueSendToBack(q2,&temp,0);/*xQueueSendToBack is used to send a message to end of Queue*/
		}
		else if(count == 3)
		{
			xQueueSendToBack(q3,&temp,0);/*xQueueSendToBack is used to send a message to end of Queue*/
		}
		else 
		{
			count = count % 3;
			xQueueSendToBack(q1,&temp,0);/*xQueueSendToBack is used to send a message to end of Queue*/
		}
		xStatus = xQueueReceive(qp,&rx_str_task,0);/*xQueueReceive is used to receive a message from the Queue*/
		if(xStatus == pdPASS)/*If Something is received then executuon goes into this loop*/
		{
			printf("The Task %d has sent the value %d\n",rx_str_task.task_number,rx_str_task.value);
		}
		printf("count = %d \n ",temp);
	
	 vTaskDelay(1000/portTICK_RATE_MS);
	/*vTaskDelay() is used to send the task into waiting state for a given
	amount of ticks*/
	
	}
}

int main(void)
{
	/*xTaskCreate is Used to Create the Task*/
	xTaskCreate(vTask1,(const signed char *)"Task 1",100,NULL,2,&t1);
	xTaskCreate(vTask2,(const signed char *)"Task 2",100,NULL,2,&t2);
	xTaskCreate(vTask3,(const signed char *)"Task 3",100,NULL,2,&t3);
	xTaskCreate(vTasktp,(const signed char *)"Task 4",100,NULL,1,&tp);
	/*vTaskSetTaskNumber is Used to set the tasknumber to a Task*/
	vTaskSetTaskNumber( t1, 1 );
	vTaskSetTaskNumber( t2, 2 );
	vTaskSetTaskNumber( t3, 3 );
	vTaskSetTaskNumber( tp, 4 );
	/*xQueueCreate is used to Create a Queue*/
	q1 = xQueueCreate(5,sizeof(xData));
	q2 = xQueueCreate(5,sizeof(xData));
	q3 = xQueueCreate(5,sizeof(xData));
	qp = xQueueCreate(5,sizeof(xData));
	vTaskStartScheduler();/*Call the Scheduler*/
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
