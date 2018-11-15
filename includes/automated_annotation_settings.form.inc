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

  array_walk($emails, 'trim');
  $emails = array_filter($emails);

  foreach ($emails as $email) {
    if (!valid_email_address($email)) {
      form_set_error('emails', $email . ' is not a valid email address');
    }
  }
}

/**
 * @param $form
 * @param $form_state
 */
function automated_annotation_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $emails = explode(',', $values['emails']);

  array_walk($emails, 'trim');
  $emails = array_filter($emails);

  $settings = [
    'emails' => $emails,
  ];

  automated_annotation_set_settings($settings);

  drupal_set_message('Settings saved successfully');
}
