<?php

/**
 * @return array
 */
function automated_annotation_drush_command() {
  $commands = [];

  $commands['annotations-check'] = [
    'description' => 'Send an email to admins about features missing annotations',
  ];

  $commands['clear-blast-db'] = [
    'description' => 'temporary function',
    'options' => [
      'organism_id' => 'o1',
      'old_db_id' => 'o2',
      'new_db_id' => 'o3',
    ],
  ];

  $commands['get-dbs'] = [
    'description' => 'tmp fn',
  ];

  return $commands;
}

/**
 * @throws \Exception
 */
function drush_automated_annotation_annotations_check() {
  automated_annotation_perform_checks();
}

function drush_automated_annotation_get_dbs() {
  $dbs = db_query('SELECT distinct d.db_id, name FROM chado.db d
                    INNER JOIN chado.blast_hit_data b ON b.db_id = d.db_id 
                    WHERE name NOT IN (:names)', [
    ':names' => ['UNIPROT/TREMBL', 'Swiss-Prot'],
  ])->fetchAll();

  $db_ids = array_map(function ($db) {
    return $db->db_id;
  }, $dbs);

  if(empty($db_ids)) {
    print "All is good here!!\n";
    return;
  }

  $orgs = db_query('select distinct o.organism_id, o.common_name, genus, species 
            from chado.blast_hit_data b 
            inner join chado.feature f on f.feature_id = b.feature_id
            inner join chado.organism o on f.organism_id = o.organism_id
            where b.db_id in (:dbs)', [
    ':dbs' => $db_ids,
  ])->fetchAll();

  foreach ($orgs as $org) {
    $all = db_query('select distinct b.db_id, d.name from chado.blast_hit_data b inner join chado.feature f on f.feature_id = b.feature_id inner join chado.db d on d.db_id = b.db_id where f.organism_id = :org_id', [
      ':org_id' => $org->organism_id,
    ])->fetchAll();

    echo "$org->organism_id: $org->common_name ($org->genus $org->species) has the following databases:\n";
    foreach ($all as $item) {
      echo "ID: $item->db_id, NAME: $item->name\n";
    }
    echo "\n";
  }
}

function drush_automated_annotation_clear_blast_db() {
  // Get options
  $organism_id = drush_get_option('organism_id');
  if (is_null($organism_id)) {
    throw new \Exception("Please provide an organism id using the --organism_id=[ID] option");
  }

  $old_db_id = drush_get_option('old_db_id');
  if (is_null($old_db_id)) {
    throw new \Exception("Please provide an old db id using the --old_db_id=[ID] option");
  }

  $new_db_id = drush_get_option('new_db_id');
  if (is_null($new_db_id)) {
    throw new \Exception("Please provide an new db id using the --db_id=[ID] option");
  }

  $t = db_transaction();
  try {
    print "Starting DB conversion for organism $organism_id, db $old_db_id to $new_db_id\n";

    db_query('UPDATE chado.blast_hit_data 
            SET db_id = :new_db_id 
            WHERE db_id = :old_db_id
              AND feature_id IN (
                SELECT feature_id FROM chado.feature WHERE organism_id = :oid
              )', [
      ':oid' => $organism_id,
      ':old_db_id' => $old_db_id,
      ':new_db_id' => $new_db_id,
    ])->fetchAll();

    print "Success!\n";
  } catch (\Exception $exception) {
    $t->rollback();
    print "ERROR: " . $exception->getMessage() . "\n";
  }
}
