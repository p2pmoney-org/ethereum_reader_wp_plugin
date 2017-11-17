<?php 

namespace ethereum_reader\rest_client\core\model;

require_once('answerobject.inc');
require_once('ethereumreaderserver.inc');

require_once(dirname(__FILE__).'/./persistence/accountpersistor.inc');

class Account extends AnswerObject {
	
	private $address;
	private $balance;
	private $nonce;
	private $code;
	private $name;
	private $storage;
	private $firstseen;
	private $transactioncount;
	
	private $currentblocknumber;
	private $new_blocks_seen;
	
	private function __construct ()	{
	}
	
	protected function init($data) {
		
		$this->address = $data['address'];
		$this->balance = $data['balance'];
		$this->nonce = $data['nonce'];
		$this->code = $data['code'];
		$this->name = $data['name'];
		$this->storage = $data['storage'];
		$this->firstseen = $data['firstseen'];
		$this->transactioncount = $data['transactioncount'];
		
		$this->currentblocknumber = $data['currentblocknumber'];
		$this->new_blocks_seen = $data['new_blocks_seen'];
	}
	
	
	public function getAddress() {
		return $this->address;
	}

	public function getBalance() {
		return $this->balance;
	}
	
	public function getNonce() {
		return $this->nonce;
	}
	
	public function getCode() {
		return $this->code;
	}
	
	public function getName() {
		return $this->name;
	}
	
	public function getStorage() {
		return $this->storage;
	}
	
	public function getFirstSeen() {
		return $this->firstseen;
	}
	
	public function getTransactionCount() {
		return $this->transactioncount;
	}
	
	public function getCurrentBlockNumber() {
		return $this->currentblocknumber;
	}
	
	public function getNewBlocksSeen() {
		return $this->new_blocks_seen;
	}
	
	// persistence
	public function read($data) {
		$this->init($data);
	}
	
	protected function create() {
	
	}
	
	public function save($perCon) {
	
		$persistor = new AccountPersistor($this, $perCon);
	
		$persistor->saveAccount();
	}
	
	public function delete() {
	
	}
	
	// static functions
	static public function getAccountFromData($data) {
		$account = new Account();
		
		$account->init($data);
		
		return $account;
	}
	
	static public function getAccountFromAddress($address, $perCon) {
		$account = new Account();
		
		$account->address = $address;
		$persistor = new AccountPersistor($account, $perCon);
		
		$account = $persistor->readAccount();

		return $account;
	}
	
	static public function createAccountForAddress($address) {
		$account = new Account();
		
		$account->address = $address;

		return $account;
	}
	
}



?>