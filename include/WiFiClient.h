#if defined(USING_PICO_W) || defined(USING_PICO_2_W)

#include <ZipsLib.h>
#include <OrderedList.h>

namespace uazips
{

    class WiFiClient
    {
    private:
        static OrderedList<const cyw43_ev_scan_result_t*> ssids;

        static inline int ScanCallback(void* env, const cyw43_ev_scan_result_t* result)
        {
            if (result)
                WiFiClient::ssids.Push(result);
            return 0;
        }

    public:
        inline WiFiClient()
        {
            cyw43_arch_enable_sta_mode();
        }

        inline OrderedList<const cyw43_ev_scan_result_t*> ScanNetworks(uint32_t iterations)
        {
            ssids.Malloc(iterations);
            //for (int i = 0; i < iterations; i++)
            //{
                cyw43_wifi_scan_options_t scanopts = {0};
                if (cyw43_wifi_scan(&cyw43_state, &scanopts, NULL, ScanCallback))
                    WARN("Failed to start scan.");
            //}
            OrderedList<const cyw43_ev_scan_result_t*> copy = ssids;
            ssids.Malloc(1, 1);
            return copy;
        }

        inline void Connect()
        {

        }
    };

    OrderedList<const cyw43_ev_scan_result_t*> WiFiClient::ssids = OrderedList<const cyw43_ev_scan_result_t*>();

}

#endif