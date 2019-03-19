<?php

/**
 * @return array|mixed
 */
function automated_annotation_get_settings() {
  $default = [
    'emails' => [],
    'max_organisms' => 3,
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
    'max_organisms' => 3,
  ];

  variable_set('annotation_annotation_settings', json_encode($settings + $default));
}

/**
 * Get a list of organisms.
 *
 * @param int $limit
 *
 * @return mixed
 * @throws \Exception
 */
function automated_annotation_get_organisms(int $limit = NULL) {
  $limit_clause = '';
  $args = [];
  if ($limit !== NULL) {
    $limit_clause .= ' LIMIT :limit';
    $args[':limit'] = $limit;
  }
  return chado_query('SELECT * FROM {organism} ORDER BY genus ASC, species ASC' . $limit_clause, $args)->fetchAll();
}
