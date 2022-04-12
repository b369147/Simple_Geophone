# How to Use This Module

# 1#

1. enable NVIC of USART, make sure it has most Priority, in case of interrupt during transmitting

![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdown20220404143327.png)

2. enable IRQ handler and disable Call HAL handle

![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdownimage-20211122161011393.png)

3. add command.h into the include

![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdown20220409172746.png)

4. copy Command_Get(); to USART_IRQHandler(), remember to add header file

![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdownimage-20211122161157911.png)

# 2#

1. add header file to main.c

2. add private variable

   ![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdownimage-20211122161515676.png)

3. add init func

   ![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdownimage-20211122161546590.png)

4. add Excute function to while

   ![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdownimage-20211122161623481.png)

# Reference

```c
// 
// Command_Get();
// 全局变量
extern uint8_t Command_Index;
extern uint8_t Command_List[10][128];
// 初始化部分
Command_Init();
// while部分
if (Command_Index != 0)
{
	Command_Excute(&Command_List[0][0]);
	Command_Index = 0;
}
HAL_Delay(10);
```



而我们在进行替换的时候，由于指定的为weak，因此可以直接在main.c里面指定：

```c
void Func_div(uint8_t* command)
{
	uint8_t value = strtol(command + 20, '\0', 10);
	
	if (strcmp(command + 10, "_Amp") == 0) 
	{
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, value);
		HAL_Delay(1000);
	}
	else if (strcmp(command + 10, "_BIAOS") == 0)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, value);
		HAL_Delay(1000);
	}
	else if (strcmp(command + 10, "_Status") == 0)
	{
		HAL_Delay(1000);
	}
	else
	{
		printf("Argument incorrect\r\n");
	}
}
```







输入的应该如下（注意：一定要有换行！！！）：

```c
test _Amp 109  

```

![](https://wang-1304725667.cos.ap-chengdu.myqcloud.com/markdown20220404194725.png)

