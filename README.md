# RobotPotager
ESP8266 linked to Domoticz.

# Sensors: 
## Temperature: NTC Thermistor
https://www.aliexpress.com/item/32830765468.html
It can measure the temperature directly in the water.  It is widely used in temperature monitoring for indoor, outdoor, greenhouse, etc. You can plug the sensor onto the adapter module directory with screwing.

Specifications:
- Working voltage: 2.2~12V DC
- Working current: 0.5mA (max)
- Measuring range: -30~120℃
- Measuring Accuracy: ±2% (4℃~50℃), ±3% (-15℃~80℃)
- B value: 3950K
- R25: 10Kohm
- Sampling resistor parameters: 10k ohm, 0.1%, 10ppm
- Sensor connector: XH2.54-2P
- Lead length: 1m
- Adapter output connector: standard electronic brick interface, 2.54mm-3P header

## Rain meter: MISOL Rain Bucket Sensor
https://www.aliexpress.com/item/1000001838878.html
This Rain Buacket Gauge is designed to work with the MISOL Weather Station Pro. The tip of the tipping bucket can hold up to 0.3537 mm per tip.

## Soil moisture
https://www.aliexpress.com/item/32959352428.html

Specifications: 
- Working Voltage: DC 3.3-12V
- Working Current: <20ma
- Output Current: <30ma
- I/O: + - DO AO
- Dimension: 36x15x7mm 

# Domoticz integration
At startup, will query Domoticz API **by plan** and browse devices where plan matching ESP8266 hostname.
In get_DevicesIdx function, save devices indexes.

Functions send_**** will send values to Domoticz device.
