<?php 


namespace ethereum_reader\rest_client\core\model;

require_once('answerobject.inc');
require_once('ethereumreaderserver.inc');

require_once(dirname(__FILE__).'/./persistence/transactionpersistor.inc');

class Transaction extends AnswerObject {
	
	private $hash;
	private $sender;
	private $recipient;
	private $accountNonce;
	private $price;
	private $gasLimit;
	private $amount;
	private $block_id;
	private $time;
	private $newContract;
	private $isContractTx;
	private $blockHash;
	private $parentHash;
	private $txIndex;
	private $gasUsed;
	private $type;
	
	private function __construct ()	{
	}
	
	
	public function init($data) {
		$this->hash = $data['hash'];
		$this->sender = $data['sender'];
		$this->recipient = $data['recipient'];
		$this->accountNonce = $data['accountNonce'];
		$this->price = $data['price'];
		$this->gasLimit = $data['gasLimit'];
		$this->amount = $data['amount'];
		$this->block_id = $data['block_id'];
		$this->time = $data['time'];
		$this->newContract = $data['newContract'];
		$this->isContractTx = $data['isContractTx'];
		$this->blockHash = $data['blockHash'];
		$this->parentHash = $data['parentHash'];
		$this->txIndex = $data['txIndex'];
		$this->gasUsed = $data['gasUsed'];
		$this->type = $data['type'];
	}

	public function getHash() {
		return $this->hash;
	}

	public function getSender() {
		return $this->sender;
	}
	
	public function getRecipient() {
		return $this->recipient;
	}
	
	public function getAccountNonce() {
		return $this->accountNonce;
	}
	
	public function getPrice() {
		return $this->price;
	}
	
	public function getGasLimit() {
		return $this->gasLimit;
	}
	
	public function getAmount() {
		return $this->amount;
	}
	
	public function getBlockId() {
		return $this->block_id;
	}
	
	public function getTime() {
		return $this->time;
	}
	
	public function getNewContract() {
		return $this->newContract;
	}
	
	public function getIsContractTx() {
		return $this->isContractTx;
	}
	
	public function getBlockHash() {
		return $this->blockHash;
	}
	
	public function getParentHash() {
		return $this->parentHash;
	}
	
	public function getTransactionIndex() {
		return $this->txIndex;
	}
	
	public function getGasUsed() {
		return $this->gasUsed;
	}
	
	public function getType() {
		return $this->type;
	}
	
	
	
	// persistence
	public function read($data) {
		$this->init($data);
	}
	
	protected function create() {
	
	}
	
	public function save($perCon) {
	
		$persistor = new TransactionPersistor($this, $perCon);
	
		$persistor->saveTransaction();
	}
	
	public function delete($perCon) {
	
	}
	

	// static functions
	static public function getTransactionFromData($data) {
		$transaction = new Transaction();
	
		$transaction->init($data);
	
		return $transaction;
	}
	
	static public function getTransactionFromHash($hash, $perCon) {
		$transaction = new Transaction();
		$transaction->hash = $hash;
		
		$persistor = new TransactionPersistor($transaction, $perCon);
		
		$transaction = $persistor->readTransaction();
		
		return $transaction;
	}
	
	static public function getTransactionsForAddress($address, $perCon) {
		
		$hashes = TransactionPersistor::getTransactionsHashForAddress($address, $perCon);
		
		if (!isset($hashes))
			return;
		
		$transactions = new Transactions();
		
		foreach($hashes as $hash) {
			$transaction = new Transaction();
			$transaction->hash = $hash;
			
			$persistor = new TransactionPersistor($transaction, $perCon);
			
			$transaction = $persistor->readTransaction();
				
			if (isset($transaction))
				$transactions->addTransaction($transaction);
		}
	
		return $transactions;
	}

	static public function getLastTransactions($offset, $count, $perCon) {
		return TransactionPersistor::getLastTransactions($offset, $count, $perCon);
	}
	
	static public function getBlockRangeTransactions($startBlockNum, $endBlockNum, $perCon) {
		return TransactionPersistor::getBlockRangeTransactions($startBlockNum, $endBlockNum, $perCon);
	}
	
}


?>