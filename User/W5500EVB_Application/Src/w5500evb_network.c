#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "w5500evb_w5500.h"
#include "w5500evb_mqtt.h"
#include "w5500evb_network.h"

#define W5500EVB_NETWORK_TASK_PRIO		0
#define W5500EVB_NETWORK_STK_SIZE		128

TaskHandle_t W5500EVB_Network_Task_Handler;

//void test(void)
//{
//	uint8_t server_ip[4] = { 192,168,1,104 };
//	uint16_t server_port = 5000;
//
//	uint8_t buffer[64];
//
//	uint16_t len = 0;
//
//	/***** 打开W5500的Keepalive功能 *****/
//	setKPALVTR(0, 0x02);
//
//	printf("TCP Server IP: %d.%d.%d.%d \r\n", server_ip[0], server_ip[1], server_ip[2], server_ip[3]);
//	printf("TCP Server Port: %d \r\n", server_port);
//	printf("W5500 Init Complete!\r\n");
//	printf("Start TCP Client Test!\r\n");
//	printf("phy status is %2X\r\n", getPHYStatus());
//
//	/*Socket状态机，MCU通过读Sn_SR(0)的值进行判断Socket应该处于何种状态
//		Sn_SR状态描述：
//		0x00		SOCK_CLOSED
//		0x13		SOCK_INIT
//		0x14		SOCK_LISTEN
//		0x17		SOCK_ESTABLISHED
//		0x1C		SOCK_CLOSE_WAIT
//		0x22		SOCK_UDP
//	*/
//	while (1)																				// Socket状态机
//	{
//		switch (getSn_SR(0))														// 获取socket0的状态
//		{
//		case SOCK_INIT:															// Socket处于初始化完成(打开)状态
//			connect(0, server_ip, server_port);			// 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
//			break;
//		case SOCK_ESTABLISHED:											// Socket处于连接建立状态
//			if (getSn_IR(0) & Sn_IR_CON)
//			{
//				setSn_IR(0, Sn_IR_CON);								// Sn_IR的CON位置1，通知W5500连接已建立
//			}
//			// 数据回环测试程序：数据从上位机服务器发给W5500，W5500接收到数据后再回给服务器
//			len = getSn_RX_RSR(0);										// len=Socket0接收缓存中已接收和保存的数据大小
//			if (len > 0)
//			{
//				recv(0, buffer, len);										// W5500接收来自服务器的数据，并通过SPI发送给MCU
//				printf("%s\r\n", buffer);							// 串口打印接收到的数据
//				send(0, buffer, len);										// 接收到数据后再回给服务器，完成数据回环
//			}
//			break;
//		case SOCK_CLOSE_WAIT:												// Socket处于等待关闭状态
//			close(0);																// 关闭Socket0
//			break;
//		case SOCK_CLOSED:														// Socket处于关闭状态
//			socket(0, Sn_MR_TCP, 6000, Sn_MR_ND);		// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
//			break;
//		}
//	}
//}

void W5500EVB_Network_Init(void)
{
	uint8_t MAC[6] = { 0x00,0x08,0xDC,0xCB,0x5C,0x86 };
	uint8_t IP[4] = { 192,168,1,150 };
	uint8_t SUBNET[4] = { 255,255,255,0 };
	uint8_t GATEWAY[4] = { 192,168,1 ,1 };
	/*uint8_t DNS[4] = { 8,8,8,8 };*/

	uint8_t TX_Buffer_Size[MAX_SOCK_NUM] = { 2,2,2,2,2,2,2,2 };
	uint8_t RX_Buffer_Size[MAX_SOCK_NUM] = { 2,2,2,2,2,2,2,2 };

	uint8_t Temp[4];

	setSHAR(MAC);
	setSIPR(IP);
	setSUBR(SUBNET);
	setGAR(GATEWAY);

	sysinit(TX_Buffer_Size, RX_Buffer_Size);

	setRTR(2000);																			// 设置超时时间
	setRCR(3);

	printf("Init Network\r\n");

	getSIPR(Temp);
	printf("IP : \t\t%d.%d.%d.%d\r\n", Temp[0], Temp[1], Temp[2], Temp[3]);

	getSUBR(Temp);
	printf("SUBNET : \t%d.%d.%d.%d\r\n", Temp[0], Temp[1], Temp[2], Temp[3]);

	getGAR(Temp);
	printf("GATEWAY : \t%d.%d.%d.%d\r\n", Temp[0], Temp[1], Temp[2], Temp[3]);
}

void W5500EVB_Network_Task(void* pvParameters)
{
	W5500_Reset();

	W5500EVB_Network_Init();

	printf("Enter MQTT Task\r\n");

	taskENTER_CRITICAL();

	W5500EVB_MQTT_Task_Creat();

	vTaskDelete(NULL);

	taskEXIT_CRITICAL();
}

void W5500EVB_Network_Task_Creat(void)
{
	xTaskCreate(
		(TaskFunction_t)W5500EVB_Network_Task,
		(const char*)"Network",
		(uint16_t)W5500EVB_NETWORK_STK_SIZE,
		(void*)NULL,
		(UBaseType_t)W5500EVB_NETWORK_TASK_PRIO,
		(TaskHandle_t*)&W5500EVB_Network_Task_Handler
	);
}
