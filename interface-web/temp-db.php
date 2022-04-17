<?php

// ACQUISITION

exec('mode COM4: baud=9600 data=8 stop=1 parity=n xon=on');

$fp =fopen("COM4", "a+");

if( !$fp) {
echo "Error";die();
}
sleep(1);
$data = fread($fp, 4);
$date = date("H:i:s");

//echo $data;

fclose($fp);
/*
exec('mode COM4: baud=9600 data=8 stop=1 parity=n xon=on');

global $data;

$fd = dio_open('COM4:', O_RDWR);
//dio_fcntl($fd, F_SETFL, O_SYNC);
//dio_tcsetattr($fd, array('baud' => 9600, 'bits' => 8, 'stop' => 1, 'parity' => 0));
sleep(1);
$data = dio_read($fd, 8);
//echo $data;

dio_close($fd);
*/

// INJECTION BD
global $db;

$db = new PDO('mysql:host=localhost;port=3306;dbname=pic18f', 'root', '');

$db->exec("INSERT INTO temperature(val, heure) VALUES ($data,UNIX_TIMESTAMP())");


// LECTURE BD

$query = $db -> prepare("SELECT val AS temp FROM  temperature ORDER BY heure DESC LIMIT 1");

$query -> execute();

$temperature = $query -> fetch(PDO::FETCH_ASSOC);

//echo $distance["dist"];

$query = $db -> prepare("SELECT heure AS t FROM temperature ORDER BY  heure DESC LIMIT 1");

$query -> execute();

$time = $query -> fetch(PDO::FETCH_ASSOC);

//echo $time["t"];

$return= '';
$return.=$temperature["temp"]."||";
$return.=$time["t"]."||";
echo $return;exit;


?>
