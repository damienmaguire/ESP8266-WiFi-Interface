<?php
$request = array_key_exists('value', $_GET) ? $_GET['value'] : null;
if ('chart' == $request) {
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

} else {
  $response = new stdClass;
  $response->p = rand(0, 100);
  $response->v = rand(100, 300);
  $response->t = rand(0, 60);
  $response->kwh = rand(0, 60);
  $response->status = rand(0, 60);
  $response->ah = rand(0, 60);
  $response->mph = rand(0, 60);
  header('Content-Type: application/json');
  header("HTTP/1.1 200 OK");
  echo json_encode($response);
  exit();
}
?>
