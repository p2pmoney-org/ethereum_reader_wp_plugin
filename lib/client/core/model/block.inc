<?php 
namespace ethereum_reader\rest_client\core\model;

require_once('answerobject.inc');
require_once('ethereumreaderserver.inc');


class Block extends AnswerObject {
	// web3 members
	private $difficulty;
	private $extraData;
	private $gasLimit;
	private $gasUsed;
	private $hash;
	private $logsBloom;
	private $miner;
	private $mixHash;
	private $nonce;
	private $number;
	private $parentHash;
	private $receiptsRoot;
	private $sha3Uncles;
	private $size;
	private $stateRoot;
	private $timestamp;
	private $totalDifficulty;
	private $transactions;
	private $transactionsRoot;
	private $uncles;
	
	private function __construct ()	{
	}
	
	private function init($data) {
		$this->difficulty = $data['difficulty'];
		$this->extraData = $data['extraData'];
		$this->gasLimit = $data['gasLimit'];
		$this->gasUsed = $data['gasUsed'];
		$this->hash = $data['hash'];
		$this->logsBloom = $data['logsBloom'];
		$this->miner = $data['miner'];
		$this->mixHash = $data['mixHash'];
		$this->nonce = $data['nonce'];
		$this->number = $data['number'];
		$this->parentHash = $data['parentHash'];
		$this->receiptsRoot = $data['receiptsRoot'];
		$this->sha3Uncles = $data['sha3Uncles'];
		$this->size = $data['size'];
		$this->stateRoot = $data['stateRoot'];
		$this->timestamp = $data['timestamp'];
		$this->totalDifficulty = $data['totalDifficulty'];
		$this->transactions = $data['transactions'];
		$this->transactionsRoot = $data['transactionsRoot'];
		$this->uncles = $data['uncles'];
		
	}
	
	public function getNumber() {
		return $this->number;
	}
	
	public function getDifficulty() {
		return $this->difficulty;
	}
	
	public function getExtraData() {
		return $this->extraData;
	}
	
	public function getGasLimit() {
		return $this->gasLimit;
	}
	
	public function getGasUsed() {
		return $this->gasUsed;
	}
	
	public function getHash() {
		return $this->hash;
	}
	
	public function getLogsBloom() {
		return $this->logsBloom;
	}
	
	public function getMiner() {
		return $this->miner;
	}
	
	public function getMixHash() {
		return $this->mixHash;
	}
	
	public function getNonce() {
		return $this->nonce;
	}
	
	public function getParentHash() {
		return $this->parentHash;
	}
	
	public function getReceiptsRoot() {
		return $this->receiptsRoot;
	}
	
	public function getSha3Uncles() {
		return $this->sha3Uncles;
	}
	
	public function getSize() {
		return $this->size;
	}
	
	public function getStateRoot() {
		return $this->stateRoot;
	}
	
	public function getTimeStamp() {
		return $this->timestamp;
	}
	
	public function getTotalDifficulty() {
		return $this->totalDifficulty;
	}
	
	public function getTransactionsRoot() {
		return $this->transactionsRoot;
	}
	
	// static part
	static $lastblock;
	
	static public function getBlock($data) {
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