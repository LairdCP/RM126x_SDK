[![Ezurio](images/ezurio_logo.jpg)](https://www.ezurio.com/)

# RM126x Template Applications - RM126x Class B

[![RM1261 & RM1262](images/rm126x_render.jpg)](https://www.ezurio.com/wireless-modules/lorawan-modules-solutions/rm126x-ultra-low-power-lorawan-a-b-c-module)[![Silabs](images/silabs_logo.jpg)](https://www.silabs.com)[![Semtech](images/semtech_logo.jpg)](https://www.semtech.com)[![Gecko SDK](images/gecko_sdk_logo.jpg)](https://www.silabs.com/developers/gecko-software-development-kit)[![Simplicity Studio](images/simplicity_studio_logo.jpg)](https://www.silabs.com/developers/simplicity-studio)

This template application demonstrates LoRaWAN Class B functionality for the [RM1261][RM126x module datasheet] and [RM1262][RM126x module datasheet].

The RM1261 incorporates a [Semtech SX1261][Semtech SX1261 product page] LoRa radio and is intended for use in countries with a transmit power limitation of up to 15dBm (EU, India, Japan, Taiwan & UK).

The RM1262 incorporates a [Semtech SX1262][Semtech SX1262 product page] LoRa radio and is intended for use in countries with a transmit power limitation of up to 22dBm (Australia, New Zealand & US).

# Configuration

Details of the associated configuration options are described in the [RM126x GSDK Extension user guide][RM126x GSDK Extension user guide].

The template application was tested with an [RM1261 DVK][RM126x DVK user guide] and [RM1262 DVK][RM126x DVK user guide].

# Operation

During start-up, peripherals associated with the Semtech radio are started, then the LoRaWAN stack initialised.

The application then attempts to initiate a Class A connection with the Network Server, and will perform repeated attempts to establish the connection.

Upon successful connection, the application will send periodic uplinks with a 2-byte payload indicating the index of the uplink.

The uplink data is formatted as shown below.

![Uplink count payload](images/rm126x_uplink_count.jpg)

Also following connection, the application attempts to transition to Class B.

It first requests the time from the network. Once received, details of the application Ping Slot Periodicity are sent to the network server. Having received the time from the network and successful acknowledgement of receipt of the Ping Slot Periodicity by the network server, the application is then ready to listen for beacons.

Upon successful receipt of a beacon, the transition to Class B can be performed following the next periodic uplink by the application.

Downlink messages can then be received in the RXB receive windows opened within each Class B ping slot.

[RM126x module datasheet]: <https://www.ezurio.com/documentation/datasheet-rm126x-lorawan-module>
[RM126x DVK user guide]: <https://www.ezurio.com/documentation/user-guide-rm126x-development-kit>
[RM126x GSDK Extension user guide]: <https://www.ezurio.com/documentation/application-note-c-code-development-rm126x-series>
[Semtech SX1261 product page]: <https://www.semtech.com/products/wireless-rf/lora-connect/sx1261>
[Semtech SX1262 product page]: <https://www.semtech.com/products/wireless-rf/lora-connect/sx1262>
