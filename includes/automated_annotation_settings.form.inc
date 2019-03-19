<?php

/**
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function automated_annotation_settings_form($form, &$form_state) {
  $settings = automated_annotation_get_settings();

  $form['emails'] = [
    '#type' => 'textarea',
    '#title' => t('Enter Email Addresses'),
    '#description' => t('Comma separated list of email addresses to send the monthly report to.'),
    '#default_value' => implode(',', $settings['emails']),
    '#required' => TRUE,
  ];

  $form['max_organisms'] = [
    '#type' => 'textfield',
    '#title' => t('Maximum Number of Organisms'),
    '#description' => t('Number of organisms to limit in each produced FASTA file.'),
    '#default_value' => $settings['max_organisms'],
    '#required' => TRUE,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'Save',
  ];

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function automated_annotation_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $emails = explode(',', $values['emails']);
  $max_organisms = $values['max_organisms'];

  array_walk($emails, 'trim');
  $emails = array_filter($emails);

  foreach ($emails as $email) {
    if (!valid_email_address($email)) {
      form_set_error('emails', $email . ' is not a valid email address');
    }
  }

  if (!is_numeric($max_organisms)) {
    form_set_error('max_organisms', 'Max number of organisms field must be a number between 1-100');
  }
  elseif (!intval($max_organisms)) {
    form_set_error('max_organisms', 'Max number of organisms field must be a number between 1-100');
  }
}

/**
 * @param $form
 * @param $form_state
 */
function automated_annotation_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $emails = explode(',', $values['emails']);
  $max_organisms = $values['max_organisms'];

  array_walk($emails, 'trim');
  $emails = array_filter($emails);

  $settings = [
    'emails' => $emails,
    'max_organisms' => $max_organisms,
  ];

  automated_annotation_set_settings($settings);

  drupal_set_message('Settings saved successfully');
}
