$AVRDUDE = 'C:\Users\Admin\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/bin/avrdude.exe'
$CONF = 'C:\Users\Admin\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf'

$COM = 'COM11'

$FLAGS = @('-v';'-V';'-patmega328p';'-carduino';'-b115200';'-D';)

&$AVRDUDE -C $CONF $FLAGS "-P$COM" "-Uflash:w:$PWD\bin\peekbot-aps.hex:i"





