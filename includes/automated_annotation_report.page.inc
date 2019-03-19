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
    $options[$organism->organism_id] =
      $organism->genus . ' ' . $organism->species;
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

  if (isset($form_state['values'])
    && isset($form_state['values']['organism'])
  ) {
    $finder = new \AutomatedAnnotation\AnnotationFinder();
    $organism_id = $form_state['values']['organism'];
    $blast_time_start = time();
    $blast = $finder->blast($organism_id);
    $blast_time_end = time();
    $terms_time_start = time();
//    $terms = $finder->featureCVTerm($organism_id);
    $terms_time_end = time();
    $cvterms = [];

    foreach ($blast as $item) {
      $cvterms[] = [
        "BLAST: {$item[0]}",
        $item[1],
      ];
    }

//    foreach ($terms as $item) {
//      $cvterms[] = [
//        $item[0],
//        $item[1],
//      ];
//    }

    $blast_time = $blast_time_end - $blast_time_start;
    $term_time = $terms_time_end - $terms_time_start;
    $total_time = $term_time + $blast_time;
    $form['ajax_response']['time'] = [
      '#type' => 'item',
      '#markup' => t(
        "Took $total_time seconds to generate this report. Spent $blast_time seconds to obtain BLAST information and $term_time seconds to generate the rest."
      ),
    ];

    $form['ajax_response']['report'] = [
      '#type' => 'markup',
      '#markup' => theme(
        'table', [
          'header' => ['Vocabulary', 'Count'],
          'rows' => $cvterms,
        ]
      ),
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
