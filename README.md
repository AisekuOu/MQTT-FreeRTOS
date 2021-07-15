# MQTT-FreeRTOS

This project is a routine for MQTT-FreeRTOS based on W5500EVB.

To test this routine, please modify some parameter:

//-------------------------------------------------------------------------

in "MQTT-FreeRTOS\User\W5500EVB_Application\Src\w5500evb_mqtt.c"

static void prvMQTTEchoTask(void* pvParameters)
{
...

uint8_t address[4] = { 5,196,95,208 };
	if ((rc = NetworkConnect(&network, (char*)address, 1883)) != 0)
		printf("Return code from network connect is %d\n", rc);
    
...

//-------------------------------------------------------------------------

