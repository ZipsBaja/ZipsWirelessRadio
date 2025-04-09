#include <Options.h>

#include <ZipsLib.h>
#include <ZipsWireless.h>
#include <WiFiClient.h>

#include <hardware/gpio.h>

// Millisecond period, frequency is 1 / delay. Needed for using 
#define DELAY_INTERVAL 40

#define BUTTON_PTT_PIN 16
#define BUTTON_TOG_PIN 17

#if USING_EXT_LED
#define EXT_LED_PIN 18
#endif

int main(int argc, char* argv[])
{
	init_libs();

	gpio_init(BUTTON_PTT_PIN);
	gpio_init(BUTTON_TOG_PIN);
	gpio_set_dir(BUTTON_PTT_PIN, GPIO_IN);
	gpio_set_dir(BUTTON_PTT_PIN, GPIO_IN);
	gpio_pull_down(BUTTON_PTT_PIN);
	gpio_pull_down(BUTTON_TOG_PIN);

	BEGIN_SETUP();

	uazips::WiFiClient client;

	uint8_t payload = WIFI_PAYLOAD_LOW;
	bool toggle = false;
	bool tog_pressed = false;
	bool allow = true;

	client.Connect(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, WIFI_TIMEOUT_MS, [](){ WARN("Failed to connect. Retrying...\n"); }, RETRY_CONNECTION);

	BEGIN_LOOP();
	while (1)
	{
		sleep_ms(DELAY_INTERVAL);

		client.Update();

		tog_pressed = gpio_get(BUTTON_TOG_PIN);

		if (tog_pressed && allow)
		{
			// Toggled here.
			toggle = !toggle;
			allow = false;
			payload = toggle ? WIFI_PAYLOAD_HIGH : WIFI_PAYLOAD_LOW;
		}
		else if (!tog_pressed)
			allow = true;

		if (!toggle)
		{
			// Set payload high/low based on button status.
			payload = gpio_get(BUTTON_PTT_PIN) ? WIFI_PAYLOAD_HIGH : WIFI_PAYLOAD_LOW;
		}
#if USING_EXT_LED
		gpio_put(EXT_LED_PIN, payload);
#endif
		int8_t e = client.SendUDP(&payload, sizeof(payload), WIFI_PORT);
		if (e != ERR_OK)
		{
			WARN("Packet Error: %d.\n", e);
		}
	}

}