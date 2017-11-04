<?php 
namespace ethereum_reader\rest_client\core\model;

require_once('block.inc');

class Blocks extends AnswerObject {
	private $block_array = array();
	
	public function getBlockArray() {
		return $this->block_array;
	}
	
	public function addBlock($block) {
		if (($block instanceof Block) === false)
			return;
		
		$count = count($this->block_array);
		
		$i = $count;
		
		$this->block_array[$i] = $block;
	}
	
}
