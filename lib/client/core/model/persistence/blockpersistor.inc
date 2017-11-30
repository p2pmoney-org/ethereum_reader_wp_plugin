<?php 
namespace ethereum_reader\rest_client\core\model;

use p2pmoney\common\core\model\DBConnection;
use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;

require_once(dirname(__FILE__).'/../blocks.inc');


class BlockPersistor {
	protected $perCon;
	
	protected $block;
	
	public function __construct(&$block, $perCon) {
		
		// working with reference on $block for the read part
		$this->block =& $block;
		$this->perCon = $perCon;
	}
	
	public function saveBlock() {
		if (!isset($this->perCon))
			return;
		
		if ($this->perCon instanceof DBConnection) {
			$this->saveDB();
			return;
		}
	}
	
	public function readBlock() {
		if (!isset($this->perCon))
			return;
		
		if ($this->perCon instanceof DBConnection) {
			return $this->readDB();
		}
	}
	
	// static
	const TABLE_NAME = 'blocks';
	
	
	public function getPrefixedTableName() {
		return $this->perCon->getPrefixedTableName(BlockPersistor::TABLE_NAME);
	}

	private function saveDB() {
		$dbCon = $this->perCon;
		
		// DB formatting
		$number = $this->block->getNumber();
		$hash = '"'.strtolower($this->block->getHash()).'"';
		$parentHash = '"'.strtolower($this->block->getParentHash()).'"';
		$uncleHash = '"'.strtolower($this->block->getUncleHash()).'"';
		$coinbase = '"'.strtolower($this->block->getCoinBase()).'"';
		$root = '"'.strtolower($this->block->getRoot()).'"';
		$txHash = '"'.strtolower($this->block->getTxHash()).'"';
		$difficulty = $this->block->getDifficulty();
		$gasLimit = $this->block->getGasLimit();
		$gasUsed = $this->block->getGasUsed();
		$time = ($this->block->getTime() != null ? "'".date('Y-m-d H:i:s', $this->block->getTime())."'" : "NULL");
		$extra = $this->block->getExtra();
		$mixDigest = ($this->block->getMixDigest() != null ? "'".$this->block->getMixDigest()."'" : "''");
		$nonce = '"'.$this->block->getNonce().'"';
		$tx_count = $this->block->getTxCount();
		$uncle_count = $this->block->getUncleCount();
		$size = $this->block->getSize();
		$blockTime = $this->block->getBlockTime();
		$totalDifficulty = '"'.$this->block->getTotalDifficulty().'"';
		$reward = ($this->block->getReward() != null ? $this->block->getReward() : -1);
		$totalFee = ($this->block->getTotalFee() != null ? $this->block->getTotalFee() : -1);
		
		// opening connection
		$dbCon->open();
		
		$tablename = $this->getPrefixedTableName();
		
		$query = "INSERT INTO ".$tablename."
				(
				number,
				hash,
				parentHash,
				uncleHash,
				coinbase,
				root,
				txHash,
				difficulty,
				gasLimit,
				gasUsed,
				time,
				extra,
				mixDigest,
				nonce,
				tx_count,
				uncle_count,
				size,
				blockTime,
				totalDifficulty,
				reward,
				totalFee
				
							
				)
				VALUES(
				".$number.",
				".$hash.",
				".$parentHash.",
				".$uncleHash.",
				".$coinbase.",
				".$root.",
				".$txHash.",
				".$difficulty.",
				".$gasLimit.",
				".$gasUsed.",
				".$time.",
				".$extra.",
				".$mixDigest.",
				".$nonce.",
				".$tx_count.",
				".$uncle_count.",
				".$size.",
				".$blockTime.",
				".$totalDifficulty.",
				".$reward.",
				".$totalFee."
						
				);";
		
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result =$dbCon->executeQuery($query);
		
		//closing connection
		$dbCon->close();
		
	}
	
