<?php
$request = $_GET;

if (array_key_exists('p', $request)) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 100);
} else if (array_key_exists('v', $request)) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(100, 300);
} else if (array_key_exists('t', $request)) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 60);
} else if (array_key_exists('kwh', $request)) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 60);
} else if (array_key_exists('ah', $request)) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 60);
} else if (array_key_exists('chart', $request)) {
  header('Content-Type: application/javascript');
  header("HTTP/1.1 200 OK");
  $string = file_get_contents(__DIR__."/data.json");
  echo "var chartConfig = " . $string;
  exit();
} else if (array_key_exists('save', $request)) {

  //handle post
  $data = $_POST;
  $charts = json_decode(file_get_contents(__DIR__."/data.json"));
  header('Content-Type: text/html');

  $enabled = array_keys($data['chart']);
  foreach($charts as $index=>$chart) {
    $chart->enabled = in_array($index, $enabled);
    $chart->min = intval($data['min'][$index]);
    $chart->max = intval($data['max'][$index]);
    $chart->updateFrequency = intval($data['frequency'][$index]);
  }

  $fp = fopen('data.json', 'w');
  fwrite($fp, json_encode($charts, JSON_UNESCAPED_UNICODE));
  fclose($fp);

  header('Location: /');
  exit();

}
?>
