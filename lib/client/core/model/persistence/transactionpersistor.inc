<?php 
namespace ethereum_reader\rest_client\core\model;

use p2pmoney\common\core\model\DBConnection;
use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;

require_once('accountpersistor.inc');

class TransactionPersistor {

	private $perCon;
	
	private $transaction;
	
	public function __construct(&$transaction, $perCon) {
	
		// working with reference on $block for the read part
		$this->transaction =& $transaction;
		$this->perCon = $perCon;
	}
	
	public function saveTransaction() {
		if (!isset($this->perCon))
			return;
	
			if ($this->perCon instanceof DBConnection) {
				$this->saveDB();
				return;
			}
	}
	
	public function readTransaction() {
		if (!isset($this->perCon))
			return;
	
			if ($this->perCon instanceof DBConnection) {
				return $this->readDB();
			}
	}
	
	// static
	const TABLE_NAME = 'transactions';
	
	public function getPrefixedTableName() {
		return $this->perCon->getPrefixedTableName(TransactionPersistor::TABLE_NAME);
	}
	
	private function saveDB() {
		$dbCon = $this->perCon;
		
		// DB formatting
		$hash = '"'.strtolower($this->transaction->getHash()).'"';
		
		$senderaddr = strtolower($this->transaction->getSender());
		$sender = '"'.$senderaddr.'"';
		$senderId = AccountPersistor::getSafeAccountId($senderaddr, $dbCon);
		
		$recipientaddr = strtolower($this->transaction->getRecipient());
		$recipient = '"'.$recipientaddr.'"';
		$recipientId = AccountPersistor::getSafeAccountId($recipientaddr, $dbCon);
		
		$accountNonce = $this->transaction->getAccountNonce();
		$price = $this->transaction->getPrice();
		$gasLimit = $this->transaction->getGasLimit();
		$amount = $this->transaction->getAmount();
		$block_id = $this->transaction->getBlockId();
		$time = ($this->transaction->getTime() != null ? "'".date('Y-m-d H:i:s', $this->transaction->getTime())."'" : "NULL");
		$newContract = $this->transaction->getNewContract();
		$isContractTx = '"'.$this->transaction->getHash().'"';
		$blockHash = '"'.strtolower($this->transaction->getBlockHash()).'"';
		$parentHash = '"'.strtolower($this->transaction->getParentHash()).'"';
		$txIndex = $this->transaction->getTransactionIndex();
		$gasUsed = $this->transaction->getGasUsed();
		$type = '"'.$this->transaction->getType().'"';
		
		// opening connection
		$dbCon->open();
		
		$tablename = $this->getPrefixedTableName();
		
		$query = "INSERT INTO ".$tablename."
				(
				hash ,
				sender,
				senderId,
				recipient,
				recipientId,
				accountNonce,
				price,
				gasLimit,
				amount,
				block_id,
				time,
				newContract,
				isContractTx,
				blockHash,
				parentHash,
				txIndex,
				gasUsed,
				type
				
				)
				VALUES(
				
				".$hash.",
				".$sender.",
				".$senderId.",
				".$recipient.",
				".$recipientId.",
				".$accountNonce.",
				".$price.",
				".$gasLimit.",
				".$amount.",
				".$block_id.",
				".$time.",
				".$newContract.",
				".$isContractTx.",
				".$blockHash.",
				".$parentHash.",
				".$txIndex.",
				".$gasUsed.",
				".$type."
				
				);";
		
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result =$dbCon->executeQuery($query);
		
		//closing connection
		$dbCon->close();
		
	}
	
	
	private function readDB() {
		$dbCon = $this->perCon;
		$transaction = $this->transaction;
		$hash = $this->transaction->getHash();
		
		// opening connection
		$dbCon->open();
	
		$tablename = $this->getPrefixedTableName();
	
		$query = 'SELECT * FROM '.$tablename.' WHERE '.$tablename.'.hash = "'.$hash.'";';
	
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result = $dbCon->executeQuery($query);
	
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
	
		if ($num_rows > 0) {
			$i = 0;
			
			$data['hash'] = $dbCon->getField($result, $i,'hash');
			$data['sender'] = $dbCon->getField($result, $i,'sender');
			$data['recipient'] = $dbCon->getField($result, $i,'recipient');
			$data['accountNonce'] = $dbCon->getField($result, $i,'accountNonce');
			$data['price'] = $dbCon->getField($result, $i,'price');
			$data['gasLimit'] = $dbCon->getField($result, $i,'gasLimit');
			$data['amount'] = $dbCon->getField($result, $i,'amount');
			$data['block_id'] = $dbCon->getField($result, $i,'block_id');
			$data['time'] = strtotime($dbCon->getField($result, $i,'time'));
			$data['newContract'] = $dbCon->getField($result, $i,'newContract');
			$data['isContractTx'] = $dbCon->getField($result, $i,'isContractTx');
			$data['blockHash'] = $dbCon->getField($result, $i,'blockHash');
			$data['parentHash'] = $dbCon->getField($result, $i,'parentHash');
			$data['txIndex'] = $dbCon->getField($result, $i,'txIndex');
			$data['gasUsed'] = $dbCon->getField($result, $i,'gasUsed');
			$data['type'] = $dbCon->getField($result, $i,'type');
				
			$transaction->read($data);
		}
		else {
			$transaction = null;
		}

		//closing connection
		$dbCon->close();
		
		return $transaction;
	}
}


?>