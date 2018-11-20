<?php

namespace Tests;

use AutomatedAnnotation\AnnotationFinder;
use StatonLab\TripalTestSuite\DBTransaction;
use StatonLab\TripalTestSuite\TripalTestCase;

class FeatureFinderTest extends TripalTestCase {

  use DBTransaction;

  /**
   * @throws \Exception
   */
  public function testThatCVTermFinderWorks() {
    $finder = new AnnotationFinder();
    $cvterm = $finder->cvterm('mRNA');
    $this->assertNotEmpty($cvterm);
    $this->assertEquals($cvterm->name, 'mRNA');
  }

  /**
   * @throws \Exception
   */
  public function testThatCountingBlastFeaturesIsCorrect() {
    // Create a random cvterm
    $cvterm = factory('chado.cvterm')->create();
    $features = factory('chado.feature', 10)->create([
      'type_id' => $cvterm->cvterm_id,
    ]);

    $finder = new AnnotationFinder();
    $count = $finder->countBlast($cvterm->name);
    $this->assertEquals(count($features), $count);
  }

  /**
   * @throws \Exception
   */
  public function testThatCountingCVTermFeaturesIsCorrect() {
    // Create a random cvterm
    $cvterm = factory('chado.cvterm')->create();
    $features = factory('chado.feature', 10)->create([
      'type_id' => $cvterm->cvterm_id,
    ]);

    $finder = new AnnotationFinder();
    $count = $finder->countCVTerm($cvterm->name);
    $this->assertEquals(count($features), $count);
  }

  /**
   * @throws \Exception
   */
  public function testThatReportGeneratorReturnsCorrectCounts() {
    // Create a random cvterm
    $cvterm = factory('chado.cvterm')->create();
    $features = factory('chado.feature', 10)->create([
      'type_id' => $cvterm->cvterm_id,
    ]);

    $finder = new AnnotationFinder();
    $count = $finder->generateAnnotationsReport($cvterm->name);
    $this->assertEquals(count($features), $count['blast']);
    $this->assertEquals(count($features), $count['cvterm']);
  }
}
