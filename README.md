# FNDClock
Make your own clock with Arduino.

## License
Copyright (c) 2016 Sung-ho Lee

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

## How to use

### Hardware configuration
RTC: DS1307 <br>
FND: S-5462CSR2/C

Pin 2 ~ Pin 8: A ~ G of FND via current limiting resistors<br>
Pin 9 ~ Pin 12: DIG.1 ~ DIG.4 of FND via NPN transistors

### Setting time
Send 3 bytes of data via serial(baudrate:9600)
<ul>
<li>Order: second, minute, hour</li>
<li>Format: BCD</li>
</ul>

## Donation
If you find this project useful for you and if you want, you can donate me.

### Paypal
[Donate via Paypal](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=KRGC3AGNBUU9S)

### Bitcoin
![QR code](http://chart.apis.google.com/chart?chs=200x200&cht=qr&chld=|1&chl=1NL1wZ2pgeyyvhAnh51YDaWwX34GoSFZA1)<br>
1NL1wZ2pgeyyvhAnh51YDaWwX34GoSFZA1
