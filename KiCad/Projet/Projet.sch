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
Text Label 4750 1550 2    50   ~ 0
ThermistorSignal
Text Label 4750 1650 2    50   ~ 0
3V3
Text Label 4750 1750 2    50   ~ 0
GND
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
Text Label 4800 2300 2    50   ~ 0
GND
Text Label 4800 2200 2    50   ~ 0
FlowSignal
Text Label 4800 2100 2    50   ~ 0
3V3
$Comp
L Connector:Conn_01x03_Female FlowControler1
U 1 1 5F89F991
P 5000 2200
F 0 "FlowControler1" H 5028 2226 50  0000 L CNN
F 1 "Cable_01x03_Female" H 5028 2135 50  0000 L CNN
F 2 "Connector_Wire:SolderWire-0.25sqmm_1x03_P4.2mm_D0.65mm_OD1.7mm" H 5000 2200 50  0001 C CNN
F 3 "~" H 5000 2200 50  0001 C CNN
	1    5000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 2200 4400 2200
Wire Wire Line
	4400 2200 4400 2250
Wire Wire Line
	4400 2550 2950 2550
Wire Wire Line
	4750 1550 4100 1550
Wire Wire Line
	4100 1550 4100 1950
Wire Wire Line
	4100 1950 2950 1950
$Comp
L Device:R FlowControlerSignalResistor1
U 1 1 5F8A24A9
P 4400 2400
F 0 "FlowControlerSignalResistor1" H 4470 2446 50  0000 L CNN
F 1 "R200ohm" H 4470 2355 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4330 2400 50  0001 C CNN
F 3 "~" H 4400 2400 50  0001 C CNN
	1    4400 2400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female RainControler1
U 1 1 5F8A34C2
P 5000 2700
F 0 "RainControler1" H 5028 2676 50  0000 L CNN
F 1 "Cable_01x02_Female" H 5028 2585 50  0000 L CNN
F 2 "Connector_Wire:SolderWire-0.25sqmm_1x02_P4.5mm_D0.65mm_OD2mm" H 5000 2700 50  0001 C CNN
F 3 "~" H 5000 2700 50  0001 C CNN
	1    5000 2700
	1    0    0    -1  
$EndComp
Text Label 4800 2700 2    50   ~ 0
3V3
Text Label 4800 2800 2    50   ~ 0
RainSignal
Wire Wire Line
	4800 2800 4600 2800
Wire Wire Line
	4350 2650 4350 2800
Wire Wire Line
	2950 2650 4350 2650
$Comp
L Device:R RainControlerSignalResistor1
U 1 1 5F8A4530
P 4600 3000
F 0 "RainControlerSignalResistor1" H 4670 3046 50  0000 L CNN
F 1 "R200ohm" H 4670 2955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4530 3000 50  0001 C CNN
F 3 "~" H 4600 3000 50  0001 C CNN
	1    4600 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2850 4600 2800
Connection ~ 4600 2800
Wire Wire Line
	4600 2800 4350 2800
$Comp
L power:GND #PWR0101
U 1 1 5F8A5128
P 4600 3150
F 0 "#PWR0101" H 4600 2900 50  0001 C CNN
F 1 "GND" H 4605 2977 50  0000 C CNN
F 2 "" H 4600 3150 50  0001 C CNN
F 3 "" H 4600 3150 50  0001 C CNN
	1    4600 3150
	1    0    0    -1  
$EndComp
Text Label 2650 1650 1    50   ~ 0
3V3
$Comp
L power:GND #PWR?
U 1 1 5F8A6B1A
P 2550 3600
F 0 "#PWR?" H 2550 3350 50  0001 C CNN
F 1 "GND" H 2555 3427 50  0000 C CNN
F 2 "" H 2550 3600 50  0001 C CNN
F 3 "" H 2550 3600 50  0001 C CNN
	1    2550 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3250 2550 3600
$EndSCHEMATC
