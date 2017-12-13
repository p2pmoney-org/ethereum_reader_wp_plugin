<?php 
namespace ethereum_reader\rest_client\core\model;


class AnswerObject {
	private $jsonanswer;
	
	private $metadata = array();
	
	private $array = array(); // data within data section
	
	// full json answer from remote server
	// can be forwarded if necessary
	public function getJsonAnswer() {
		return $this->jsonanswer;
	}
	
	public function setJsonAnswer($jsonanswer) {
		$this->jsonanswer = $jsonanswer;
	}
	
	// meta data returned (e.g. status, highestblock,..)
	public function getMetaDataArray() {
		return $this->metadata;
	}
	
	public function setMetaDataArray($array) {
		$this->metadata = $array;
	}
	
	public function addMetaData($key, $value) {
		$this->metadata[$key] = $value;
	}
	
	public function getMetaData($key) {
		if (isset($this->metadata[$key]))
		return $this->metadata[$key];
	}
	
	// data returned
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
	
	public function has($value) {
		if (in_array($value, $this->array))
			return true;
		else
			return false;
	}
	
}