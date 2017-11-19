<?php
$fila = 1;
$arrayIncluded = ["Alabama","Arizona","Arkansas","California","Colorado","Connecticut","Delaware","Florida","Georgia","Idaho","Illinois","Indiana","Iowa","Kansas","Kentucky","Louisiana","Maine","Maryland","Massachusetts","Michigan","Minnesota","Mississippi","Missouri","Montana","Nebraska","Nevada","New Hampshire","New Jersey","New Mexico","New York","North Carolina","North Dakota","Ohio","Oklahoma","Oregon"];
$array = array();
if (($gestor = fopen("distances.csv", "r")) !== FALSE) {
    while (($datos = fgetcsv($gestor, 1000, ",")) !== FALSE) {
        if(!in_array($datos[0], $arrayIncluded) || !in_array($datos[1], $arrayIncluded)){
            continue;
        }
    	echo implode(" - ", $datos) . PHP_EOL;
        if(!array_key_exists($datos[0], $array)){
        	$array[$datos[0]] = array();
        	$array[$datos[0]][$datos[0]] = 0;

        }
        if(!array_key_exists($datos[1], $array)){
        	$array[$datos[1]] = array();
        	$array[$datos[1]][$datos[1]] = 0;
        }
        $array[$datos[0]][$datos[1]] = $datos[2];
        $array[$datos[1]][$datos[0]] = $datos[2];
    }
    fclose($gestor);
}
ksort($array);

foreach($array as $key => $subarray){
	ksort($subarray);
}
$fp = fopen('distances4.csv', 'w');
foreach($array as $key => $subarray){
	foreach($subarray as $key2 => $value){
		$data = $key.','.$key2.','.$value.PHP_EOL;
		fwrite($fp, $data);
	}	
}
fclose($fp);

?>