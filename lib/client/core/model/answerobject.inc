<?php 
namespace ethereum_reader\rest_client\core\model;


class AnswerObject {
	private $jsonanswer;
	
	private $array = array();
	
	public function getJsonAnswer() {
		return $this->jsonanswer;
	}
	
	public function setJsonAnswer($jsonanswer) {
		$this->jsonanswer = $jsonanswer;
	}
	
	public function getArray() {
		return $this->array;
	}
	
	public function setArray($array) {
		$this->array = $array;
	}
	
	public function add($key, $value) {
		$this->array[$key] = $value;
	}
	
	public function get($key) {
		if (isset($this->array[$key]))
		return $this->array[$key];
	}
	
}