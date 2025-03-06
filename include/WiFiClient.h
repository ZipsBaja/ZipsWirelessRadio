#pragma once

#if defined(USING_PICO_W) || defined(USING_PICO_2_W)

#include <WiFiController.h>
#include <vector>

namespace uazips
{

    int scan_callback(void*, const cyw43_ev_scan_result_t*);


    class WiFiClient : public WiFiController
    {
    private:
        static std::vector<const cyw43_ev_scan_result_t*> scanned_ssids;

        bool connected;
        bool allow_retry;
        NetworkInfo connection_info;
        ip4_addr_t client_ip;
    public:
        inline WiFiClient() : WiFiController(), connected(0)
        {
            cyw43_arch_enable_sta_mode();
        }

        inline ~WiFiClient()
        {
            cyw43_arch_disable_sta_mode();
        }

        inline std::vector<const cyw43_ev_scan_result_t*> ScanNetworks()
        {          
            cyw43_wifi_scan_options_t scanopts = {0};
            
            if (cyw43_wifi_scan(&cyw43_state, &scanopts, NULL, scan_callback))
                WARN("Failed to start scan.");

            std::vector<const cyw43_ev_scan_result_t*> copy;
            copy.assign(scanned_ssids.begin(), scanned_ssids.end());
            scanned_ssids.clear();
            return copy;
        }

        inline bool Connect(const char* ssid = "PicoW-AP", const char* password = 0, uint32_t auth = CYW43_AUTH_WPA2_AES_PSK, uint32_t timeout = 30000, void (*fail_callback)() = 0, bool retry_conn = 0)
        {
            allow_retry = retry_conn;
            connection_info.ssid = ssid;
            connection_info.password = password;
            connection_info.auth = auth;
            connection_info.timeout_ms = timeout;
            connection_info.fail_callback = fail_callback;
        Retry:
            if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, auth, timeout))
            {
                connected = false;
                if (fail_callback)
                    fail_callback();
                if (allow_retry)
                    goto Retry;
            }
            else
            {
                client_ip = cyw43_state.netif->ip_addr;
                ip_switch = cyw43_state.dhcp_client.server_ip_addr;
                connected = true;
            }

            return connected;
        }

        inline bool Connect(const NetworkInfo& info)
        {
            return Connect(info.ssid, info.password, info.auth, info.timeout_ms, info.fail_callback, 1);
        }

        inline void AllowRetry(bool b)
        {
            allow_retry = b;
        }

        inline bool IsConnected() const
        {
            return connected;
        }

        friend int scan_callback(void*, const cyw43_ev_scan_result_t*);
    };

    int scan_callback(void*, const cyw43_ev_scan_result_t* result)
    {
        if (result)
        {
            WiFiClient::scanned_ssids.push_back(result);
            LOG("%s", result->ssid);
        }
        return 0;
    }  

    std::vector<const cyw43_ev_scan_result_t*> WiFiClient::scanned_ssids = std::vector<const cyw43_ev_scan_result_t*>();

}

#endif