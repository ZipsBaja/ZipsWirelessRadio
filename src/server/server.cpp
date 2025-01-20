#define USING_PRINT 1

#include <ZipsLib.h>
#include <ZipsWireless.h>

int main(int argc, char* argv[])
{
	init_libs();
	
	BEGIN_SETUP();

	cyw43_arch_enable_ap_mode(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK);


	BEGIN_LOOP();
	while (1)
	{
		cyw43_arch_poll();
		LOG("loop");
	}

}