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
	
	private $firstscannedblock;
	private $lastscannedblock;
	
	// transactions
	private $transactions;
	
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
		
		$this->firstscannedblock = (isset($data['firstscannedblock']) ? $data['firstscannedblock'] : -1);
		$this->lastscannedblock = (isset($data['lastscannedblock']) ? $data['lastscannedblock'] : -1);
	}
	
	public function getData() {
		$data['address'] = $this->address;
		$data['balance'] = $this->balance;
		$data['nonce'] = $this->nonce;
		$data['code'] = $this->code;
		$data['name'] = $this->name;
		$data['storage'] = $this->storage;
		$data['firstseen'] = $this->firstseen;
		$data['transactioncount'] = $this->transactioncount;
		
		$data['currentblocknumber'] = $this->currentblocknumber;
		$data['new_blocks_seen'] = $this->new_blocks_seen;
		
		$data['firstscannedblock'] = $this->firstscannedblock;
		$data['lastscannedblock'] = $this->lastscannedblock;
		
		return $data;
	}
	
	public function updateAccount($account) {
		if (!($account instanceof Account))
			return;
		
		$this->firstscannedblock = (isset($account->firstscannedblock) && ($account->firstscannedblock != -1) ? $account->firstscannedblock : $this->firstscannedblock);
		$this->lastscannedblock = (isset($account->lastscannedblock) && ($account->lastscannedblock != -1) ? $account->lastscannedblock : $this->lastscannedblock);
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
	
	public function getTransactions($perCon) {
		if (isset($this->transactions))
			return $this->transactions;
		
		// we get identified transactions for this account
		$address = $this->address;
		$this->transactions = Transaction::getTransactionsForAddress($address, $perCon);
			
		return $this->transactions;
	}
	
	public function addTransactions($transaction) {
		// we could perfom a check if transactions belong to this account
		$this->transactions->addTransactions($transactions);
	}
		
	public function getCurrentBlockNumber() {
		return $this->currentblocknumber;
	}
	
	public function getNewBlocksSeen() {
		return $this->new_blocks_seen;
	}
	
	public function getFirstScannedBlock() {
		return $this->firstscannedblock;
	}
	
	public function setFirstScannedBlock($firstscannedblock) {
		$this->firstscannedblock = $firstscannedblock;
	}
	
	public function getLastScannedBlock() {
		return $this->lastscannedblock;
	}
	
	public function setLastScannedBlock($lastscannedblock) {
		$this->lastscannedblock = $lastscannedblock;
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
	
	public function delete($perCon) {
	
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