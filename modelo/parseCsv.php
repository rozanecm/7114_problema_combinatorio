<?php
$fila = 1;
$array = array();
if (($gestor = fopen("distances.csv", "r")) !== FALSE) {
    while (($datos = fgetcsv($gestor, 1000, ",")) !== FALSE) {
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
$fp = fopen('distances2.csv', 'w');
foreach($array as $key => $subarray){
	foreach($subarray as $key2 => $value){
		$data = $key.','.$key2.','.$value.PHP_EOL;
		fwrite($fp, $data);
	}	
}
fclose($fp);

?>