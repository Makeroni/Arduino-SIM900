Phone alarm using Arduino and SIM900
====================================

Functions to work with Arduino and SIM900 GSM shield as phone alarm.

```C
// Power SIM900 on
SIM900powerOn();

// Expect a call for n seconds
// Save phone number persistent (EEPROM)
SIM900expectCall(n);

// Call back during n seconds
SIM900callHome(n);

// Power SIM900 off
SIM900powerOff();
```

Copyright and license
---------------------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

See the file `COPYING` in this directory or  http://www.gnu.org/licenses/, for a description of the GNU General Public License terms under which you can copy the files.
