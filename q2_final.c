/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Standard include. */
#include <stdio.h>

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue, specified in milliseconds. */
#define mainQUEUE_SEND_FREQUENCY_MS         ( 10 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH                    ( 1 )

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
int fputc( int iChar, FILE *pxNotUsed );

/*-----------------------------------------------------------*/

/* One array position is used for each task created by this demo.  The 
variables in this array are set and cleared by the trace macros within
FreeRTOS, and displayed on the logic analyzer window within the Keil IDE -
the result of which being that the logic analyzer shows which task is
running when. */
unsigned long ulTaskNumber[ configEXPECTED_NO_RUNNING_TASKS ];
/*Creating the Tasks t1,t2,t3,t4,t5*/
xTaskHandle t1,t2,t3,t4,t5;

/*-----------------------------------------------------------*/
/* vTask1 is Created */
void vTask1(void *pvParameters)
{
    for(;;)/*Running task in an infinite Loop*/
    {
        printf("%lu\n",uxTaskGetTaskNumber(t1));
        vTaskDelay(1000/portTICK_RATE_MS);
        /*vTaskDelay() is used to send the task into waiting state for a given
            amount of ticks*/
    }
            
}
/* vTask2 is Created */
void vTask2(void *pvParameters)
{
    for(;;)/*Running task in an infinite Loop*/
    {
        printf("%lu\n",uxTaskGetTaskNumber(t2));
        vTaskDelay(2000/portTICK_RATE_MS);
        /*vTaskDelay() is used to send the task into waiting state for a given
          amount of ticks*/
    }
}
/* vTask3 is Created */
void vTask3(void *pvParameters)
{
    for(;;)/*Running task in an infinite Loop*/
    {
        printf("%lu\n",uxTaskGetTaskNumber(t3));
        vTaskDelay(3000/portTICK_RATE_MS);
        /*vTaskDelay() is used to send the task into waiting state for a given
          amount of ticks*/
    }
}
/* vTask4 is Created */
void vTask4(void *pvParameters)
{
    for(;;)/*Running task in an infinite Loop*/
    {
        printf("%lu\n",uxTaskGetTaskNumber(t4));
        vTaskDelay(4000/portTICK_RATE_MS);
        /*vTaskDelay() is used to send the task into waiting state for a given
          amount of ticks*/
    }
}
/* vTask5 is Created */
void vTask5(void *pvParameters)
{
    for(;;)
    {
        printf("%lu\n",uxTaskGetTaskNumber(t5));
        vTaskDelay(5000/portTICK_RATE_MS);
        /*vTaskDelay() is used to send the task into waiting state for a given
          amount of ticks*/

    }
}


int main(void)
{
    /*xTaskCreate is Used to Create the Task*/
    xTaskCreate (vTask1,(const signed char *)"Task 1",100,NULL,1,&t1);
    xTaskCreate (vTask2,(const signed char *)"Task 2",100,NULL,2,&t2);
    xTaskCreate (vTask3,(const signed char *)"Task 3",100,NULL,3,&t3);
    xTaskCreate (vTask4,(const signed char *)"Task 2",100,NULL,4,&t4);
    xTaskCreate (vTask5,(const signed char *)"Task 3",100,NULL,5,&t5);
  /*vTaskSetTaskNumber is Used to set the tasknumber to a Task*/
    vTaskSetTaskNumber( t1, 1 );
    vTaskSetTaskNumber( t2, 2 );
    vTaskSetTaskNumber( t3, 3 );
    vTaskSetTaskNumber( t4, 4 );
    vTaskSetTaskNumber( t5, 5 );
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
