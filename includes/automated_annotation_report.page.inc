<?php

/**
 * @param $form
 * @param $form_state
 *
 * @return mixed
 * @throws \Exception
 */
function automated_annotation_report_form($form, &$form_state) {
  $form['description'] = [
    '#type' => 'item',
    '#markup' => t('Select an organism to generate a report for.'),
  ];

  $organisms = automated_annotation_get_organisms();

  $options = [
    -1 => '-- Select an Organism --',
  ];
  foreach ($organisms as $organism) {
    $options[$organism->organism_id] = $organism->genus . ' ' . $organism->species;
  }

  $form['organism'] = [
    '#title' => t('Organisms'),
    '#type' => 'select',
    '#options' => $options,
    '#ajax' => [
      'callback' => 'automated_annotation_report_form_callback',
      'wrapper' => 'ajax_response',
    ],
  ];

  $form['ajax_response'] = [
    '#type' => 'markup',
    '#prefix' => '<div id="ajax_response">',
    '#suffix' => '</div>',
  ];

  if (isset($form_state['values']) && isset($form_state['values']['organism'])) {
    $finder = new \AutomatedAnnotation\AnnotationFinder();
    $organism_id = $form_state['values']['organism'];
    $blast = $finder->blast($organism_id);
    $terms = $finder->featureCVTerm($organism_id);
    $cvterms = [];

    foreach ($blast as $item) {
      $cvterms[] = [
        "BLAST: {$item[0]}",
        $item[1],
      ];
    }

    foreach ($terms as $item) {
      $cvterms[] = [
        $item[0],
        $item[1],
      ];
    }

    $form['ajax_response']['report'] = [
      '#type' => 'markup',
      '#markup' => theme('table', [
        'header' => ['Vocabulary', 'Count'],
        'rows' => $cvterms,
      ]),
    ];
  }

  return $form;
}

/**
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function automated_annotation_report_form_callback($form, &$form_state) {
  return $form['ajax_response'];
}
