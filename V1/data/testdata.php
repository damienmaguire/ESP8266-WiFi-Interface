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
}
?>
