<?php

namespace AutomatedAnnotation;

abstract class Dumper {

  /**
   * @var bool|resource
   */
  protected $output_file;

  /**
   * @var string
   */
  protected $output_path;

  /**
   * Dumper constructor.
   *
   * @param string $output_path
   *
   * @throws \Exception
   */
  public function __construct($output_path = NULL) {
    $this->output_file = FALSE;
    if ($output_path) {
      $this->output_file = fopen($output_path, 'w');
      if (!$this->output_file) {
        throw new \Exception('Unable to open file at ' . $output_path . '.');
      }
      $this->output_path = $output_path;
    }
  }

  /**
   * Write a line to the file.
   *
   * @param $line
   */
  public function write($line) {
    if (!$this->output_file) {
      echo $line . "\n";
    }
    else {
      fputs($this->output_file, "{$line}\n");
    }

    return $this;
  }

  /**
   * Do the dumping.
   *
   * @return mixed
   */
  abstract public function dump();

  /**
   * Output file path.
   *
   * @return string
   */
  public function file() {
    return $this->output_path;
  }
}
