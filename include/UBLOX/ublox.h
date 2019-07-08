#ifndef UBLOX_H
#define UBLOX_H

#include <stdint.h>
#include <iostream>
#include <fstream>

#include "async_comm/serial.h"
#include "async_comm/udp.h"

#include "UBLOX/parsers/ubx.h"
#include "UBLOX/parsers/rtcm.h"
#include "UBLOX/parsers/nmea.h"


class UBLOX
{
public:

    typedef enum
    {
        NONE = 0,
        ROVER = 0b10,
        BASE = 0b11,
        RTK = 0b10,
    } rtk_type_t;

    UBLOX(std::string port);
    ~UBLOX();

    void initBase(std::string local_host, uint16_t local_port,
                  std::string remote_host, uint16_t remote_port);

    void initRover(std::string local_host, uint16_t local_port,
                   std::string remote_host, uint16_t remote_port);

    void initLogFile(std::string filename);
    void readFile(std::string filename);

    async_comm::UDP* udp_ = nullptr;
    async_comm::Serial serial_;

    UBX ubx_;
    RTCM rtcm_;
    NMEA nmea_;

    std::ofstream log_file_;

    inline void registerUBXCallback(uint8_t cls, uint8_t type, UBX::ubx_cb cb)
    {
        ubx_.registerCallback(cls, type, cb);
    }

    rtk_type_t type_;

    void serial_read_cb(const uint8_t* buf, size_t size);
    void udp_read_cb(const uint8_t *buf, size_t size);
    void rtcm_complete_cb(const uint8_t* buf, size_t size);
};

#endif
