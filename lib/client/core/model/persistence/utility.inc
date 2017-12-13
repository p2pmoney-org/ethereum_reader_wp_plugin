<?php 
namespace ethereum_reader\rest_client\core\model;


class Utility {
	static protected function key_encode($key) {
		$jsonpart = '';
		
		if (!isset($key)) {
			return $jsonpart;
		}
		else {
			if (!is_numeric($key)) {
				$jsonpart .= '"'.$key.'":';
			}
					
		}
		return $jsonpart;
	}
	
	static protected function key_value_encode($key, $value) {
		$json = '';
		
		$json .= Utility::key_encode($key);
		
		if (is_array($value)) {
			$json .= Utility::json_string_encode($value).',';
		}
		else {
			if (is_numeric($value)) {
				$stringval = strval($value);
		
				if (strpos($stringval, '0x')===0) {
					// json string
					$json .= '"'.$value.'",';
				}
				else {
					$json .= ''.$value.',';
				}
			}
			else  {
				if ((strpos($value, '{')===0) || (strpos($value, '[')===0)){
					// json string
					$json .= $value.',';
				}
				else {
					$json .= '"'.$value.'",';
				}
			}
		
		}
		
		return $json;
	}
	
	
	static protected function data_encode($data) {
		$json = '';
		
		if (is_array($data)) {
			foreach($data as $key=>$value) {
				$json .= Utility::key_value_encode($key, $value);
			}
		}
		else {
			$key = null;
			$value = $data;
			
			$json .= Utility::key_value_encode($key, $value);
		}
			
			
		
		return $json;
	}
	
	static public function json_array_encode($data) {
		$json = '[';
		
		$json .= Utility::data_encode($data);
		
		$json .= ']';
				
		return $json;
	}
	
	static public function json_string_encode($data) {
		// encode like javascript
		$json = '{';
		
		$json .= Utility::data_encode($data);
		
		$json .= '}';
				
		return $json;
	}
}