	private function readDB() {
		$dbCon = $this->perCon;
		$block = $this->block;
		$blocknumber = $this->block->getNumber();
		
		// opening connection
		$dbCon->open();
	
		$tablename = $this->getPrefixedTableName();
	
		$query = 'SELECT * FROM '.$tablename.' WHERE '.$tablename.'.number = '.$blocknumber.';';
	
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result = $dbCon->executeQuery($query);
	
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
	
		if ($num_rows > 0) {
			$i = 0;
			
			$data['number'] = $dbCon->getField($result, $i,'number');
			$data['hash'] = $dbCon->getField($result, $i,'hash');
			$data['parentHash'] = $dbCon->getField($result, $i,'parentHash');
			$data['uncleHash'] = $dbCon->getField($result, $i,'uncleHash');
			$data['coinbase'] = $dbCon->getField($result, $i,'coinbase');
			$data['root'] = $dbCon->getField($result, $i,'root');
			$data['txHash'] = $dbCon->getField($result, $i,'txHash');
			$data['difficulty'] = $dbCon->getField($result, $i,'difficulty');
			$data['gasLimit'] = $dbCon->getField($result, $i,'gasLimit');
			$data['gasUsed'] = $dbCon->getField($result, $i,'gasUsed');
			$data['time'] = strtotime($dbCon->getField($result,$i,'time'));
			$data['extra'] = $dbCon->getField($result, $i,'extra');
			$data['mixDigest'] = $dbCon->getField($result, $i,'mixDigest');
			$data['nonce'] = $dbCon->getField($result, $i,'nonce');
			$data['tx_count'] = $dbCon->getField($result, $i,'tx_count');
			$data['uncle_count'] = $dbCon->getField($result, $i,'uncle_count');
			$data['size'] = $dbCon->getField($result, $i,'size');
			$data['blockTime'] = $dbCon->getField($result, $i,'blockTime');
			$data['totalDifficulty'] = $dbCon->getField($result, $i,'totalDifficulty');
			$data['reward'] = $dbCon->getField($result, $i,'reward');
			$data['totalFee'] = $dbCon->getField($result, $i,'totalFee');	
				
			$block->read($data);
		}
		else {
			$block = null;
		}

		//closing connection
		$dbCon->close();
		
		return $block;
	}
	
	
	// static functions
	static protected function getDBLastBlockNumber($dbCon) {
		// opening connection
		$dbCon->open();
	
		$tablename = $dbCon->getPrefixedTableName(BlockPersistor::TABLE_NAME);
	
		$query = 'SELECT MAX(number) AS lastblocknumber FROM '.$tablename.';';
	
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result = $dbCon->executeQuery($query);
		
		$maxblocknumber = null;
		
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
		
		if ($num_rows > 0) {
			$i = 0;
				
			$maxblocknumber = $dbCon->getField($result, $i,'lastblocknumber');
		}
		
		//closing connection
		$dbCon->close();
		
		return $maxblocknumber;
	}
	
	static public function getLastBlockNumber($perCon) {
		if (isset($perCon) && ($perCon instanceof DBConnection)) {
			return BlockPersistor::getDBLastBlockNumber($perCon);
		}
			
	}
	
	
	static protected function getDBBlockRange($startBlockNum, $endBlockNum, $dbCon) {
		
		// opening connection
		$dbCon->open();
	
		$tablename = $dbCon->getPrefixedTableName(BlockPersistor::TABLE_NAME);
		
		$query = 'SELECT * FROM '.$tablename;
		$query .= ' WHERE '.$tablename.'.number >= '.$startBlockNum;
		$query .= ' AND '.$tablename.'.number <= '.$endBlockNum;
		$query .= ';';
		
		P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result = $dbCon->executeQuery($query);
	
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
		
		$blocks = null;
	
		if ($num_rows > 0) {
			$blocks = new Blocks();
			
			for($i = 0; $i < $num_rows; $i++) {
				$data['number'] = $dbCon->getField($result, $i,'number');
				$data['hash'] = $dbCon->getField($result, $i,'hash');
				$data['parentHash'] = $dbCon->getField($result, $i,'parentHash');
				$data['uncleHash'] = $dbCon->getField($result, $i,'uncleHash');
				$data['coinbase'] = $dbCon->getField($result, $i,'coinbase');
				$data['root'] = $dbCon->getField($result, $i,'root');
				$data['txHash'] = $dbCon->getField($result, $i,'txHash');
				$data['difficulty'] = $dbCon->getField($result, $i,'difficulty');
				$data['gasLimit'] = $dbCon->getField($result, $i,'gasLimit');
				$data['gasUsed'] = $dbCon->getField($result, $i,'gasUsed');
				$data['time'] = strtotime($dbCon->getField($result,$i,'time'));
				$data['extra'] = $dbCon->getField($result, $i,'extra');
				$data['mixDigest'] = $dbCon->getField($result, $i,'mixDigest');
				$data['nonce'] = $dbCon->getField($result, $i,'nonce');
				$data['tx_count'] = $dbCon->getField($result, $i,'tx_count');
				$data['uncle_count'] = $dbCon->getField($result, $i,'uncle_count');
				$data['size'] = $dbCon->getField($result, $i,'size');
				$data['blockTime'] = $dbCon->getField($result, $i,'blockTime');
				$data['totalDifficulty'] = $dbCon->getField($result, $i,'totalDifficulty');
				$data['reward'] = $dbCon->getField($result, $i,'reward');
				$data['totalFee'] = $dbCon->getField($result, $i,'totalFee');
				
				$block = Block::getBlockFromData($data);
				
				$blocks->addBlock($block);
			}
			
		}

		//closing connection
		$dbCon->close();
		
		return $blocks;
	}
	
	static public function getBlockRange($startBlockNum, $endBlockNum, $perCon) {
		P2PMoneyAlphaGlobals::logString('BlockPersistor::getBlockRange called start '.$startBlockNum.' end '.$endBlockNum);
		
		if (isset($perCon) && ($perCon instanceof DBConnection)) {
			$blocks = BlockPersistor::getDBBlockRange($startBlockNum, $endBlockNum, $perCon);
			
			// check that we have all the blocks
			$blockarray = $blocks->getBlockArray();
			
			if ((!isset($blockarray[0])) || ($blockarray[0]->getNumber() != $startBlockNum))
				return;
			
			if (count($blockarray) != ($endBlockNum + 1 - $startBlockNum))
				return;
				
			P2PMoneyAlphaGlobals::logString('BlockPersistor::getBlockRange returning '.count($blockarray).' blocks.');
			return $blocks;
		}
			
	}
	
}



?>