[![Laird Connectivity](images/laird_connectivity_logo.jpg)](https://www.lairdconnect.com/)

# RM126x Template Applications - RM1261 Class C

[![RM1261 & RM1262](images/rm126x_render.jpg)](https://www.lairdconnect.com/wireless-modules/lorawan-modules-solutions/rm126x-ultra-low-power-lorawan-a-b-c-module)[![Silabs](images/silabs_logo.jpg)](https://www.silabs.com)[![Semtech](images/semtech_logo.jpg)](https://www.semtech.com)[![Gecko SDK](images/gecko_sdk_logo.jpg)](https://www.silabs.com/developers/gecko-software-development-kit)[![Simplicity Studio](images/simplicity_studio_logo.jpg)](https://www.silabs.com/developers/simplicity-studio)

This template application demonstrates LoRaWAN Class C functionality for the [RM1261][RM126x module datasheet] product variant.

The RM1261 incorporates a [Semtech SX1261][Semtech SX1261 product page] LoRa radio and is intended for use in countries with a transmit power limitation of up to 15dBm (EU, India, Japan, Taiwan & UK).

# Configuration

Details of the associated configuration options are described in the [RM126x GSDK Extension user guide][RM126x GSDK Extension user guide].

The template application was tested with an [RM1261 DVK][RM126x DVK user guide].

# Operation

During start-up, peripherals associated with the Semtech radio are started, then the LoRaWAN stack initialised.

The application then attempts to initiate a Class A connection with the Network Server, and will perform repeated attempts to establish the connection.

Upon successful connection, the application will send periodic uplinks with a 4-byte payload indicating the index of the uplink.

Downlink messages can be received in the RX1 receive window opened following each uplink.

Class C mode is also enabled upon successful connection with the Class C receive window being opened following closure of the RX1 window after each periodic uplink.

[RM126x module datasheet]: <https://www.lairdconnect.com/documentation/datasheet-rm126x-lorawan-module>
[RM126x DVK user guide]: <https://www.lairdconnect.com/documentation/user-guide-rm126x-development-kit>
[RM126x GSDK Extension user guide]: <https://www.lairdconnect.com/documentation/application-note-c-code-development-rm126x-series>
[Semtech SX1261 product page]: <https://www.semtech.com/products/wireless-rf/lora-connect/sx1261>
