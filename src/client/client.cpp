#include <Options.h>

#include <ZipsLib.h>
#include <ZipsWireless.h>
#include <WiFiClient.h>

#include <hardware/gpio.h>

#define BUTTON_PIN 16

int main(int argc, char* argv[])
{
	init_libs();

	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);

	BEGIN_SETUP();

	uazips::WiFiClient client;

	uint8_t payload = WIFI_PAYLOAD_LOW;

	client.Connect(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, WIFI_TIMEOUT_MS, [](){ WARN("Failed to connect. Retrying...\n"); }, RETRY_CONNECTION);

	BEGIN_LOOP();
	while (1)
	{
		client.Update();

		// Set payload high/low based on button status.
		payload = gpio_get(BUTTON_PIN) ? WIFI_PAYLOAD_HIGH : WIFI_PAYLOAD_LOW;

		int8_t e = client.SendUDP(&payload, sizeof(payload), WIFI_PORT);
		if (e != ERR_OK)
		{
			WARN("Packet Error: %d.\n", e);
		}
	}

}