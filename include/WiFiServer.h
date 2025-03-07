#pragma once

#if defined(USING_PICO_W) || defined(USING_PICO_2_W)

#include <WiFiController.h>

extern "C"
{
#include <dhcpserver.h>
}

namespace uazips
{

    class WiFiServer : public WiFiController
    {
    private:
        dhcp_server_t dhcp;
        ip4_addr_t ip_self;
    public:
        inline WiFiServer(const char* ssid = "PicoW-AP", const char* password = 0, uint32_t auth = CYW43_AUTH_WPA2_AES_PSK) : WiFiController()
        {
            cyw43_arch_enable_ap_mode(ssid, password, auth);
            
            ip4_addr_t gw = CreateIPV4(192, 168, 4, 1);
            ip4_addr_t mask = CreateIPV4(255, 255, 255, 0);
            dhcp_server_init(&dhcp, &gw, &mask);
            ip_self = gw;
        }

        inline ~WiFiServer()
        {
            cyw43_arch_disable_ap_mode();
            dhcp_server_deinit(&dhcp);
        }

        /*
        * Network switching tool for the server to select
        * different devices.
        */
        inline void Switch(const ip4_addr_t& ip)
        {
            ip_switch = ip;
        }

        inline const ip4_addr_t& GetSelfIPAddress() const
        {
            return ip_self;
        }

    };

}

#endif