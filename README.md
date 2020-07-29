Description
===========
Vintage series Rhode &amp; Schwarz spectrum analyzers feature a number
of code-activated options.
* FSP
* FSEA20
* FSEA30
* FSEB20
* FSEB30
* FSEM20
* FSEM21
* FSEM30
* FSEM31
* FSIQ3
* FSIQ7
* FSIQ26
* FSIQ40
* Others?

This project contains the source code to generate the keys required to
activate these options.

Building
========
Build the project with:<br>
$ make

Usage
=====
keygen -s serialnr [-t type]

Example
=======
	$ ./x86_64-linux-gnu/keygen -s 123456/123 -t fsiq3
	Option keys for FSIQ3 with serial number 123456/123 are:
	2999404191 - K10 - GSM MS Analyzer
	0015198213 - K11 - GSM BTS Analyzer
	3032448905 - B5  - FFT
	2158664308 - ESI (Does not show on FSIQ3)
	1626762585 - Kxx - Receiver Mode
	2119393690 - FSIQ
	1143185227 - K71 - CDMA ONE BTS Analyzer
	3964804064 - K72 - W-CDMA BTS Analyzer
	3956932042 - K12 - Digital Standard ICO
	3391675518 - K20 - EDGE MS Analyzer Extension
	4223392397 - K21 - EDGE BTS Analyzer Extension
	2594997132 - K73 - W-CDMA MS Analyzer
	3882610711 - K30 - 850 MHz Extension for K10/K20
	1893721019 - K31 - 850 MHz Extension for K11/K21
	1351204819 - AFB (Does not show on FSIQ3)
	0167478992 - Freq limit 41GHz (Does not show on FSIQ3)

Important note
==============
This project needs your help. If your instrument type is reported not to
be supported, omit the type from the command line options to optain a
list of keys:
	$ ./x86_64-linux-gnu/keygen -s 123456/123
	Raw keys for instrument with serial number 123456/123 are:
	000: 2999404191
	001: 0015198213
	002: 3032448905
	...
	197: 4148504597
	198: 1532745977
	199: 0342575056
These keys should be accepted by your instrument. If not,
this tool is not capable of supporting your instrument.<br>
Even if accepted, not every key activates an option, and this is where
your help is needed. If you have found a key to activate an option on
your instrument, please report the instrument typ, the option and
description and the key number (0...199) so it can be added.<br>
Also note: Even if your instrument type is supported, the option list
may still be incomplete and additional raw keys may (or may not)
activate new options.<br>
Naturally, you are using this tool at your own risk.
