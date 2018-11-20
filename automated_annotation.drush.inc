<?php

/**
 * @return array
 */
function automated_annotation_drush_command() {
  $commands = [];

  $commands['annotations-check'] = [
    'description' => 'Send an email to admins about features missing annotations',
  ];

  return $commands;
}

/**
 * @throws \Exception
 */
function drush_automated_annotation_annotations_check() {
  automated_annotation_perform_checks();
}
