<?php
$request = $_GET;

header('Content-Type: text/plain');
header("HTTP/1.1 200 OK");

if (array_key_exists('power', $request)) {
  header("HTTP/1.1 200 OK");
  echo rand(0, 100);
} else if (array_key_exists('voltage', $request)) {
  echo rand(100, 300);
} else if (array_key_exists('temperature', $request)) {
  echo rand(0, 60);
} else {

  //handle post
  $data = $_POST;
  $string = file_get_contents(__DIR__."/config-dev.js");
  preg_match('/charts: (\[{\X*}\])/m', $string, $matches);
  header('Content-Type: text/html');

  $charts = json_decode($matches[1]);

  $enabled = array_keys($data['chart']);
  foreach($charts as $index=>$chart) {
    $chart->enabled = in_array($index, $enabled);
    $chart->min = intval($data['min'][$index]);
    $chart->max = intval($data['max'][$index]);
    $chart->updateFrequency = intval($data['frequency'][$index]);
  }

  $chartStr = json_encode($charts);
  $configStr = preg_replace( '/charts: (\[{\X*}\])/m', 'charts: ' . $chartStr, $string );
  file_put_contents(__DIR__."/config-dev.js", $configStr);
  header('Location: /');
  exit();

}
?>
