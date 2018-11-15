<?php

function automated_annotation_drush_command() {
  $commands = [];

  $commands['annotations-check'] = [
    'description' => 'Send an email to admins about features missing annotations',
  ];

  return $commands;
}

function drush_automated_annotation_annotations_check() {

}
