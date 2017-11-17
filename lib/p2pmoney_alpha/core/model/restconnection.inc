<?php
namespace p2pmoney\common\core\model;

include_once('global.inc');

use Exception;

class RestException extends Exception {

}

class RestConnection {
	
	private $context = array();
	private $bcleanurl = false;
	private $parameter = 'path';
	
	private $extraheaders = array();
	
	protected function getApiContext() {

		$context = array();

		$context['base_url'] = $this->context['base_url'];
		$context['access_token'] = $this->context['access_token'];
		$context['domain'] = $this->context['domain'];
		$context['accept'] = $this->context['accept'];
		$context['contenttype'] = $this->context['contenttype'];
		
		return $context;
	}
	
	public function getName() {
		return $this->context['base_url'];
	}
	
	public function getUserName() {
		return $this->context['domain'];
	}
	
	public function getPassword() {
		return $this->context['access_token'];
	}
	
	public function getServiceUrl($resourcearray) {
		$context = $this->getApiContext();
		
		$path = RestConnection::makePath($resourcearray);
		
		if (!$this->bcleanurl)
			$service_url = $context['base_url'].'?'. $this->parameter.'='.$context['domain'].'/'.$path;
		else
			$service_url = $context['base_url'].$context['domain'].'/'.$path;
				
		
		return $service_url; 
	}
	
	public function setCleanUrl($bcleanurl) {
		$this->bcleanurl = $bcleanurl;
	}
	
	public function setRequestParameterName($name) {
		$this->parameter = $name;
	}
	
	public function setBaseURL($url) {
		$this->context['base_url'] = $url;
	}
	
	public function setAccessToken($token) {
		$this->context['access_token'] = $token;
	}
	
	public function setDomain($domain) {
		$this->context['domain'] = $domain;
	}
	
	public function setApplicationType($accept) {
		$this->context['accept'] = $accept;
	}
	
	public function setContentType($contenttype) {
		$this->context['contenttype'] = $contenttype;
	}
	
	public function setExtraHeader($key, $value) {
		$this->extraheaders[$key] = $value;
	}
	
	
	protected function getHeaders($sessiontoken, $resourcearray) {
		$context = $this->getApiContext();
		
		$secret = (isset($context['access_token']) ? $context['access_token'] : null);
		$salt = (isset($context['domain']) ? $context['domain'] :'').'/'.RestConnection::makePath($resourcearray);
		$pepper = $secret;
		
		$accesstoken = (isset($secret) ? EncryptionUtils::getHashedPhrase($secret, $salt, $pepper) : 'none');
	
		$headers = array();
		$headers[] = 'sessiontoken:' .  (isset($sessiontoken) ? $sessiontoken : 'none');
		$headers[] = 'accesstoken:' .  (isset($accesstoken) ? $accesstoken : 'none');
		
		$accept = (isset($context['accept']) ? $context['accept'] : null);
		$contenttype = (isset($context['contenttype']) ? $context['contenttype'] : null);
		
		$headers[] = 'Accept:'.(isset($accept) ? $accept : '');
		$headers[] = 'Content-Type:'.(isset($contenttype) ? $contenttype : '');
		
		// extra headers
		$extraheaders = $this->extraheaders;
		
		foreach($extraheaders as $key=>$value) {
			$headers[] = $key.':'.$value;
		}
	
		return $headers;
	}
	
	public function curlGetCall($resourcearray,  $expectedhttpcode, $sessiontoken, &$jsonanswer = null) {
		
		$headers = $this->getHeaders($sessiontoken, $resourcearray);
		$service_url = $this->getServiceUrl($resourcearray);
		
		P2PMoneyAlphaGlobals::logString('RestConnection::curlGetCall called with service url '.$service_url);
		
		
		$curl = curl_init($service_url);
	
		if ($curl === false) {
			$info = curl_getinfo($curl);
	
			curl_close($curl);
	
			$erromessage = 'failed to initialize'."\n".' Additional info: ' . RestConnection::printAdditionalInfo($info)."\n";
	
			throw new RestException($erromessage);
	
		}
	
	
	
		// headers
		curl_setopt($curl, CURLOPT_HTTPHEADER, $headers); // custom header in call
		curl_setopt($curl, CURLOPT_HEADER, true); // get header in response
			
		// return content in response
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	
		// executing call
		$curl_response = curl_exec($curl);
	
		if ($curl_response === false) {
	
			$info = curl_getinfo($curl);
	
			curl_close($curl);
	
			$erromessage = 'error occurred during curl exec.'."\n".' Additional info: ' . RestConnection::printAdditionalInfo($info)."\n";
	
			throw new RestException($erromessage);
	
		}
	
		$httpcode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
	
		// log call
		$curl_post_data = null;
		
		RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
		if ($httpcode != $expectedhttpcode) {
	
			$erromessage = 'http error code'."\n";
			$curl_post_data = null;
			$erromessage .= RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			curl_close($curl);
	
			throw new RestException($erromessage) ;
		}
	
	
		$response_header_size = curl_getinfo($curl, CURLINFO_HEADER_SIZE);
		$response_header = substr($curl_response, 0, $response_header_size);
		$response_body = substr($curl_response, $response_header_size);
			
		$decoded = json_decode($response_body, true);
		
		if (isset($jsonanswer)) {
			// caller ask to also get the raw answer
			$jsonanswer = $response_body;
		}
	
	
		if (isset($decoded->response->status) && $decoded->response->status == 'ERROR') {
	
			$errormessage = 'error occurred: ' . $decoded->response->errormessage."\n";
	
			// close curl handle
			curl_close($curl);
	
			throw new RestException($errormessage);
	
		}
	
		if (!isset($decoded)) {
			$erromessage = 'could not decode body'."\n";
	
			$curl_post_data = null;
			$erromessage .= RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			// close curl handle
			//curl_close($curl);
	
			//throw new RestException($erromessage) ;
		}
	
		// close curl handle
		curl_close($curl);
	
		return $decoded;
	}
	
