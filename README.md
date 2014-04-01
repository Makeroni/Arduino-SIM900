Phone alarm using Arduino and SIM900
====================================

Functions to work with Arduino and SIM900 GSM shield as phone alarm.

```C
// Power SIM900 on
SIM900powerOn();

// Expect a call for n seconds
// Store phone number in EEPROM
SIM900expectCall(int n);

// Call back during n seconds
SIM900callHome(int n);

// Power SIM900 off
SIM900powerOff();
```

Localization
------------

Phone number is checked according to spanish telephone numbering (6 or 9 at the start).

Replace your country codes in the corresponding sentences:

````C
// Check number starts with 6 or 9
if( phone[0] != '6' && phone[0] != '9' )
````

Replace the international prefix in the call back function:

````C
// Add Spain country code
sprintf(msg, "ATD + +34%s;", phone);
````


Any problems?
-------------
Feel free to [write an issue](https://github.com/Makeroni/Arduino-SIM900/issues) if you have any questions or problems.



Copyright and license
---------------------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

See the file `COPYING` in this directory or  http://www.gnu.org/licenses/, for a description of the GNU General Public License terms under which you can copy the files.
