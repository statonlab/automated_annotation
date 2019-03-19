<?php

namespace AutomatedAnnotation;

use Exception;

class AnnotationFinder {

  /**
   * @param $organism
   *
   * @return array
   * @throws \Exception
   */
  public function blast($organism) {
    $results = [];

    $counts = db_query('SELECT DB.db_id, DB.name, count(*) AS count 
                        FROM chado.blast_hit_data B 
                        INNER JOIN chado.feature F ON F.feature_id = B.feature_id
                        INNER JOIN chado.db DB ON DB.db_id = B.db_id 
                        WHERE F.organism_id = :oid 
                        GROUP BY DB.db_id
                        ', [
      ':oid' => is_object($organism) ? $organism->organism_id : $organism,
    ])->fetchAll();

    foreach ($counts as $count) {
      $results[$count->db_id] = [
        $count->name,
        $count->count,
      ];
    }

    return $results;
  }

  /**
   * @param $organism
   *
   * @return array
   */
  public function featureCVTerm($organism) {
    $organism = is_object($organism) ? $organism->organism_id : $organism;
    $dbs = $this->db(['INTERPRO', 'GO', 'KEGG']);
    $results = [];
    $indexed_dbs = [];
    $db_ids = [];

    foreach ($dbs as $db) {
      $indexed_dbs[$db->db_id] = $db;
      $results[$db->db_id] = [
        $db->name,
        0,
      ];
      $db_ids[] = $db->db_id;
    }

    if (empty($db_ids)) {
      return [];
    }

    foreach ($dbs as $db) {
      $count = db_query('SELECT count(*) FROM chado.feature_cvterm FC
                                    INNER JOIN chado.feature F ON F.feature_id = FC.feature_id
                                    INNER JOIN chado.cvterm CVT ON FC.cvterm_id = CVT.cvterm_id
                                    WHERE organism_id=:oid
                                      AND CVT.cvterm_id IN (
                                      SELECT CVT2.cvterm_id FROM chado.cvterm CVT2
                                         INNER JOIN chado.dbxref DBX ON DBX.dbxref_id = CVT.dbxref_id
                                         WHERE  DBX.db_id = :db_id
                                      )', [
        ':oid' => $organism,
        ':db_id' => $db->db_id,
      ])->fetchObject();

      $results[$db->db_id] = [
        $db->name,
        $count->count,
      ];
    }


    return $results;
  }

  /**
   * Get chado vocabulary DBs.
   *
   * @param $names
   *
   * @return array
   */
  public function db($names) {
    if (!is_array($names)) {
      $names = [$names];
    }

    return db_select('chado.db', 'DB')
      ->fields('DB')
      ->condition('name', $names, 'IN')
      ->execute()
      ->fetchAll();
  }

  /**
   * Get the CVTerm record.
   *
   * @param string|array $name
   *
   * @return array|object A CVTerm record or an array of records.
   * @throws Exception
   */
  public function cvterm($name) {
    $cvterm = db_select('chado.cvterm', 'C');

    $cvterm->fields('C');
    if (is_array($name)) {
      $cvterm->condition('C.name', $name, 'IN');
    }
    else {
      $cvterm->condition('C.name', $name);
    }

    $cvterm = $cvterm->execute();

    if (is_array($name)) {
      $cvterm = $cvterm->fetchAll();
    }
    else {
      $cvterm = $cvterm->fetchObject();
    }

    if (empty($cvterm)) {
      throw new Exception('Unable to find term ' . $name);
    }

    return $cvterm;
  }
}