	public function curlPostCall($resourcearray, $input_data, $expectedhttpcode, $sessiontoken, &$jsonanswer = null) {
		$headers = $this->getHeaders($sessiontoken, $resourcearray);
		$service_url = $this->getServiceUrl($resourcearray);
		
		P2PMoneyAlphaGlobals::logString('RestConnection::curlPostCall called with service url '.$service_url);
		
		// POST
		$curl_post_data = $input_data;
		
		$curl = curl_init($service_url);
	
		// headers
		curl_setopt($curl, CURLOPT_HTTPHEADER, $headers); // custom header in call
		curl_setopt($curl, CURLOPT_HEADER, true); // get header in response
	
	
		// return content in response
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	
	
		// post
		curl_setopt($curl, CURLOPT_POST, true);
		curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
			
		//curl_setopt($curl, CURLOPT_VERBOSE, true);
	
		$curl_response = curl_exec($curl);
	
		if (curl_errno($curl)){
			$erromessage = 'error occured during curl exec. error is: ' . curl_error($curl);
			throw new RestException($erromessage);
		}
			
		if ($curl_response === false) {
	
			$info = curl_getinfo($curl);
	
			curl_close($curl);
	
			$erromessage = 'error occured during curl exec. Additional info: ' . RestConnection::printAdditionalInfo($info);
	
			throw new RestException($erromessage);
	
		}
	
		$httpcode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
	
		// log call
		//RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
		if ($httpcode != $expectedhttpcode) {
	
			$erromessage = RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			curl_close($curl);
	
			throw new RestException($erromessage) ;
		}
	
		$response_header_size = curl_getinfo($curl, CURLINFO_HEADER_SIZE);
		$response_header = substr($curl_response, 0, $response_header_size);
		$response_body = substr($curl_response, $response_header_size);
			
		$decoded = json_decode($response_body, true);

		if (isset($jsonanswer)) {
			// caller ask to also get the raw answer
			$jsonanswer = $response_body;
		}
		
		if (!isset($decoded)) {
			$erromessage = 'could not decode body'."\n";
			$erromessage .= RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			//curl_close($curl);
	
			//throw new RestException($erromessage) ;
		}
	
		// everything went ok
		curl_close($curl);
	
		if (isset($decoded->response->status) && $decoded->response->status == 'ERROR') {
	
			$erromessage = 'error occured: ' . $decoded->response->errormessage;
	
			throw new RestException($erromessage);
	
		}
	
		return $decoded;
	}
	
	
	public function curlPutCall($resourcearray, $input_data, $expectedhttpcode, $sessiontoken, &$jsonanswer = null) {
		$headers = $this->getHeaders($sessiontoken, $resourcearray);
		$service_url = $this->getServiceUrl($resourcearray);
		
		P2PMoneyAlphaGlobals::logString('RestConnection::curlPutCall called with service url '.$service_url);
		
		// POST
		$curl_post_data = $input_data;
		
		$curl = curl_init($service_url);
	
		// headers
		curl_setopt($curl, CURLOPT_HTTPHEADER, $headers); // custom header in call
		curl_setopt($curl, CURLOPT_HEADER, true); // get header in response
	
	
		// return content in response
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	
	
		// putt
		curl_setopt($curl, CURLOPT_CUSTOMREQUEST, 'PUT');
		//curl_setopt($curl, CURLOPT_POST, true);
		curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
			
		//curl_setopt($curl, CURLOPT_VERBOSE, true);
	
		$curl_response = curl_exec($curl);
	
		if (curl_errno($curl)){
			$erromessage = 'error occured during curl exec. error is: ' . curl_error($curl);
			throw new RestException($erromessage);
		}
			
		if ($curl_response === false) {
	
			$info = curl_getinfo($curl);
	
			curl_close($curl);
	
			$erromessage = 'error occured during curl exec. Additional info: ' . RestConnection::printAdditionalInfo($info);
	
			throw new RestException($erromessage);
	
		}
	
		$httpcode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
	
		// log call
		//RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
		if ($httpcode != $expectedhttpcode) {
	
			$erromessage = RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			curl_close($curl);
	
			throw new RestException($erromessage) ;
		}
	
		$response_header_size = curl_getinfo($curl, CURLINFO_HEADER_SIZE);
		$response_header = substr($curl_response, 0, $response_header_size);
		$response_body = substr($curl_response, $response_header_size);
			
		$decoded = json_decode($response_body, true);

		if (isset($jsonanswer)) {
			// caller ask to also get the raw answer
			$jsonanswer = $response_body;
		}
		
		if (!isset($decoded)) {
			$erromessage = 'could not decode body'."\n";
			$erromessage .= RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			//curl_close($curl);
	
			//throw new RestException($erromessage) ;
		}
	
		// everything went ok
		curl_close($curl);
	
		if (isset($decoded->response->status) && $decoded->response->status == 'ERROR') {
	
			$erromessage = 'error occured: ' . $decoded->response->errormessage;
	
			throw new RestException($erromessage);
	
		}
	
		return $decoded;
	}
	
	
	public function curlDeleteCall($resourcearray, $expectedhttpcode, $sessiontoken, &$jsonanswer = null) {
		$headers = $this->getHeaders($sessiontoken, $resourcearray);
		$service_url = $this->getServiceUrl($resourcearray);
		
		P2PMoneyAlphaGlobals::logString('RestConnection::curlDeleteCall called with service url '.$service_url);
		
		// POST
		$curl_post_data = $input_data;
		
		$curl = curl_init($service_url);
	
		// headers
		curl_setopt($curl, CURLOPT_HTTPHEADER, $headers); // custom header in call
		curl_setopt($curl, CURLOPT_HEADER, true); // get header in response
	
	
		// return content in response
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	
	
		// delete
		curl_setopt($curl, CURLOPT_CUSTOMREQUEST, 'DELETE');
			
		//curl_setopt($curl, CURLOPT_VERBOSE, true);
	
		$curl_response = curl_exec($curl);
	
		if (curl_errno($curl)){
			$erromessage = 'error occured during curl exec. error is: ' . curl_error($curl);
			throw new RestException($erromessage);
		}
			
		if ($curl_response === false) {
	
			$info = curl_getinfo($curl);
	
			curl_close($curl);
	
			$erromessage = 'error occured during curl exec. Additional info: ' . RestConnection::printAdditionalInfo($info);
	
			throw new RestException($erromessage);
	
		}
	
		$httpcode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
	
		// log call
		//RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
		if ($httpcode != $expectedhttpcode) {
	
			$erromessage = RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			curl_close($curl);
	
			throw new RestException($erromessage) ;
		}
	
		$response_header_size = curl_getinfo($curl, CURLINFO_HEADER_SIZE);
		$response_header = substr($curl_response, 0, $response_header_size);
		$response_body = substr($curl_response, $response_header_size);
			
		$decoded = json_decode($response_body, true);

		if (isset($jsonanswer)) {
			// caller ask to also get the raw answer
			$jsonanswer = $response_body;
		}
		
		if (!isset($decoded)) {
			$erromessage = 'could not decode body'."\n";
			$erromessage .= RestConnection::printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data);
	
			//curl_close($curl);
	
			//throw new RestException($erromessage) ;
		}
	
