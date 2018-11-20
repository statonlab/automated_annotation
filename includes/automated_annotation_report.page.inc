<?php

function automated_annotation_report_form($form, &$form_state) {
  $form['description'] = [
    '#type' => 'item',
    '#markup' => t('Select an organism to generate a report for.'),
  ];

  $organisms = automated_annotation_get_organisms();

  $options = [
    0 => '-- Select an Organism --',
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
    $organism_id = $form_state['values']['organism'];

    $blast_count = (int) db_query('SELECT count(*) FROM chado.blast_hit_data B 
                                    INNER JOIN chado.feature F ON F.feature_id = B.feature_id
                                    WHERE F.organism_id = :oid', [':oid' => $organism_id])->fetchField();

    $cvterms = automated_annotation_make_cvterms_rows($organism_id);
//    $cvterms[] = ['Blast Hit Data', $blast_count];

    $form['ajax_response']['report'] = [
      '#type' => 'markup',
      '#markup' => theme('table', [
        'header' => ['Organism', 'Vocabulary', 'Count'],
        'rows' => $cvterms,
      ]),
    ];
  }

  return $form;
}

function automated_annotation_report_form_validate($form, &$form_state) {

}

function automated_annotation_report_submit($form, &$form_state) {

}

function automated_annotation_report_form_callback($form, &$form_state) {
  return $form['ajax_response'];
}

function automated_annotation_make_cvterms_rows($organism_id) {
  $cvterm_counts = db_query('SELECT count(*), DB.db_id, F.organism_id FROM chado.feature_cvterm FC 
                                    INNER JOIN chado.feature F ON F.feature_id = FC.feature_id
                                    INNER JOIN chado.cvterm CVT ON FC.cvterm_id = CVT.cvterm_id
                                    INNER JOIN chado.dbxref DBX ON DBX.dbxref_id = CVT.dbxref_id
                                    INNER JOIN chado.db DB ON DB.db_id = DBX.db_id
                                    GROUP BY F.organism_id, DB.db_id
                                    ORDER BY F.organism_id ASC', [
    ':oid' => $organism_id,
  ])->fetchAll();

  $cvterms = [];
  if (empty($cvterm_counts)) {
    return $cvterms;
  }

  $db_ids = [];
  $organism_ids = [];

  foreach ($cvterm_counts as $count) {
    $db_ids[$count->db_id] = $count->db_id;
    $organism_ids[$count->organism_id] = $count->organism_id;
  }

  $dbs = db_query('SELECT * FROM chado.db WHERE db_id IN (:ids)', [':ids' => $db_ids])->fetchAll();
  $indexed_terms = [];
  foreach ($dbs as $term) {
    $indexed_terms[$term->db_id] = $term;
  }

  $organisms = db_query('SELECT * FROM chado.organism WHERE organism_id IN (:ids)', [':ids' => $organism_ids])->fetchAll();
  $indexed_organisms = [];
  foreach ($organisms as $organism) {
    $indexed_organisms[$organism->organism_id] = $organism;
  }
  foreach ($cvterm_counts as $count) {
    $organism = $indexed_organisms[$count->organism_id];
    $organism_name = "$organism->genus $organism->species ($organism->common_name)";
    $cvterms[] = [$organism_name, $indexed_terms[$count->db_id]->name, $count->count];
  }

  return $cvterms;
}
