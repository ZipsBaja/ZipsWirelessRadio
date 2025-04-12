#include <Options.h>

#include <ZipsLib.h>
#include <ZipsWireless.h>
#include <WiFiServer.h>

#define SIGNAL_PIN 4

volatile bool radio_connected = false;

int main(int argc, char* argv[])
{
	init_libs();
	
	gpio_init(SIGNAL_PIN);
	gpio_set_dir(SIGNAL_PIN, GPIO_OUT);

	BEGIN_SETUP();

	uazips::WiFiServer server(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK);

	server.AwaitUDP([](void* arg, udp_pcb* pcb, pbuf* p, const ip_addr_t* addr, uint16_t port)
		{
			uint32_t status = (uint32_t) pbuf_get_at(p, 0);
			if (status == WIFI_PAYLOAD_HIGH)
			{
				gpio_put(SIGNAL_PIN, true);
#if USING_PRINT
				if (!radio_connected)
					LOG("Radio signal caught, radio connected.\n");
#endif
				radio_connected = true;
			}
			else
			{
				gpio_put(SIGNAL_PIN, false);
#if USING_PRINT
				if (radio_connected)
					LOG("Radio signal lost, radio disconnected.\n");
#endif
				radio_connected = false;
			}
			char ipbuf[16];
			ipaddr_ntoa_r(addr, ipbuf, sizeof(ipbuf));
			LOG("Packet received. Payload: %i\t on port: %i\t from IP: %s\n", status, port, ipbuf);
			pbuf_free(p);
		},
	WIFI_PORT, &server);
	
	BEGIN_LOOP();
	while (1)
	{
		server.Update();
	}

}