		// everything went ok
		curl_close($curl);
	
		if (isset($decoded->response->status) && $decoded->response->status == 'ERROR') {
	
			$erromessage = 'error occured: ' . $decoded->response->errormessage;
	
			throw new RestException($erromessage);
	
		}
	
		return $decoded;
	}
	
	
	// static
	static public function makePath($resourcearray) {
		$bFirst = true;
		$path = '';
		
		foreach($resourcearray as $token) {
			$path .= ($bFirst ? '' : '/');
				
			$path .= $token;
				
			$bFirst = false;
		}
		
		return $path;
	}
	
	static public function printAdditionalInfo($info) {
		return print_r($info, true);;//var_export($info);
	}
	
	static public function printCurlCall($service_url, $headers, $curl, $curl_response, $curl_post_data) {
		$httpcode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		$errormessage = 'http response is: '.$httpcode."\n";
			
		$header_size = curl_getinfo($curl, CURLINFO_HEADER_SIZE);
		$header = substr($curl_response, 0, $header_size);
		$body = substr($curl_response, $header_size);
			
		$decoded = json_decode($body, true);
			
		$headerstring = '';
		$i = 0;
		
		while (isset($headers[$i])) {
			$headerstring .= $headers[$i].' ';
			$i++;
		}
			
		$errormessage .= 'call service url is: '.$service_url."\n";
		$errormessage .= 'call header is: '.$headerstring."\n"; // security cautious
		$errormessage .= 'call post/put data is: '.print_r($curl_post_data, true)."\n";
		$errormessage .= 'response header is: '.$header."\n";
		$errormessage .= 'response body is: '.$body."\n";
		$errormessage .= 'decoded json is:'.print_r($decoded, true)."\n";
				
		P2PMoneyAlphaGlobals::logString($errormessage);
		
		return $errormessage;
	}
	
	
}