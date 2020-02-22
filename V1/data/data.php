<?php
$request = $_GET['value'];
if ('p' == $request) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 100);
} else if ('v' == $request) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(100, 300);
} else if ('t' == $request) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 60);
} else if ('kwh' == $request) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 60);
} else if ('status' == $request) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 60);
} else if ('ah' ==  $request) {
  header('Content-Type: text/plain');
  header("HTTP/1.1 200 OK");
  echo rand(0, 60);

} else if ('chart' == $request) {
  header('Content-Type: application/javascript');
  header("HTTP/1.1 200 OK");
  $string = file_get_contents(__DIR__."/data.json");
  echo "var chartConfig = " . $string;
  exit();
} else if ('save' == $request) {

  //handle post
  $data = $_POST;
  $charts = json_decode(file_get_contents(__DIR__."/data.json"));
  header('Content-Type: text/html');

  $enabled = array_keys($data['chart']);
  foreach($charts as $index=>$chart) {
    $chart->type = $data['type'][$index];
    if ($chart->type == 'gauge') {
      $chart->min = intval($data['min'][$index]);
      $chart->max = intval($data['max'][$index]);
    }
    $chart->updateFrequency = intval($data['frequency'][$index]);
  }

  $fp = fopen('data.json', 'w');
  fwrite($fp, json_encode($charts, JSON_UNESCAPED_UNICODE));
  fclose($fp);

  header('Location: /');
  exit();

}
?>
