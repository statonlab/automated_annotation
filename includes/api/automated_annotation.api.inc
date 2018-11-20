<?php

/**
 * @return array|mixed
 */
function automated_annotation_get_settings() {
  $default = [
    'emails' => [],
  ];

  $settings = variable_get('annotation_annotation_settings', json_encode($default));
  $settings = json_decode($settings, TRUE);

  return $settings + $default;
}

/**
 * @param $settings
 */
function automated_annotation_set_settings($settings) {
  $default = [
    'emails' => [],
  ];

  variable_set('annotation_annotation_settings', json_encode($settings + $default));
}

/**
 * @return mixed
 * @throws \Exception
 */
function automated_annotation_get_organisms() {
  return chado_query('SELECT * FROM {organism}')->fetchAll();
}
