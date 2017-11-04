<?php 
namespace ethereum_reader\rest_client\core\model;

use p2pmoney\common\core\model\P2PMoneyAlphaDeployment;
use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;
use p2pmoney\common\core\model\RestConnection;

require_once(P2PMoneyAlphaDeployment::getCommonCodeDir().'/p2pmoney_alpha/core/model/restconnection.inc');

require_once('answerobject.inc');
require_once('blocks.inc');

class EthereumReaderServer {

	private static $server;
	
	static public function getServer() {
		if (!isset(EthereumReaderServer::$server)) {
			EthereumReaderServer::$server = new EthereumReaderServer();
		}
		
		return EthereumReaderServer::$server;
	}

	private $ethrestcon;
	
	private $resturl;
	private $restdomain;
	private $apikey;
	
	public function logString($log) {
		P2PMoneyAlphaGlobals::logString($log);
	}
	
	public function getRestConnectionUrl() {
		return $this->resturl;
	}
	
	public function setRestConnectionUrl($url) {
		unset($this->ethrestcon);
		$this->resturl = $url;
	}
	
	public function getRestDomain() {
		return $this->restdomain;
	}
	
	public function setRestDomain($restdomain) {
		unset($this->ethrestcon);
		$this->restdomain = $restdomain;
	}
	
	public function getApiKey() {
		return $this->apikey;
	}
	
	public function setApiKey($apikey) {
		unset($this->ethrestcon);
		$this->apikey =$apikey;
	}
	
	public function getEthereumConnection() {
		if (!isset($this->ethrestcon)) {
			$this->ethrestcon = new RestConnection();
			
			$this->ethrestcon->setBaseURL($this->resturl);
			$this->ethrestcon->setDomain($this->restdomain);
			$this->ethrestcon->setAccessToken($this->apikey);
			$this->ethrestcon->setCleanUrl(true);
		}
		
		return $this->ethrestcon;
	}
	
	public function setEthereumConnection($connection) {
		$this->ethrestcon = $connection;
	}
	
	// rest api
	public function getAccountInfoAnswerObject($address) {
		$restCon = $this->getEthereumConnection();
		
		$this->logString('address is ['.$address.']');
		
		// GET
		$resource[] = 'account';
		$resource[] = preg_replace('/\s+/', '', $address); // remove blanks
		
		$service_url = $restCon->getServiceUrl($resource);
		
		$this->logString('EthereumReaderServer::getAccountInfoAnswerObject called, service url is '.$service_url);
		
		// GET curl call
		$expectedhttpcode = '200';
		$sessiontoken = null;
				
		$jsonanswer = '';
		
		try {
			$decoded = $restCon->curlGetCall($resource, $expectedhttpcode, $sessiontoken, $jsonanswer);
		}
		catch(RestException $e) {
			$this->logString('rest exception: '.$e->getMessage());
			$decoded = null;
		}
		
		$answerobject = new AnswerObject();
		
		$answerobject->setJsonAnswer($jsonanswer);
		
		if (isset($decoded) && (isset($decoded['data']) && (isset($decoded['data'][0]['balance'])) )) {
			$answerobject->add('balance', $decoded['data'][0]['balance']);
		}
		
		return $answerobject;
	}
	
	public function getBlocksCount() {
		$answerobject = $this->getBlocksCountAnswerObject() ;
		
		return $answerobject->get('count');
	}
	
	public function getBlocksCountAnswerObject() {
		$restCon = $this->getEthereumConnection();
		
		// GET
		$resource[] = 'blocks/count';
		
		$service_url = $restCon->getServiceUrl($resource);
		
		$this->logString('EthereumReaderServer::getBlocksCountAnswerObject called, service url is '.$service_url);
		
		// GET curl call
		$expectedhttpcode = '200';
		$sessiontoken = null;
				
		$jsonanswer = '';
		
		try {
			$decoded = $restCon->curlGetCall($resource, $expectedhttpcode, $sessiontoken, $jsonanswer);
		}
		catch(RestException $e) {
			$this->logString('rest exception: '.$e->getMessage());
			$decoded = null;
		}
		
		$answerobject = new AnswerObject();
		
		$answerobject->setJsonAnswer($jsonanswer);
		
		if (isset($decoded) && (isset($decoded['data']) && (isset($decoded['data'][0]['count'])) )) {
			$answerobject->add('count', $decoded['data'][0]['count']);
		}
		
		return $answerobject;
	}
	
	public function getBlocksFrom($offset, $count) {
		$restCon = $this->getEthereumConnection();
		
		// GET
		$resource[] = 'blocks';
		$resource[] = $offset;
		$resource[] = $count;
		
		$service_url = $restCon->getServiceUrl($resource);
		
		$this->logString('EthereumReaderServer::getBlocksCount called, service url is '.$service_url);
		
		// GET curl call
		$expectedhttpcode = '200';
		$sessiontoken = null;
		
		$jsonanswer = '';
		
		try {
			$decoded = $restCon->curlGetCall($resource, $expectedhttpcode, $sessiontoken, $jsonanswer);
		}
		catch(RestException $e) {
			$this->logString('rest exception: '.$e->getMessage());
			$decoded = null;
		}
		
		$blocks = new Blocks();
		
		$blocks->setJsonAnswer($jsonanswer);
		
		if (isset($decoded) && (isset($decoded['data'])  )) {
			$i = 0;
				
			while ((isset($decoded['data'][$i])) &&
					(isset($decoded['data'][$i]['data'])) &&
					(isset($decoded['data'][$i]['data'][0])) ) {
						$block = Block::getBlock($decoded['data'][$i]['data'][0]);
		
						$blocks->addBlock($block);
		
						$i++;
					}
		}
		
			
		return $blocks;
	}
	
	public function getBlockRange($startBlockNum, $endBlockNum) {

		$lastblocknumber = Block::getLastBlockNumber();
		$offset = $lastblocknumber - $startBlockNum;
		$count = $endBlockNum - $startBlockNum;
		
		
		return $this->getBlocksFrom($offset, $count);
	}
}