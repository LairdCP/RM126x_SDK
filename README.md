[![Ezurio](app/documentation/images/ezurio_logo.jpg)](https://www.ezurio.com/)

# RM126x GSDK Extension

[![RM1261 & RM1262](app/documentation/images/rm126x_render.jpg)](https://www.ezurio.com/wireless-modules/lorawan-modules-solutions/rm126x-ultra-low-power-lorawan-a-b-c-module)
[![Silabs](app/documentation/images/silabs_logo.jpg)](https://www.silabs.com)
[![Semtech](app/documentation/images/semtech_logo.jpg)](https://www.semtech.com)
[![Gecko SDK](app/documentation/images/gecko_sdk_logo.jpg)](https://www.silabs.com/developers/gecko-software-development-kit)
[![Simplicity Studio](app/documentation/images/simplicity_studio_logo.jpg)](https://www.silabs.com/developers/simplicity-studio)

This is the release page for the Ezurio [RM126x][RM126x product brief] product family GSDK Extension. It consists of a full stack integration and contains template applications for demonstrating the module functionality and to act as the basis for further development.

All template applications in this repository are considered to be EVALUATION QUALITY which implies that the code provided is subjected to limited testing and is provided as-is. It is not suitable for production environments.

The product is available in [RM1261][RM126x module datasheet] & [RM1262][RM126x module datasheet] variants.

The RM1261 incorporates a [Semtech SX1261][Semtech SX1261 product page] LoRa radio and is intended for use in countries with a transmit power limitation of up to 15dBm (EU, India, Japan, Taiwan & UK).

The RM1262 incorporates a [Semtech SX1262][Semtech SX1262 product page] LoRa radio and is intended for use in countries with a transmit power limitation of up to 22dBm (Australia, New Zealand & US/Canada).

# Content

A full list of available template applications in this repository is defined below.

| Template application name | Description                                                                             | Link to template application                      | Link to documentation                                     |
|---------------------------|-----------------------------------------------------------------------------------------|---------------------------------------------------|-----------------------------------------------------------|
| rm1261_class_a            | Class A connection using the RM1261                                                     | [Click Here](app/example/rm1261_class_a/)         | [Click Here](app/documentation/rm126x_class_a.md)         |
| rm1261_class_a_co         | Class A connection using the RM1261 and uplink of carbon monoxide level data            | [Click Here](app/example/rm1261_class_a_co/)      | [Click Here](app/documentation/rm126x_class_a_co.md)      |
| rm1261_class_a_disp       | Class A connection using the RM1261 and display of uplink count data                    | [Click Here](app/example/rm1261_class_a_disp/)    | [Click Here](app/documentation/rm126x_class_a_disp.md)    |
| rm1261_class_a_env        | Class A connection using the RM1261 and uplink of environmental data                    | [Click Here](app/example/rm1261_class_a_env/)     | [Click Here](app/documentation/rm126x_class_a_env.md)     |
| rm1261_class_a_hr         | Class A connection using the RM1261 and uplink of heart rate data                       | [Click Here](app/example/rm1261_class_a_hr/)      | [Click Here](app/documentation/rm126x_class_a_hr.md)      |
| rm1261_class_a_th         | Class A connection using the RM1261 and uplink of temperature and humidity data         | [Click Here](app/example/rm1261_class_a_th/)      | [Click Here](app/documentation/rm126x_class_a_th.md)      |
| rm1261_class_a_weather    | Class A connection using the RM1261 and uplink of weather data                          | [Click Here](app/example/rm1261_class_a_weather/) | [Click Here](app/documentation/rm126x_class_a_weather.md) |
| rm1262_class_a            | Class A connection using the RM1262                                                     | [Click Here](app/example/rm1262_class_a/)         | [Click Here](app/documentation/rm126x_class_a.md)         |
| rm1262_class_a_co         | Class A connection using the RM1262 and uplink of carbon monoxide level data            | [Click Here](app/example/rm1262_class_a_co/)      | [Click Here](app/documentation/rm126x_class_a_co.md)      |
| rm1262_class_a_disp       | Class A connection using the RM1262 and display of uplink count data                    | [Click Here](app/example/rm1262_class_a_disp/)    | [Click Here](app/documentation/rm126x_class_a_disp.md)    |
| rm1262_class_a_env        | Class A connection using the RM1262 and uplink of environmental data                    | [Click Here](app/example/rm1262_class_a_env/)     | [Click Here](app/documentation/rm126x_class_a_env.md)     |
| rm1262_class_a_hr         | Class A connection using the RM1262 and uplink of heart rate data                       | [Click Here](app/example/rm1262_class_a_hr/)      | [Click Here](app/documentation/rm126x_class_a_hr.md)      |
| rm1262_class_a_th         | Class A connection using the RM1262 and uplink of temperature and humidity data         | [Click Here](app/example/rm1262_class_a_th/)      | [Click Here](app/documentation/rm126x_class_a_th.md)      |
| rm1262_class_a_weather    | Class A connection using the RM1262 and uplink of weather data                          | [Click Here](app/example/rm1262_class_a_weather/) | [Click Here](app/documentation/rm126x_class_a_weather.md) |
| rm1261_class_b            | Class B connection using the RM1261                                                     | [Click Here](app/example/rm1261_class_b/)         | [Click Here](app/documentation/rm126x_class_b.md)         |
| rm1262_class_b            | Class B connection using the RM1262                                                     | [Click Here](app/example/rm1262_class_b/)         | [Click Here](app/documentation/rm126x_class_b.md)         |
| rm1261_class_c            | Class C connection using the RM1261                                                     | [Click Here](app/example/rm1261_class_c/)         | [Click Here](app/documentation/rm126x_class_c.md)         |
| rm1262_class_c            | Class C connection using the RM1262                                                     | [Click Here](app/example/rm1262_class_c/)         | [Click Here](app/documentation/rm126x_class_c.md)         |

# Configuration & Usage

Full details of configuration and usage of the template applications is described in the [RM126x GSDK Extension User Guide][RM126x GSDK Extension User Guide].

All template applications were tested using the appropriate [RM126x DVK][RM126x DVK user guide].

The [Getting started with the RM126x GSDK Extension video][Getting started with the RM126x GSDK Extension video] provides a step by step walkthrough of usage of the RM126x GSDK Extension.

[RM126x product brief]: <https://www.ezurio.com/documentation/product-brief-rm126x-series>
[RM126x module datasheet]: <https://www.ezurio.com/documentation/datasheet-rm126x-lorawan-module>
[RM126x DVK user guide]: <https://www.ezurio.com/documentation/user-guide-rm126x-development-kit>
[Semtech SX1261 product page]: <https://www.semtech.com/products/wireless-rf/lora-connect/sx1261>
[Semtech SX1262 product page]: <https://www.semtech.com/products/wireless-rf/lora-connect/sx1262>
[RM126x GSDK Extension User Guide]: <https://www.ezurio.com/documentation/application-note-c-code-development-rm126x-series>
[Getting started with the RM126x GSDK Extension video]: <https://www.ezurio.com/resources/videos/getting-started-with-the-rm126x-gsdk-extension>
