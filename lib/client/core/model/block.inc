<?php 
namespace ethereum_reader\rest_client\core\model;

require_once('answerobject.inc');
require_once('ethereumreaderserver.inc');

require_once(dirname(__FILE__).'/./persistence/blockpersistor.inc');

class Block extends AnswerObject {
	
	// api members
	private $number;
	private $hash;
	private $parentHash;
	private $uncleHash;
	private $coinbase;
	private $root;
	private $txHash;
	private $difficulty;
	private $gasLimit;
	private $gasUsed;
	private $time;
	private $extra;
	private $mixDigest;
	private $nonce;
	private $tx_count;
	private $uncle_count;
	private $size;
	private $blockTime;
	private $reward;
	private $totalFee;	
	
	private $totalDifficulty;
	
	// web3 additionals
	//private $difficulty;
	//private $extraData;
	//private $gasLimit;
	//private $gasUsed;
	//private $hash;
	private $logsBloom;
	private $mixHash;
	//private $nonce;
	//private $number;
	//private $parentHash;
	private $receiptsRoot;
	//private $sha3Uncles;
	//private $size;
	//private $stateRoot;
	//private $timestamp;
	private $transactionsRoot;
	
	// arrays
	private $transactions;
	private $uncles;
	
	private function __construct ()	{
	}
	
	protected function init($data) {
		if ((isset($this->number) && $this->number != $data['number']))
			return;
		
		$this->number = $data['number'];
		$this->hash = $data['hash'];
		$this->parentHash = $data['parentHash'];
		$this->uncleHash = $data['uncleHash'];
		$this->coinbase = $data['coinbase'];
		$this->root = $data['root'];
		$this->txHash = $data['txHash'];
		$this->difficulty = $data['difficulty'];
		$this->gasLimit = $data['gasLimit'];
		$this->gasUsed = $data['gasUsed'];
		$this->time = $data['time'];
		$this->extra = $data['extra'];
		$this->mixDigest = $data['mixDigest'];
		$this->nonce = $data['nonce'];
		$this->tx_count = $data['tx_count'];
		$this->uncle_count = $data['uncle_count'];
		$this->size = $data['size'];
		$this->blockTime = $data['blockTime'];
		
		$this->totalDifficulty = $data['totalDifficulty'];
		
		$this->reward = $data['reward'];
		$this->totalFee = $data['totalFee'];	
		
	}
	
	// get/set api members
	public function getNumber() {
		return $this->number;
	}
	
	public function getHash() {
		return $this->hash;
	}
	
	public function getParentHash() {
		return $this->parentHash;
	}
	
	public function getUncleHash() {
		return $this->uncleHash;
	}
	
	public function getCoinBase() {
		return $this->coinbase;
	}
	
	public function getRoot() {
		return $this->root;
	}
	
	public function getTxHash() {
		return $this->txHash;
	}
	
	public function getDifficulty() {
		return $this->difficulty;
	}
	
	public function getGasLimit() {
		return $this->gasLimit;
	}
	
	public function getGasUsed() {
		return $this->gasUsed;
	}
	
	public function getTime() {
		return $this->time;
	}
	
	public function getExtra() {
		return $this->extra;
	}
	
	public function getMixDigest() {
		return $this->mixDigest;
	}
	
	public function getNonce() {
		return $this->nonce;
	}
	
	public function getTxCount() {
		return $this->tx_count;
	}
	
	public function getUncleCount() {
		return $this->uncle_count;
	}
	
	public function getSize() {
		return $this->size;
	}
	
	public function getBlockTime() {
		return $this->blockTime;
	}
	
	public function getMiner() {
		return $this->coinbase;
	}
	
	public function getTotalDifficulty() {
		return $this->totalDifficulty;
	}
	
	public function getReward() {
		return $this->reward;
	}
	
	public function getTotalFee() {
		return $this->totalFee;
	}
	
	
	// persistence
	public function read($data) {
		$this->init($data);
	}
	
	protected function create() {
		
	}
	
	public function save($perCon) {
		
		$persistor = new BlockPersistor($this, $perCon);
		
		$persistor->saveBlock();
	}
	
	public function delete() {
		
	}
	
	// static part
	static protected $lastblock;
	
	static public function getBlockFromNumber($blocknumber, $perCon) {
		
		$block = new Block();
		$block->number = $blocknumber;
		$persistor = new BlockPersistor($block, $perCon);
		
		$block = $persistor->readBlock();

		return $block;
	}
	
	static public function getBlockFromData($data) {
		$block = new Block();
		
		$block->init($data);
		
		return $block;
	}
	
	static public function getLastBlockNumber() {
		if (isset(Block::$lastblock))
			return Block::$lastblock;
		
		Block::$lastblock = EthereumReaderServer::getServer()->getBlocksCount();
		
		return Block::$lastblock;
	}
	
	
}

?>