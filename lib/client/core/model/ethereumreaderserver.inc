<?php 
namespace ethereum_reader\rest_client\core\model;

use p2pmoney\common\core\model\P2PMoneyAlphaDeployment;
use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;
use p2pmoney\common\core\model\RestConnection;
use p2pmoney\common\core\model\DBConnection;

require_once(P2PMoneyAlphaDeployment::getCommonCodeDir().'/p2pmoney_alpha/core/model/restconnection.inc');
require_once(P2PMoneyAlphaDeployment::getCommonCodeDir().'/p2pmoney_alpha/core/model/dbconnection.inc');

require_once('answerobject.inc');
require_once('blocks.inc');
require_once('transactions.inc');
require_once('accounts.inc');

class EthereumReaderServer {

	private static $server;
	
	static public function getServer() {
		if (!isset(EthereumReaderServer::$server)) {
			EthereumReaderServer::$server = new EthereumReaderServer();
		}
		
		return EthereumReaderServer::$server;
	}
	
	// dd connection to local database
	private $dbcon;
	
	private $databasename;
	private $dbusername;
	private $dbpassword;
	private $dbhost;
	private $dbport;
	
	private $dbtableprefix;
	
	public function getDBName() {
		return $this->databasename;
	}
	
	public function setDBName($database) {
		$this->databasename = $database;
	}
	
	public function getDBUserName() {
		return $this->dbusername;
	}
	
	public function setDBUserName($username) {
		$this->dbusername = $username;
	}
	
	public function getDBPassword() {
		return $this->dbpassword;
	}
	
	public function setDBPassword($password) {
		$this->dbpassword = $password;
	}
	
	public function getDBHost() {
		return $this->dbhost;
	}
	
	public function setDBHost($host) {
		$this->dbhost = $host;
	}
	
	public function getDBPort() {
		return $this->dbport;
	}
	
	public function setDBPort($port) {
		$this->dbport = $port;
	}
	
	public function getDBTablePrefix() {
		return $this->dbtableprefix;
	}
	
	public function setDBTablePrefix($tableprefix) {
		$this->dbtableprefix = $tableprefix;
	}
	
	public function getDBConnection() {
		if (!isset($this->dbcon)) {
			$this->dbcon = new DBConnection();
			
			$this->dbcon->setName($this->databasename);
			$this->dbcon->setUserName($this->dbusername);
			$this->dbcon->setPassword($this->dbpassword);
			$this->dbcon->setHost($this->dbhost);
			$this->dbcon->setPort($this->dbport);
			$this->dbcon->setTablePrefix($this->dbtableprefix);
				
			
		}
		
		return $this->dbcon;
	}
	
	public function setDBConnection($connection) {
		$this->dbcon = $connection;
	}
	
	
	// rest connection to ethereum_reader_server
	private $ethrestcon;
	
	private $resturl;
	private $restdomain;
	private $apikey;
	
	static public function logString($log) {
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
			
			$this->ethrestcon->setExtraHeader('apikey', $this->apikey);
		}
		
