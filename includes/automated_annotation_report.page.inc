<?php

function automated_annotation_report_page() {
  $finder = new \AutomatedAnnotation\FeatureFinder();


  $query = $finder->blastQuery('gene');
  $query->fields('F', ['organism_id', 'COUNT(*)']);
  $query->fields('CVT', ['name']);
  $query->groupBy('F.organism_id');
  $query->groupBy('CVT.name');
  $query->innerJoin('chado.cvterm', 'CVT', 'CVT.cvterm_id = F.type_id');
  $query->innerJoin('chado.organism', 'O', 'O.organism_id = F.organism_id');
  $query->having('COUNT(*) > 1');

  $all = $query->execute()->fetchAll();

  $organism_ids = array_map(function ($item) {
    return $item->organism_id;
  }, $all);

  $organisms = db_select('chado.organism', 'CO')
    ->fields('CO')
    ->condition('organism_id', $organism_ids, 'IN')
    ->execute()
    ->fetchAll();

  $indexed_organisms = [];
  foreach ($organisms as $organism) {
    $indexed_organisms[$organism->organism_id] = $organism;
  }

  foreach ($all as &$item) {
    $item->organism = $indexed_organisms[$item->organism_id];
  }

  dpm($all);

  return '';
}
