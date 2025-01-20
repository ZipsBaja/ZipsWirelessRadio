#define USING_PRINT 1
#define RETRY_CONNECTION 1

#include <ZipsLib.h>
#include <ZipsWireless.h>
#include <WiFiClient.h>

int main(int argc, char* argv[])
{
	init_libs();

	BEGIN_SETUP();

	cyw43_arch_enable_sta_mode();

	uazips::WiFiClient client;
	uazips::OrderedList<const cyw43_ev_scan_result_t*> list = client.ScanNetworks(10);
	while (1)
	{
		for (const cyw43_ev_scan_result_t* res : list)
		{
			LOG("%s", res->ssid);
		}
		sleep_ms(1000);
	}

	bool connected = false;
#if RETRY_CONNECTION
	while (!connected)
	{
#endif
		if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, TIMEOUT_MS))
		{
#if RETRY_CONNECTION
			WARN("Connection timed out. Retrying...");
#else
			THROW("Could not connect to Wi-Fi point.");
#endif
		} else
			connected = true;
#if RETRY_CONNECTION
	}
#endif

	BEGIN_LOOP();
	while (1)
	{
		//cyw43_arch_poll();
		THROW("in loop");
		
	}

}