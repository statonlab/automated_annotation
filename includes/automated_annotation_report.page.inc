<?php

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
//    $blast = $finder->blast($organism_id);
    $terms = $finder->featureCVTerm($organism_id);

//    dpm($blast);
    dpm($terms);

    $cvterms = [];
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

function automated_annotation_report_form_callback($form, &$form_state) {
  return $form['ajax_response'];
}

function automated_annotation_make_cvterms_rows($organism_id = NULL) {
  $cvterm_counts = db_query('SELECT count(*), DB.db_id FROM chado.feature_cvterm FC 
                                    INNER JOIN chado.feature F ON F.feature_id = FC.feature_id
                                    INNER JOIN chado.cvterm CVT ON FC.cvterm_id = CVT.cvterm_id
                                    INNER JOIN chado.dbxref DBX ON DBX.dbxref_id = CVT.dbxref_id
                                    INNER JOIN chado.db DB ON DB.db_id = DBX.db_id
                                    WHERE organism_id=:oid
                                    GROUP BY DB.db_id', [
    ':oid' => $organism_id,
  ])->fetchAll();

  $cvterms = [];
  if (empty($cvterm_counts)) {
    return $cvterms;
  }

  $db_ids = [];
  foreach ($cvterm_counts as $count) {
    $db_ids[$count->db_id] = $count->db_id;
  }

  $dbs = db_query('SELECT * FROM chado.db WHERE db_id IN (:ids)', [':ids' => $db_ids])->fetchAll();
  $indexed_terms = [];
  foreach ($dbs as $term) {
    $indexed_terms[$term->db_id] = $term;
  }

  foreach ($cvterm_counts as $count) {
    $cvterms[] = [
      $indexed_terms[$count->db_id]->name,
      $count->count,
    ];
  }

  return $cvterms;
}
