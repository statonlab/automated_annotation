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
    'description' => 'temporary function'
  ];

  $commands['get-dbs'] =[
    'description' => 'tmp fn'
  ];

  $commands['get-organisms'] =[
    'description' => 'tmp fn'
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
  $dbs = db_query('SELECT distinct b.db_id, d.name FROM chado.blast_hit_data b inner join chado.db d on b.db_id = d.db_id')->fetchAll();
  var_dump($dbs);

  $orgs = db_query('SELECT distinct o.organism_id, o.common_name, genus, specis 
            from chado.blast_hit_data b 
            inner join chado.feature f on f.feature_id = b.feature_id
            inner join chado.organism o on f.organism_id = o.organism_id
            ')->fetchAll();

  var_dump($orgs);
}

function drush_automated_annotation_clear_blast_db() {
  // Get options
  $organism_id = drush_get_option('organism_id');
  if(is_null($organism_id)) {
    throw new \Exception("Please provide an organism id using the --organism_id=[ID] option");
  }

  $old_db_id = drush_get_option('old_db_id');
  if(is_null($old_db_id)) {
    throw new \Exception("Please provide an old db id using the --old_db_id=[ID] option");
  }

  $new_db_id = drush_get_option('new_db_id');
  if(is_null($new_db_id)) {
    throw new \Exception("Please provide an new db id using the --db_id=[ID] option");
  }
}
