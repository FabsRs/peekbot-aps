$AVRDUDE = 'C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude.exe'
$CONF = 'C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf'

$COM = 'COM8'

$FLAGS = @('-v';'-V';'-patmega328p';'-carduino';'-b115200';'-D';)

&$AVRDUDE -C $CONF $FLAGS "-P$COM" "-Uflash:w:$PWD\bin\peekbot-aps.hex:i"





