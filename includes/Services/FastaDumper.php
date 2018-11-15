<?php

namespace AutomatedAnnotation;

class FastaDumper extends Dumper {

  /**
   * Organism record.
   *
   * @var object
   */
  protected $organism;

  /**
   * CVTerm record.
   *
   * @var object
   */
  protected $type;

  /**
   * Set an organism.
   *
   * @param object $organism
   */
  public function organism($organism) {
    $this->organism = $organism;
  }

  /**
   * Set a list of organisms.
   *
   * @param array $organisms
   */
  public function organisms(array $organisms) {
    $this->organism = $organisms;
  }

  /**
   * Set the type of sequences.
   *
   * @param object $cvterm A CVTerm record.
   */
  public function type($cvterm) {
    $this->type = $cvterm;
  }

  /**
   * Print a fasta line.
   *
   * @param object $feature A feature object.
   */
  protected function fastaLine($feature) {
    $this->write(">{$feature->uniquename}");
    $this->write($feature->residue);
  }

  /**
   * Get features chunk.
   *
   * @param $start
   * @param $size
   *
   * @return mixed
   */
  protected function features($start, $size) {
    return $this->query()->range($start, $size)->execute()->fetchAll();
  }

  /**
   * Get the query.
   *
   * @return \SelectQuery
   */
  protected function query() {
    if (is_array($this->organism)) {
      $organisms = array_map(function ($organism) {
        return $organism->organism_id;
      }, $this->organism);
    }
    else {
      $organisms = [$this->organism->organism_id];
    }

    $query = db_select('chado.feature', 'F');
    $query->fields('F');
    if ($this->type) {
      $query->condition('F.type_id', $this->type->cvterm_id);
    }
    if ($this->organism) {
      $query->condition('F.organism_id', $organisms, 'IN');
    }

    return $query;
  }

  /**
   * @return int
   */
  protected function count() {
    return (int) $this->query()->countQuery()->execute()->fetchField();
  }

  /**
   * Start printing fasta lines to the given file.
   *
   * @return mixed|void
   */
  public function dump() {
    $total = $this->count();
    $chunk_size = 1000;

    for ($start = 0; $start <= $total; $start += $chunk_size) {
      $features = $this->features($start, $chunk_size);
      foreach ($features as $feature) {
        $this->fastaLine($feature);
      }
    }
  }
}
