[![Ezurio](images/ezurio_logo.jpg)](https://www.ezurio.com/)

# RM126x Template Applications - Split 8-Bit Payload Formatting

[![RM1261 & RM1262](images/rm126x_render.jpg)](https://www.ezurio.com/wireless-modules/lorawan-modules-solutions/rm126x-ultra-low-power-lorawan-a-b-c-module)[![Silabs](images/silabs_logo.jpg)](https://www.silabs.com)[![Semtech](images/semtech_logo.jpg)](https://www.semtech.com)[![Gecko SDK](images/gecko_sdk_logo.jpg)](https://www.silabs.com/developers/gecko-software-development-kit)[![Simplicity Studio](images/simplicity_studio_logo.jpg)](https://www.silabs.com/developers/simplicity-studio)

Floating point data within the range of a signed 8-bit number (-128 .. 127) is converted into two signed 8-bit bytes to conserve space when transmitting over LoRa and to simplify decoding by cloud based hosts. One byte represents the integer part of the data and the other the fractional part.

Data in this format is decoded as follows.

# Decoding split 8-bit payload data

Split 8-bit data appears as shown below in application payload data.

![Split 8-bit data application payload](images/formatter_decode_8_bit.jpg)

$Decoded Value = YY + (ZZ / 100)$

A temperature of 24.5C would be encoded as 24 and 50 and decoded as follows.

$24 + (50 / 100) = 24.5$

A negative temperature of -75.71 would be encoded as -75 and -71 and decoded as follows.

$-75 + (-71 / 100) = -75.71$
