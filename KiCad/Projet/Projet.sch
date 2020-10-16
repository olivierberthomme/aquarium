EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4750 1550 2950 1950
$Comp
L MCU_Module:WeMos_D1_mini U1
U 1 1 5F82EB90
P 2550 2450
F 0 "U1" H 2550 1561 50  0000 C CNN
F 1 "WeMos_D1_mini" H 2550 1470 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 2550 1300 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 700 1300 50  0001 C CNN
	1    2550 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 TH_NTC1
U 1 1 5F83028F
P 4950 1650
F 0 "TH_NTC1" H 5030 1692 50  0000 L CNN
F 1 "Thermistor_NTC" H 5030 1601 50  0000 L CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x03_P1.00mm_Horizontal" H 4950 1700 50  0001 C CNN
F 3 "~" H 4950 1700 50  0001 C CNN
	1    4950 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1650 2650 1650
Wire Wire Line
	4750 1750 2550 3250
Text Label 3600 1650 0    50   ~ 0
3V3
Text Label 3200 2800 0    50   ~ 0
gnd
$EndSCHEMATC
