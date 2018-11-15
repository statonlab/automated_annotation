<?php

namespace AutomatedAnnotation;

use Exception;

class FeatureFinder {

  /**
   * Get the count of features that have no annotations.
   *
   * @param string $type
   *
   * @return array
   * @throws Exception
   */
  public function generateAnnotationsReport($type = 'mRNA') {
    $cvterm = $this->cvterm($type);

    $cvterm_count = $this->countCVTerm($cvterm);
    $blast_count = $this->countBlast($cvterm);

    return [
      'blast' => $blast_count,
      'cvterm' => $cvterm_count,
    ];
  }

  /**
   * @param string $type
   *
   * @return \SelectQuery
   * @throws \Exception
   */
  public function featureCVTerm($type = 'mRNA') {
    $cvterm = $this->cvterm($type);
    return $this->featureCVTermQuery($cvterm)->fields('F');
  }

  /**
   * @param string $type
   *
   * @return \SelectQuery
   * @throws \Exception
   */
  public function featureBlast($type = 'mRNA') {
    $cvterm = $this->cvterm($type);
    return $this->blastQuery($cvterm)->fields('F');
  }

  /**
   * Query for features that have no cvterm annotations.
   *
   * @param object $cvterm A CVTerm record.
   *
   * @return \SelectQuery
   */
  public function featureCVTermQuery($cvterm) {
    $query = db_select('chado.feature', 'F');
    $query->leftJoin('chado.feature_cvterm', 'FC', 'FC.feature_id = F.feature_id');
    $query->condition('F.type_id', $cvterm->cvterm_id);
    $query->isNull('FC.feature_id');

    return $query;
  }

  /**
   * Query for features that have no blast hit data.
   *
   * @param object $cvterm A CVTerm record.
   *
   * @return \SelectQuery
   */
  public function blastQuery($cvterm = NULL) {
    $query = db_select('chado.feature', 'F');
    $query->leftJoin('chado.blast_hit_data', 'B', 'B.feature_id = F.feature_id');
    if ($cvterm) {
      $query->condition('F.type_id', $cvterm->cvterm_id);
    }
    $query->isNull('B.feature_id');

    return $query;
  }

  /**
   * Count blast hit data.
   *
   * @param string|object $type CVTerm name
   *
   * @return int
   * @throws Exception
   */
  public function countBlast($type = 'mRNA') {
    if (is_string($type)) {
      $cvterm = $this->cvterm($type);
    }
    else {
      $cvterm = $type;
    }
    return (int) $this->blastQuery($cvterm)
      ->countQuery()
      ->execute()
      ->fetchField();
  }

  /**
   * Count CVTerms associated with features.
   *
   * @param string|object $type CVTerm name
   *
   * @return int
   * @throws Exception
   */
  public function countCVTerm($type = 'mRNA') {
    if (is_string($type)) {
      $cvterm = $this->cvterm($type);
    }
    else {
      $cvterm = $type;
    }
    return (int) $this->featureCVTermQuery($cvterm)
      ->countQuery()
      ->execute()
      ->fetchField();
  }

  /**
   * Get the CVTerm record.
   *
   * @param string $name
   *
   * @return object A CVTerm record.
   * @throws Exception
   */
  public function cvterm($name) {
    $cvterm = db_select('chado.cvterm', 'C')
      ->fields('C')
      ->condition('C.name', $name)
      ->execute()
      ->fetchObject();

    if (empty($cvterm)) {
      throw new Exception('Unable to find term ' . $name);
    }

    return $cvterm;
  }
}