		return $this->ethrestcon;
	}
	
	public function setEthereumConnection($connection) {
		$this->ethrestcon = $connection;
	}
	
	// rest api
	protected function getAccountInfoAnswerObject($address) {
		$restCon = $this->getEthereumConnection();
		
		$this->logString('EthereumReaderServer::getAccountInfoAnswerObject called with address ['.$address.']');
		
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
		
		if (isset($decoded) && (isset($decoded['data']))  && (isset($decoded['data'][0]))) {
			$data = $this->processAccountRestData($decoded['data'][0]);
			
			foreach($data as $key=>$value) {
				$answerobject->add($key, $value);
			}
		}
		
		return $answerobject;
	}
	
	
	protected function saveAccounts($accounts) {
		$this->logString('EthereumReader::saveAccounts called');
		
		$dbCon = $this->getDBConnection();
		
		$accountsarray = $accounts->getAccountArray();
		
		foreach($accountsarray as $account) {
			// check if account already saved
			$accountaddress = $account->getAddress();
			$savedaccount = Account::getAccountFromAddress($accountaddress, $dbCon);
			
			if (!isset($savedaccount)) {
				$this->logString('Inserting account '.$accountaddress.'in database');
			}
			else {
				$this->logString('Updating account '.$accountaddress.'in database');
				}
				
			$account->save($dbCon);
		}
		
	}
	
	protected function processAccountRestData($data) {
		// we process members needed a format transfer
		$data['firstseen'] = strtotime($data['firstseen']);
		
		return $data;
	}
	
	public function getAccount($address) {
		$this->logString('EthereumReaderServer::getAccount called with address ['.$address.']');
		
		$answerobject = $this->getAccountInfoAnswerObject($address);
		
		$data = $answerobject->getArray();
		$account = Account::getAccountFromData($data);
		
		// save this account
		$accounts = new Accounts();
		$accounts->addAccount($account);
		$this->saveAccounts($accounts);
		
		return $account;
	}
	
	public function getBlocksCount() {
		$answerobject = $this->getBlocksCountAnswerObject() ;
		
		return $answerobject->get('count');
	}
	
	protected function getBlocksCountAnswerObject() {
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
	
	protected function saveBlocks($blocks) {
		$this->logString('EthereumReader::saveBlocks called');
		
		$dbCon = $this->getDBConnection();
		
		$blocksarray = $blocks->getBlockArray();
		
		$this->logString('EthereumReader::saveBlocks block array size '.count($blocksarray));
		
		foreach($blocksarray as $block) {
			//see if we can find the block in the database
			$blocknumber = $block->getNumber();
			$savedblock = Block::getBlockFromNumber($blocknumber, $dbCon);
			
			if (!isset($savedblock)) {
				$block->save($dbCon);
			}
			else {
				// maybe we could check if block needs to be re-written
			}
			
		}
		
	}
	
	protected function processBlockRestData($data) {
		// we process members needed a format transfer
		$data['time'] = strtotime($data['time']);
		
		return $data;
	}
	
	public function getLastBlocks($offset, $count) {
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
				
			while (isset($decoded['data'][$i])) {
				$data = $this->processBlockRestData($decoded['data'][$i]);
				
				$block = Block::getBlockFromData($data);

				$blocks->addBlock($block);

				$i++;
			}
		}
		
		$this->saveBlocks($blocks);
		
			
		return $blocks;
	}
	
	public function getBlockRange($startBlockNum, $endBlockNum) {
		$restCon = $this->getEthereumConnection();
		
		// GET
		$resource[] = 'blocks/range';
		$resource[] = $startBlockNum;
		$resource[] = $endBlockNum;
		
		$service_url = $restCon->getServiceUrl($resource);
		
		$this->logString('EthereumReaderServer::getBlockRange called, service url is '.$service_url);
		
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
		
			while (isset($decoded['data'][$i])) {
				$data = $this->processBlockRestData($decoded['data'][$i]);
		
				$block = Block::getBlockFromData($data);
		
				$blocks->addBlock($block);
		
				$i++;
			}
		}
		
		$this->saveBlocks($blocks);
		
			
		return $blocks;
	}
	
	// transactions 
	protected function saveTransactions($transactions) {
		$this->logString('EthereumReader::saveTransactions called');
		
		$dbCon = $this->getDBConnection();
		
		$transactionsarray = $transactions->getTransactionArray();
		
		$this->logString('EthereumReader::saveTransactions transaction array size '.count($transactionsarray));
		
		foreach($transactionsarray as $transaction) {
			//see if we can find the block in the database
			$txhash = $transaction->getHash();
			$savedtransaction = Transaction::getTransactionFromHash($txhash, $dbCon);
				
			if (!isset($savedtransaction)) {
				$transaction->save($dbCon);
			}
			else {
				// maybe we could check if transaction needs to be re-written
				$this->logString('transaction read is '.print_r($savedtransaction, true));
			}
				
		}
		
	}
	
	protected function processTransactionRestData($data) {
		// we process members needed a format transfer
		$data['time'] = strtotime($data['time']);
		
		return $data;
	}
	
	public function getLastTransactions($offset, $count) {
		$restCon = $this->getEthereumConnection();
		
		// GET
		$resource[] = 'txs';
		$resource[] = $offset;
		$resource[] = $count;
		
		$service_url = $restCon->getServiceUrl($resource);
		
		$this->logString('EthereumReaderServer::getLastTransactions called, service url is '.$service_url);
		
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
		
		$transactions = new Transactions();
		
		$transactions->setJsonAnswer($jsonanswer);
		
		if (isset($decoded) && (isset($decoded['data'])  )) {
			$i = 0;
		
			while (isset($decoded['data'][$i])) {
				$data = $this->processTransactionRestData($decoded['data'][$i]);
		
				$transaction = Transaction::getTransactionFromData($data);
		
				$transactions->addTransaction($transaction);
		
				$i++;
			}
		}
		
		$this->saveTransactions($transactions);
		
			
		return $transactions;
		
	}
}