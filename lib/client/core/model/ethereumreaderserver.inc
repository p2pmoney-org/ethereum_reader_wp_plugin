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
	
	// db caching
	private $bCacheSaveToDB;
	
	private $iCacheSaveMode;
	
	const CACHE_SAVE_ALL = 1;
	const CACHE_SAVE_TX_FOR_KNOWN_ACCOUNTS = 2;
	const CACHE_SAVE_BLOCKS_FOR_KNOWN_TX = 4;
	
	
	public function getCacheSaveToDB() {
		if (!isset($this->bCacheSaveToDB))
			return false;
		
		return $this->bCacheSaveToDB;
	}
	
	public function setCacheSaveToDB($bCacheSaveToDB) {
		$this->bCacheSaveToDB = $bCacheSaveToDB;
	}
	
	public function getCacheSaveToDBMode() {
		return $this->iCacheSaveMode;
	}
	
	public function saveTxForKnownAccountsToDB() {
		if (isset($this->iCacheSaveMode))
			return ($this->iCacheSaveMode & EthereumReaderServer::CACHE_SAVE_TX_FOR_KNOWN_ACCOUNTS ? true : false);
		
		return false;
	}
	
	public function saveBlocksForKnownTxToDB() {
		if (isset($this->iCacheSaveMode))
			return ($this->iCacheSaveMode & EthereumReaderServer::CACHE_SAVE_BLOCKS_FOR_KNOWN_TX ? true : false);
		
		return false;
	}
	
	public function saveAllToDB() {
		if (isset($this->iCacheSaveMode))
			return ($this->iCacheSaveMode & EthereumReaderServer::CACHE_SAVE_ALL ? true : false);
		
		return false;
	}
	
	public function setCacheSaveToDBMode($bCacheMode) {
		$this->iCacheSaveMode = $bCacheMode;
	}
	
	// reading from cache
	private $bCacheReadFromDB;
	
	private $iCacheReadMode;
	
	const CACHE_READ_ALL = 1;
	
	public function getCacheReadFromDB() {
		if (!isset($this->bCacheReadFromDB))
			return false;
	
		return $this->bCacheReadFromDB;
	}
	
	public function setCacheReadFromDB($bCacheReadFromDB) {
		$this->bCacheReadFromDB = $bCacheReadFromDB;
	}
	
	// mode
	public function getCacheReadFromDBMode() {
		return $this->iCacheReadMode;
	}
	
	
	public function readAllFromDB() {
		if (isset($this->iCacheReadMode))
			return ($this->iCacheReadMode & EthereumReaderServer::CACHE_READ_ALL ? true : false);
	
		return false;
	}
	
	public function setCacheReadFromDBMode($iCacheMode) {
		$this->iCacheReadMode = $iCacheMode;
	}
	
	
	// db connection to local database
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
	
	public function getRestConnection() {
		return $this->getEthereumConnection();
	}
	
	public function setRestConnection($connection) {
		$this->ethrestcon = $connection;
	}
	
	// rest api
	
	// version
	public function getVersionInfoAnswerObject() {
		$restCon = $this->getEthereumConnection();
		
		$this->logString('EthereumReaderServer::getVersionInfoAnswerObject called');
		
		// GET
		$resource[] = 'version';
		
		$service_url = $restCon->getServiceUrl($resource);
		
		$this->logString('EthereumReaderServer::getVersionInfoAnswerObject called, service url is '.$service_url);
		
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
			$data = $decoded['data'][0];
			
			foreach($data as $key=>$value) {
				$answerobject->add($key, $value);
			}
		}
		
		return $answerobject;
	}
	
	public function getVersionInfo() {
		$answerobject = $this->getVersionInfoAnswerObject($address);
		
		$data = $answerobject->getArray();
		
		return $data;
	}
	
	public function getVersionSupportInfoAnswerObject() {
		$restCon = $this->getEthereumConnection();
	
		$this->logString('EthereumReaderServer::getVersionSupportInfoAnswerObject called');
	
		// GET
		$resource[] = 'version/support';
	
		$service_url = $restCon->getServiceUrl($resource);
	
		$this->logString('EthereumReaderServer::getVersionSupportInfoAnswerObject called, service url is '.$service_url);
	
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
			$data = $decoded['data'][0];
				
			foreach($data as $key=>$value) {
				$answerobject->add($key, $value);
			}
		}
	
		return $answerobject;
	}
	
	// node
	public function getNodeInfoAnswerObject() {
		$restCon = $this->getEthereumConnection();
	
		$this->logString('EthereumReaderServer::getNodeInfoAnswerObject called');
	
		// GET
		$resource[] = 'node';
	
		$service_url = $restCon->getServiceUrl($resource);
	
		$this->logString('EthereumReaderServer::getNodeInfoAnswerObject called, service url is '.$service_url);
	
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
			$data = $decoded['data'][0];
	
			foreach($data as $key=>$value) {
				$answerobject->add($key, $value);
			}
		}
	
		return $answerobject;
	}
	
	public function getNodeInfo() {
		$answerobject = $this->getNodeInfoAnswerObject($address);
	
		$data = $answerobject->getArray();
	
		return $data;
	}
	
	
	// account
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
		if (!$this->getCacheSaveToDB())
			return;
		
		$this->logString('EthereumReader::saveAccounts called');
		
		$dbCon = $this->getDBConnection();
		
		$accountsarray = $accounts->getAccountArray();
		
		foreach($accountsarray as $account) {
			// check if account already saved
			$accountaddress = $account->getAddress();
			$savedaccount = Account::getAccountFromAddress($accountaddress, $dbCon);
			
			if (!isset($savedaccount)) {
				$this->logString('Inserting account '.$accountaddress.' in database');
			}
			else {
				$this->logString('Updating account '.$accountaddress.' in database');
			}
				
			$account->save($dbCon);
		}
		
	}
	
	protected function processAccountRestData($data) {
		// we process members needed a format transfer
		$data['firstseen'] = strtotime($data['firstseen']);
		
		return $data;
	}
	
	protected function saveAccount($account) {
		$accounts = new Accounts();
		$accounts->addAccount($account);
		$this->saveAccounts($accounts);
	}
	
	public function getAccount($address) {
		$this->logString('EthereumReaderServer::getAccount called with address ['.$address.']');
		
		// we always request latest status from the blockchain
		$answerobject = $this->getAccountInfoAnswerObject($address);
		
		$data = $answerobject->getArray();
		$account = Account::getAccountFromData($data);
		
		if ($this->getCacheSaveToDB()) {
			$dbCon = $this->getDBConnection();
			$savedaccount = Account::getAccountFromAddress($address, $dbCon);
			
			if (isset($savedaccount)) {
				$account->updateAccount($savedaccount);
			}
		
			// save this account
			$this->saveAccount($account);
		}
		
		return $account;
	}
	
	// account transactions and scanning of the blockchain
	const DEFAULT_SCANNING_TIME = 2000; // 2s expressed as 2000 milliseconds
	const MAX_BLOCK_SCAN = 100; // scan by lots of 100 blocks
	
	protected function getAccountTransactionsBeforeBlockAnswerObject($address, $offset, $blocknumber) {
		$restCon = $this->getEthereumConnection();
	
		$this->logString('EthereumReaderServer::getAccountTransactionsBeforeBlockAnswerObject called for address ['.$address.'] before block '.$blocknumber);
	
		// GET
		$resource[] = 'account';
		$resource[] = preg_replace('/\s+/', '', $address); // remove blanks
		$resource[] = 'txs';

		$resource[] = (isset($offset) ? $offset : 0);
		$resource[] = 'before-block';
		$resource[] = (isset($blocknumber) ? $blocknumber : null);
		
		$service_url = $restCon->getServiceUrl($resource);

		$this->logString('EthereumReaderServer::getAccountTransactionsBeforeBlockAnswerObject called, service url is '.$service_url);

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

		if (isset($decoded)) {
			
			// meta data
			if (isset($decoded['status'])) {
				$answerobject->addMetaData('status', $decoded['status']);
			}
					
			if (isset($decoded['highestblock'])) {
				$answerobject->addMetaData('highestblock', $decoded['highestblock']);
			}
					
			if (isset($decoded['lowestblock'])) {
				$answerobject->addMetaData('lowestblock', $decoded['lowestblock']);
			}
					
			// data
			if (isset($decoded['data'])) {
				$data = $decoded['data'];
	
				foreach($data as $key=>$value) {
					$answerobject->add($key, $value);
				}
			}
		}

		return $answerobject;
	}
	
	
	protected function getAccountTransactionsAfterBlockAnswerObject($address, $offset, $blocknumber) {
		$restCon = $this->getEthereumConnection();
	
		$this->logString('EthereumReaderServer::getAccountTransactionsAfterBlockAnswerObject called for address ['.$address.'] after block '.$blocknumber);
	
		// GET
		$resource[] = 'account';
		$resource[] = preg_replace('/\s+/', '', $address); // remove blanks
		$resource[] = 'txs';

		$resource[] = (isset($offset) ? $offset : 0);
		$resource[] = 'after-block';
		$resource[] = (isset($blocknumber) ? $blocknumber : null);
		
		$service_url = $restCon->getServiceUrl($resource);

		$this->logString('EthereumReaderServer::getAccountTransactionsAfterBlockAnswerObject called, service url is '.$service_url);

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

		if (isset($decoded)) {
			
			// meta data
			if (isset($decoded['status'])) {
				$answerobject->addMetaData('status', $decoded['status']);
			}
					
			if (isset($decoded['highestblock'])) {
				$answerobject->addMetaData('highestblock', $decoded['highestblock']);
			}
					
			if (isset($decoded['lowestblock'])) {
				$answerobject->addMetaData('lowestblock', $decoded['lowestblock']);
			}
					
			// data
			if (isset($decoded['data'])) {
				$data = $decoded['data'];
	
				foreach($data as $key=>$value) {
					$answerobject->add($key, $value);
				}
			}
		}
		
		return $answerobject;
	}
	
	
	protected function getAccountTransactionsAnswerObject($address, $offset, $fromblock, $toblock) {
		$restCon = $this->getEthereumConnection();
	
		$this->logString('EthereumReaderServer::getAccountTransactionsAnswerObject called for address ['.$address.'] offset '.$offset.' from block '.$fromblock.' to block '.$toblock);
		
		if ($toblock - $fromblock > EthereumReaderServer::MAX_BLOCK_SCAN )
			throw new Exception('scanning from '.$fromblock.' to block '.$toblock.' is too large (maximum of '.EthereumReaderServer::MAX_BLOCK_SCAN.' blocks).');
	
		// GET
		$resource[] = 'account';
		$resource[] = preg_replace('/\s+/', '', $address); // remove blanks
		$resource[] = 'txs';
		
		if (isset($toblock)) {
			$resource[] = (isset($offset) ? $offset : 0);
			$resource[] = 'blocks';
			$resource[] = (isset($fromblock) ? $fromblock : $toblock);
			$resource[] = $toblock;
		}
		else {
			if (isset($fromblock)) {
				$resource[] = (isset($offset) ? $offset : 0);
				$resource[] = 'blocks';
				$resource[] = $fromblock;
				
			}
			else {
				if (isset($offset)) {
					$resource[] = $offset;
				}
			}
		}
	
		$service_url = $restCon->getServiceUrl($resource);
	
		$this->logString('EthereumReaderServer::getAccountTransactionsAnswerObject called, service url is '.$service_url);
	
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
	
		if (isset($decoded)) {
			
			// meta data
			if (isset($decoded['status'])) {
				$answerobject->addMetaData('status', $decoded['status']);
			}
					
			if (isset($decoded['highestblock'])) {
				$answerobject->addMetaData('highestblock', $decoded['highestblock']);
			}
					
			if (isset($decoded['lowestblock'])) {
				$answerobject->addMetaData('lowestblock', $decoded['lowestblock']);
			}
					
			// data
			if (isset($decoded['data'])) {
				$data = $decoded['data'];
	
				foreach($data as $key=>$value) {
					$answerobject->add($key, $value);
				}
			}
		}
		
		return $answerobject;
	}
	
	public function getAccountTransactions($address, $maxscantime = EthereumReaderServer::DEFAULT_SCANNING_TIME) {
		$this->logString('EthereumReaderServer::getAccountTransactions called with address ['.$address.'] and max scan time '.$maxscantime);
		
		// get account (create it in database, if caching is done and it does not exist yet)
		$account = $this->getAccount($address);
		
		$transactioncount = $account->getTransactionCount();

		
		if ($this->getCacheReadFromDB()) {
			$this->logString('searching in cache transactions for address['.$address.']');
			$dbCon = $this->getDBConnection();
			
			
			$accounttransactions = $account->getTransactions($dbCon);
			$this->logString('account '.$address.' has currently '.$accounttransactions->getTransactionCount().' transactions cached');
			
			if ($transactioncount == 0) {
				// nothing to scan
				return $accounttransactions;
			}
			else {
				if (isset($accounttransactions) && ($transactioncount == $accounttransactions->getTransactionCount())) {
					// we have found all the transactions already
					return $accounttransactions;
				}
			}
				
		}

		$firstscannedblock = $account->getFirstScannedBlock();
		$lastscannedblock = $account->getLastScannedBlock();
		$this->logString('first scanned is '.$firstscannedblock.' last scanned is '.$lastscannedblock.'.');
		
		$currentblock = $this->getBlocksCount();
		
		$firstscannedblock = ($firstscannedblock != -1 ? $firstscannedblock : $currentblock);
		$lastscannedblock = ($lastscannedblock != -1 ? $lastscannedblock : $currentblock);
		
		$offset = null;
		$fromblock = null;
		$toblock = null;
		
		$scandirection = -1; // -1 backward, +1 forward
		
		$starttime = time(); // works in seconds, we could use microtime if necessary
		$currenttime = time();
		$iter = 0;
		
		while ((($currenttime - $starttime)*1000) < $maxscantime) {
			
			if (($currentblock - $lastscannedblock) > (EthereumReaderServer::MAX_BLOCK_SCAN / 2)) {
				$scandirection = +1;
					
				$fromblock = $lastscannedblock + 1 ;
				$toblock = $lastscannedblock + EthereumReaderServer::MAX_BLOCK_SCAN;
			}
			else {
				$fromblock = $firstscannedblock - EthereumReaderServer::MAX_BLOCK_SCAN;
				$toblock = $firstscannedblock - 1;
			}
			
			$this->logString('doing '.($scandirection == 1 ? 'forward' : 'backward').' transactions search for account '.$address.($scandirection == 1 ? ' after block '.$lastscannedblock : ' before block '.$firstscannedblock));
			
				
				
			if ($scandirection == 1) {
				// request by range
				//$answerobject = $this->getAccountTransactionsAnswerObject($address, $offset, $fromblock, $toblock);
				
				// request by limit
				$answerobject = $this->getAccountTransactionsAfterBlockAnswerObject($address, $offset, $lastscannedblock);
			}
			else {
				// request by range
				//$answerobject = $this->getAccountTransactionsAnswerObject($address, $offset, $fromblock, $toblock);
				
				// request by limit
				$answerobject = $this->getAccountTransactionsBeforeBlockAnswerObject($address, $offset, $firstscannedblock);
			}
				
			$data = $answerobject->getArray();
			
			// get scope scanned by remote server
			$highestblock = $answerobject->getMetaData('highestblock');
			$lowestblock = $answerobject->getMetaData('lowestblock');
			
			$this->logString('remote server did a search between '.$lowestblock.' and '.$highestblock);
				
			$transactions = new Transactions();
			
			$transactions->setJsonAnswer($jsonanswer);
			
			$i = 0;
			
			while (isset($data[$i])) {
				$datatx = $this->processTransactionRestData($data[$i]);
			
				$transaction = Transaction::getTransactionFromData($datatx);
			
				$transactions->addTransaction($transaction);
				
				$i++;
			}
			
			$cnt = $transactions->getTransactionCount();
			if ($cnt > 0) {
				$this->logString('found '.$cnt.' new transactions for account '.$address.' beween '.$fromblock.' and '.$toblock.' blocks.');
				
				$this->saveTransactions($transactions);
					
				// add to account transactions
				$account->addTransactions($transactions);
			}
			
			switch($scandirection) {
				case -1:
					// request by range
					//$firstscannedblock = $fromblock;

					// request by limits
					$firstscannedblock = $lowestblock;
						
					// next scan range
					$fromblock = $fromblock - EthereumReaderServer::MAX_BLOCK_SCAN;
					$toblock = $toblock - EthereumReaderServer::MAX_BLOCK_SCAN;
					break;
				case 1:
					// request by range
					//$lastscannedblock = $toblock;
					
					// request by limits
					$lastscannedblock = $highestblock;
						
					// next scan range
					$fromblock = $fromblock + EthereumReaderServer::MAX_BLOCK_SCAN;
					$toblock = $toblock + EthereumReaderServer::MAX_BLOCK_SCAN;
					break;
			}
			
			$iter++;
			$currenttime = time();
			
			$this->logString('next iteration is '.$iter.' time spent is '.(($currenttime - $starttime)*1000).'ms after '.$starttime);
			
			if ($iter > 15)
			break;
		}
		
		// we save the new scanned blocks limits
		$account->setFirstScannedBlock($firstscannedblock);
		$account->setLastScannedBlock($lastscannedblock);
		

		// save this account
		$this->saveAccount($account);
		
		
		return $accounttransactions;
	}
	
	
	// block
	protected function processBlockRestData($data) {
		// we process members needed a format transfer
		$data['time'] = strtotime($data['time']);
	
		return $data;
	}
	
	protected function saveBlock($block) {
		$blocks = new Blocks();
		
		$blocks->addBlock($block);
		
		$this->saveBlocks($blocks);
	}
	
	public function getBlock($blocknumber) {
		$this->logString('EthereumReader::getBlock called for block '.$blocknumber);
	
		if ($this->getCacheReadFromDB()) {
			$this->logString('searching in cache block number '.$blocknumber);
			// first look in cache if we do it
			$dbCon = $this->getDBConnection();
				
			$cachedblock = Block::getBlockFromNumber($blocknumber, $dbCon);
	
			if (isset($cachedblock)) {
				$this->logString('block '.$blocknumber.' found in cache');
	
				return $cachedblock;
			}
		}
	
		$restCon = $this->getEthereumConnection();
	
		// GET
		$resource[] = 'block';
		$resource[] = $blocknumber;
	
		$service_url = $restCon->getServiceUrl($resource);
	
		$this->logString('EthereumReaderServer::getBlock called, service url is '.$service_url);
	
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
	
		$data = $this->processBlockRestData($decoded['data'][0]);
	
		$block = Block::getBlockFromData($data);
	
		// call save
		$this->saveBlock($block);
	
			
		return $block;
	}
	
	
	public function getBlockTransactions($blocknumber) {
		$this->logString('EthereumReader::getBlockTransactions called for block '.$blocknumber);
		
		// we could use the /block/:id/txs rest call as well
		return EthereumReaderServer::getBlockRangeTransactions($blocknumber, $blocknumber);
	}
	
	
	// blocks
	public function getBlocksCount() {
		$answerobject = $this->getBlocksCountAnswerObject() ;
		
		return $answerobject->get('count');
	}
	
	public function getLastestBlockNumber() {
		return $this->getBlocksCount();
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
		if (!$this->getCacheSaveToDB())
			return;
		
		if ($this->saveBlocksForKnownTxToDB())
			return;
		
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
				
				// TODO: save the block's transactions
			}
			else {
				// maybe we could check if block needs to be re-written
			}
			
		}
		
	}
	
	public function getLastBlocks($offset, $count) {
		$this->logString('EthereumReader::getLastBlocks called for offset '.$offset.' count '.$count);
		$this->logString('cache save to db '.$this->getCacheSaveToDB().' save all '.$this->readAllFromDB());
		
		if (($this->getCacheReadFromDB()) && ($this->readAllFromDB())) {
			$this->logString('searching in cache blocks for offset '.$offset.' count '.$count);
				
			// get last block on distant host
			$lastblocknumber = $this->getBlocksCount();
			
			// get last block in our cache
			$dbCon = $this->getDBConnection();
		
			$lastcachedblocknumber = Block::getLastBlockNumber($dbCon);
			$this->logString('last cached block is '.$lastcachedblocknumber.' and remote block number is '.$lastblocknumber);
				
			if ($lastblocknumber == $lastcachedblocknumber) {
				$startBlockNum = $lastblocknumber - $offset + 1;
				$endBlockNum = $startBlockNum + $count;
				
				$cacheblocks = Block::getBlockRange($startBlockNum, $endBlockNum, $dbCon);
				
				if (isset($cacheblocks)) {
					$this->logString('blocks for offset '.$offset.' count '.$count.' found in cache');
					
					return $cacheblocks;
				}
			}
		}
		
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
		$this->logString('EthereumReader::getBlockRange called for start '.$startBlockNum.' to '.$endBlockNum);
		
		if (($this->getCacheReadFromDB()) && ($this->readAllFromDB())) {
			$this->logString('searching in cache blocks from '.$startBlockNum.' to '.$endBlockNum);
				
			// get last block on distant host
			$lastblocknumber = $this->getBlocksCount();
			
			// get last block in our cache
			$dbCon = $this->getDBConnection();
		
			$lastcachedblocknumber = Block::getLastBlockNumber($dbCon);
				
			if ($lastblocknumber == $lastcachedblocknumber) {
				
				$cacheblocks = Block::getBlockRange($startBlockNum, $endBlockNum, $dbCon);
				
				if (isset($cacheblocks)) {
					$this->logString('blocks from '.$startBlockNum.' to '.$endBlockNum.' found in cache');
						
					return $cacheblocks;
				}
			}
		}
		
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
	
	public function getBlockRangeTransactions($startBlockNum, $endBlockNum) {
		$this->logString('EthereumReader::getBlockRangeTransactions called for start '.$startBlockNum.' to '.$endBlockNum);
		
		if (($this->getCacheReadFromDB()) && ($this->readAllFromDB())) {
			$this->logString('searching in cache transactions for blocks from '.$startBlockNum.' to '.$endBlockNum);
				
			$dbCon = $this->getDBConnection();
		
			$lastcachedblocknumber = Block::getLastBlockNumber($dbCon);
			
			if ($endBlockNum <= $lastcachedblocknumber) {
				$cachedtransactions = Transaction::getBlockRangeTransactions($startBlockNum, $endBlockNum, $dbCon);
				
				if (isset($cachedtransactions)) {
					$this->logString('transactions for blocks from '.$startBlockNum.' to '.$endBlockNum.' found in cache');
						
					return $cachedtransactions;
				}
			}
		}
		
		$restCon = $this->getEthereumConnection();
		
		// GET
		$resource[] = 'blocks/range';
		$resource[] = $startBlockNum;
		$resource[] = $endBlockNum;
		$resource[] = 'txs';
		
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
	
	// transaction
	protected function saveTransaction($transaction) {
		$transactions = new Transactions();
		
		$transactions->addTransaction($transaction);
		
		$this->saveTransactions($transactions);
	}
	
	protected function processTransactionRestData($data) {
		// we process members needed a format transfer
		$data['time'] = strtotime($data['time']);
		
		return $data;
	}
	
	public function getTransaction($hash) {
		$this->logString('EthereumReader::getTransaction called for hash '.$hash);
	
		if ($this->getCacheReadFromDB()) {
			$this->logString('searching in cache transaction with hash '.$hash);
			
			// first look in cache if we do it
			$dbCon = $this->getDBConnection();
	
			$cachedtransaction = Transaction::getTransactionFromHash($hash, $dbCon);
	
			if (isset($cachedtransaction)) {
				$this->logString('transaction '.$hash.' found in cache');
				return $cachedtransaction;
			}
		}
	
	
	
		$restCon = $this->getEthereumConnection();
	
		// GET
		$resource[] = 'tx';
		$resource[] = $hash;
	
		$service_url = $restCon->getServiceUrl($resource);
	
		$this->logString('EthereumReaderServer::getTransaction called, service url is '.$service_url);
	
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
	
		$data = $this->processTransactionRestData($decoded['data'][0]);
	
		$transaction = Transaction::getTransactionFromData($data);
	
		// call save
		$this->saveTransaction($transaction);
	
		return $transaction;
	}
	
	
	// transactions 
	protected function saveTransactions($transactions) {
		if (!$this->getCacheSaveToDB())
			return;
		
		$this->logString('EthereumReader::saveTransactions called');
		
		$dbCon = $this->getDBConnection();
		
		$transactionsarray = $transactions->getTransactionArray();
		
		$this->logString('EthereumReader::saveTransactions transaction array size '.count($transactionsarray));
		
		foreach($transactionsarray as $transaction) {
			//see if we can find the block in the database
			$txhash = $transaction->getHash();
			$savedtransaction = Transaction::getTransactionFromHash($txhash, $dbCon);
				
			if (!isset($savedtransaction)) {
				
				$bSaveTransaction = false;
				$bSaveBlock = false;
				
				// get the corresponding block
				$blocknumber = $transaction->getBlockId();
				$senderaddress = $transaction->getSender();
				$recipientaddress = $transaction->getRecipient();

				if ($this->saveAllToDB()) {
					$bSaveTransaction = true;
						
					// coresponding accounts
					$senderaccount = Account::getAccountFromAddress($senderaddress, $dbCon);
					$recipientaccount = Account::getAccountFromAddress($recipientaddress, $dbCon);
					
					if (!isset($senderaccount)) {
						$senderaccount = $this->getAccount($senderaddress); // will save account because saveAllToDB
					}
									
					if (!isset($recipientaccount)) {
						$recipientaccount = $this->getAccount($recipientaddress); // will save account because saveAllToDB
					}
					
					// corresponding block
					$savedblock = Block::getBlockFromNumber($blocknumber, $dbCon);
					
					if (!isset($savedblock))
					$bSaveBlock = true;
				}
				else {
					if ($this->saveTxForKnownAccountsToDB()) {
						$senderaccount = Account::getAccountFromAddress($senderaddress, $dbCon);
						$recipientaccount = Account::getAccountFromAddress($recipientaddress, $dbCon);
						
						if (isset($senderaccount) || isset($recipientaccount))
							$bSaveTransaction = true;
					}
					
					if ($this->saveBlocksForKnownTxToDB()) {
						$savedblock = Block::getBlockFromNumber($blocknumber, $dbCon);
							
						if ((!isset($savedblock)) && ($bSaveTransaction)) {
							$bSaveBlock = true;
						}
						
					}
							
				}
				
				if ($bSaveTransaction) {
					if ($bSaveBlock) {
						$block = $this->getBlock($blocknumber);
						
						$this->logString('saving block '.$blocknumber);
						$block->save($dbCon);
					}
					
					$this->logString('saving transaction '.$transaction->getHash());
					$transaction->save($dbCon);
				}
			}
			else {
				// maybe we could check if transaction needs to be re-written
				$this->logString('transaction read is '.print_r($savedtransaction, true));
			}
				
		}
		
	}
	
	public function getLastTransactions($offset, $count) {
		$this->logString('EthereumReader::getLastTransactions called with offset '.$offset.' and count '.$offset);
			
		if (($this->getCacheReadFromDB()) && ($this->readAllFromDB())) {
			$this->logString('searching in cache transactions with offset '.$offset.' and count '.$offset);
				
			// get last block on distant host
			$lastblocknumber = $this->getBlocksCount();
			
			// get last block in our cache
			$dbCon = $this->getDBConnection();
		
			$lastcachedblocknumber = Block::getLastBlockNumber($dbCon);
			
			if ($lastblocknumber == $lastcachedblocknumber) {
				// we are in synch with the distant host
				
				$cachedtransactions = Transaction::getLastTransactions($offset, $count, $dbCon);
				
				if (isset($cachedtransactions)) {
					$this->logString('transaction offset from '.$offset.' count '.$count.' found in cache');
						
					return $cachedtransactions;
				}
			}
		}
		
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
	
	// static 
	const ETHER_TO_WEI = 1000000000000000000;
	
	static public function getEtherFromWei($weinumber, $decimals = 2) {
		$ether = intval(100 * $weinumber/EthereumReaderServer::ETHER_TO_WEI)/100;
		
		return $ether;
	}
	
	static public function getWeiFromEther($ethernumber) {
		$wei = intval($ethernumber * EthereumReaderServer::ETHER_TO_WEI);
		
		return $wei;
	}
}