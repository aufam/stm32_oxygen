#include "oxygen/oxygen.h"
#include "etl/keywords.h"


fun Oxygen::init() -> void {
    uart.init({
        .baudrate=9600,
        .rxCallback=etl::bind<&Oxygen::rxCallback>(this),
    });
    #if PROJECT_OXYGEN_IS_USING_NOTIFIER
    notifier.init(); 
    #endif
}

fun Oxygen::deinit() -> void { 
    uart.deinit({.rxCallback=etl::bind<&Oxygen::rxCallback>(this)}); 
    #if PROJECT_OXYGEN_IS_USING_NOTIFIER
    notifier.deinit(); 
    #endif
}

fun Oxygen::rxCallback(const uint8_t* buf, size_t len) -> void {
    // check len and start bytes
    if (len != 12) return;
    if (buf[0] != 0x16) return; // head
    if (buf[1] != 0x09) return; // len
    if (buf[2] != 0x01) return; // cmd

    // checksum
    char checksum = 0;
    for (val i in etl::range(len -1)) 
        checksum += buf[i];
    checksum = 0x00 - checksum;
    if (checksum != buf[len - 1]) return;

    // decode
    values.concentrate = etl::safe_div<float>(buf[3] << 8 | buf[4], 10.f);
    values.flow        = etl::safe_div<float>(buf[5] << 8 | buf[6], 10.f);
    values.temperature = etl::safe_div<float>(buf[7] << 8 | buf[8], 10.f);

    // notify
    #if PROJECT_OXYGEN_IS_USING_NOTIFIER
    etl::ignore = notifier.setFlags(0b1);
    #endif
}