#include "fifo.h"

/*
FIFO
*/
typedef struct
{
	unsigned char buff[FIFO_MAX_SIZE];            //fifo buffer                         
	int fifo_r_index;                             //read index, first increase, then read, init 0                  
	int fifo_w_index;                             //write index, first write, then increase, init 1   
	int size;
} FIFO;


static FIFO fifo;



/*******************************************************************************
* Function Name  : int fifo_init(void)
* Description    : initialization fifo
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fifo_init(void)
{
	fifo.fifo_r_index = 0;
	fifo.fifo_w_index = 1;
	fifo.size = FIFO_MAX_SIZE - 1;
	return 0;
}
/*******************************************************************************
* Function Name  : int fifo_in(unsigned char m)
* Description    : write fifo
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fifo_in(unsigned char m)
{
	if(fifo.fifo_w_index != fifo.fifo_r_index)              
	{
	  fifo.buff[fifo.fifo_w_index] = m;     

		fifo.size = fifo.size - 1;
		
		if(fifo.fifo_w_index >= (FIFO_MAX_SIZE - 1))
		  fifo.fifo_w_index = 0;
		else
		  fifo.fifo_w_index++;
		
		return 0;
	}
	else    return -1;
}
/*******************************************************************************
* Function Name  : int fifo_out(unsigned char * ptr)
* Description    : read fifo
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fifo_out(unsigned char * ptr)
{
	int i;
	
	i = fifo.fifo_r_index;                                                
	
	if(i >= (FIFO_MAX_SIZE - 1))  i = 0;                                                     
	else                          i++;
	

	if(i != fifo.fifo_w_index)                                          
	{
		*ptr = fifo.buff[i];
		fifo.fifo_r_index = i;
		fifo.size = fifo.size + 1;
		return 0;
	}
	else    return -1;
}
/*******************************************************************************
* Function Name  : int fifo_size(void)
* Description    : get current buffered size
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fifo_size(void)
{
	if(fifo.fifo_r_index < fifo.fifo_w_index)
	{
		return (fifo.fifo_w_index - fifo.fifo_r_index - 1);
	}
	else
	{
		return(fifo.fifo_w_index + ((FIFO_MAX_SIZE - 1)) - fifo.fifo_r_index);
	}
}
/*******************************************************************************
* Function Name  : int fifo_freesize(void)
* Description    : get current free fifo size
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fifo_freesize(void)
{
  return ((FIFO_MAX_SIZE - 1) - fifo_size());
}
