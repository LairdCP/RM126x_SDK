[![Ezurio](images/ezurio_logo.jpg)](https://www.ezurio.com/)

# RM126x Template Applications - RM126x Class A Carbon monoxide level data

[![RM1261 & RM1262](images/rm126x_render.jpg)](https://www.ezurio.com/wireless-modules/lorawan-modules-solutions/rm126x-ultra-low-power-lorawan-a-b-c-module)[![Silabs](images/silabs_logo.jpg)](https://www.silabs.com)[![Semtech](images/semtech_logo.jpg)](https://www.semtech.com)[![Gecko SDK](images/gecko_sdk_logo.jpg)](https://www.silabs.com/developers/gecko-software-development-kit)[![Simplicity Studio](images/simplicity_studio_logo_centre.jpg)](https://www.silabs.com/developers/simplicity-studio)[![MikroE](images/mikroe_logo.jpg)](https://www.mikroe.com)

This template application demonstrates LoRaWAN Class A functionality for the [RM1261][RM126x module datasheet] and [RM1262][RM126x module datasheet]. Carbon monoxide level data, read from a [MikroE CO 2 Click board][MikroE CO 2 Click board product page], is included in each uplink.

The RM1261 incorporates a [Semtech SX1261][Semtech SX1261 product page] LoRa radio and is intended for use in countries with a transmit power limitation of up to 15dBm (EU, India, Japan, Taiwan & UK).

The RM1262 incorporates a [Semtech SX1262][Semtech SX1262 product page] LoRa radio and is intended for use in countries with a transmit power limitation of up to 22dBm (Australia, New Zealand & US).

# Setup

The [RM126x DVK][RM126x DVK user guide] must be configured for I2C operation prior to fitting the Click board.

The CO 2 Click board is shown below.

![MikroE CO 2 Click](images/mikroe_3196_profile.jpg)

Care should be taken to ensure the designations on the Click connector align with those of the RM126x DVK.

The Click board fitted in the correct orientation is shown below.

![RM126x DVK with MikroE Heart Rate 2 Click](images/rm126x_mikroe_3196_profile.jpg)

The CO 2 board allows carbon monoxide levels to be read via I2C or as a voltage via the board AN output. This example uses the RM126x ADC to measure the carbon monoxide level from the CO 2 board. The board Measurement type selection switch marked OP / ADC should be set to the OP position to connect the CO 2 AN output to the RM126x ADC.

# Configuration

Details of the associated configuration options are described in the [RM126x GSDK Extension user guide][RM126x GSDK Extension user guide].

The template application was tested with an [RM1261 DVK][RM126x DVK user guide] and [RM1262 DVK][RM126x DVK user guide].

# Operation

During start-up, the CO 2 board is initialised then peripherals associated with the Semtech radio started and the LoRaWAN stack initialised.

The application then attempts to initiate a Class A connection with the Network Server, and will perform repeated attempts to establish the connection.

Upon successful connection, the application will send periodic uplinks containing a 2-byte uplink count and the carbon monoxide level readings from the CO 2 board.

The uplink data is formatted as shown below.

![Uplink count and CO 2 payload](images/rm126x_uplink_3196.jpg)

Carbon monoxide level data is encoded into 16-bit integer and fractional parts.

Downlink messages can be received in the RX1 or RX2 receive windows opened following each uplink.

[RM126x module datasheet]: <https://www.ezurio.com/documentation/datasheet-rm126x-lorawan-module>
[RM126x DVK user guide]: <https://www.ezurio.com/documentation/user-guide-rm126x-development-kit>
[RM126x GSDK Extension user guide]: <https://www.ezurio.com/documentation/application-note-c-code-development-rm126x-series>
[Semtech SX1261 product page]: <https://www.semtech.com/products/wireless-rf/lora-connect/sx1261>
[Semtech SX1262 product page]: <https://www.semtech.com/products/wireless-rf/lora-connect/sx1262>
[MikroE CO 2 Click board product page]: <https://www.mikroe.com/co-2-click>
