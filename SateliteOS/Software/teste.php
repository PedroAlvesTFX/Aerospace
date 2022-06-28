<html><pre>
<?php
$TIMEOUT = 10;
$fd = dio_open( '/dev/ttyS0', O_RDWR | O_NONBLOCK );
dio_tcsetattr( $fd, array(
			’baud’       => 115200,
			’bits’       => 8,
			’stop’       => 1,
			’parity’     => 0,
			’flow_control’ => 0,
			’is_canonical’ => 0	));
dio_write( $fd, ’AT’ );
$input = ’’;
$time_start = time();
while($time_start + $TIMEOUT > time())
    {
    $input .= dio_read( $fd );
    usleep( 100000 );
    }
echo $input;
dio_close( $fd );
?>