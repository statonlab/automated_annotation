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
}
