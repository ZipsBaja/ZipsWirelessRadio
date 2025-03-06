#pragma once

#include <ZipsLib.h>
#include <LoopingFunction.h>

#include <lwip/udp.h>

static void lwip_begin()
{
#if NOSYS
    cyw43_arch_lwip_begin();
#endif
}

static void lwip_end()
{
#if NOSYS
    cyw43_arch_lwip_end();
#endif
}

namespace uazips
{

    /*
    * Automates the construction and destruction of a `pbuf`.
    * This takes ownership of the packet buffer and will free it accordingly.
    */
    struct PacketWrapper
    {
        pbuf* packet_buffer;
        inline PacketWrapper(void* payload, size_t buffer_len)
        {
            packet_buffer = pbuf_alloc(PBUF_TRANSPORT, buffer_len, PBUF_RAM);
            memcpy(packet_buffer->payload, payload, buffer_len);
        }

        inline PacketWrapper()
        {}

        // WARNING: This takes ownership of the pointer!
        inline PacketWrapper(pbuf* packet_buffer) : packet_buffer(packet_buffer)
        {}

        inline ~PacketWrapper()
        {
            pbuf_free(packet_buffer);
        }

        inline operator pbuf*()
        {
            return packet_buffer;
        }

    };

    struct NetworkInfo
    {
        const char* ssid;
        const char* password;
        uint32_t auth;
        uint32_t timeout_ms;
        void (*fail_callback)();
    };

    class WiFiController : public LoopingFunction
    {
    public:
        ip4_addr_t ip_switch;
        udp_pcb* pcb;
    public:

        inline WiFiController()
        {
            lwip_begin();
            pcb = udp_new();
            lwip_end();
        }

        virtual inline ~WiFiController()
        {
            lwip_begin();
            udp_remove(pcb);
            lwip_end();
        }

        /*
        * @return The current selected IP address.
        * If the device is a client, the server IP address is returned. 
        */
        inline const ip4_addr_t& GetIPAddress() const
        {
            return ip_switch;
        }
        
        inline err_t SendUDP(void* packet_payload, size_t payload_size, uint16_t port) const
        {
            lwip_begin();

            PacketWrapper friggin_packet_yo(packet_payload, payload_size);

            err_t e = udp_sendto(pcb, friggin_packet_yo.packet_buffer, &ip_switch, port);

            lwip_end();

            return e;
        }

        inline void AwaitUDP(udp_recv_fn receive_handler, uint16_t port, void* additional_arg_struct) const
        {
            lwip_begin();

            udp_bind(pcb, IP_ANY_TYPE, port);
            udp_recv(pcb, receive_handler, additional_arg_struct);

            lwip_end();
        }

        inline static ip4_addr_t CreateIPV4(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
        {
            ip4_addr_t ip;
            IP4_ADDR(&ip, a, b, c, d);
            return ip;
        }
        inline static ip4_addr_t CreateIPV4(uint8_t* v)
        {
            return CreateIPV4(v[0], v[1], v[2], v[3]);
        }

        inline virtual void Update() override
        {
#ifdef PICO_CYW43_ARCH_POLL
            cyw43_arch_poll();
#endif
        }
    };

}