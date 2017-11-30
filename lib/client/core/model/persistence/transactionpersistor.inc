<?php 
namespace ethereum_reader\rest_client\core\model;

use p2pmoney\common\core\model\DBConnection;
use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;

require_once('accountpersistor.inc');
require_once(dirname(__FILE__).'/../transactions.inc');

class TransactionPersistor {

	protected $perCon;
	
	protected $transaction;
	
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
		$senderId = AccountPersistor::getAccountId($senderaddr, $dbCon);
		
		$recipientaddr = strtolower($this->transaction->getRecipient());
		$recipient = '"'.$recipientaddr.'"';
		$recipientId = AccountPersistor::getAccountId($recipientaddr, $dbCon);
		
		if (($senderId == -1) && ($recipientId == -1))
			return;
		
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
	
	
	// static
	static protected function getDBTransactionsHashForAddress($address, $dbCon) {
		
		// opening connection
		$dbCon->open();
		
		$tablename = $dbCon->getPrefixedTableName(TransactionPersistor::TABLE_NAME);
		
		$query = 'SELECT * FROM '.$tablename;
		$query .= ' WHERE '.$tablename.'.sender = "'.$address.'"';
		$query .= ' OR '.$tablename.'.recipient = "'.$address.'"';
		$query .= ';';
		
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result =  $dbCon->executeQuery($query);
	
		$hashes = array();
		
		$num_rows = $dbCon->getNumberOfRows($result);
		
		if ($num_rows > 0) {
			for ($i = 0; $i < $num_rows; $i++) {
				$hashes[] = $dbCon->getField($result, $i,'hash');
			}
				
		}
				
		//closing connection
		$dbCon->close();
	
		return $hashes;
	}
	
	static public function getTransactionsHashForAddress($address, $perCon) {
		if (!isset($perCon))
		return;
	
		if ($perCon instanceof DBConnection) {
			return TransactionPersistor::getDBTransactionsHashForAddress($address, $perCon);
		}
	}
	
	static protected function getTransactionFromDBResult($result, $i, $dbCon) {
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
		
		$transaction = Transaction::getTransactionFromData($data);
		
		return $transaction;
	}
	
	
	static protected function getDBLastTransactions($offset, $count, $dbCon) {
		// opening connection
		$dbCon->open();
		
		$tablename = $dbCon->getPrefixedTableName(TransactionPersistor::TABLE_NAME);
		
		$query = 'SELECT * FROM '.$tablename;
		$query .= ' ORDER BY CONCAT(block_id, txIndex) DESC';
		$query .= ' LIMIT '.$count.' OFFSET '.($offset-1);
		$query .= ';';
		
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result = $dbCon->executeQuery($query);
		
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
		
		$transactions = null;
	
		if ($num_rows > 0) {
			$transactions = new Transactions();
			
			for ($i = 0; $i < $num_rows; $i++) {
				$transaction = TransactionPersistor::getTransactionFromDBResult($result, $i, $dbCon);
				
				$transactions->addTransaction($transaction);
			}
			
		}
	
		//closing connection
		$dbCon->close();

		return $transactions;
	}
	
	
	static public function getLastTransactions($offset, $count, $perCon) {
		P2PMoneyAlphaGlobals::logString('TransactionPersistor::getLastTransactions called offset '.$offset.' count '.$count);
		
		if (!isset($perCon))
			return;
		
		if ($perCon instanceof DBConnection) {
			$transactions = TransactionPersistor::getDBLastTransactions($offset, $count, $perCon);
			
			// should check array corresponds to request (how?)
			if (isset($transactions)) {
				$transactionarray = $transactions->getTransactionArray();
					
				if (count($transactionarray) != $count)
					return;
				
				// (what else?)
								
			}
			
			return $transactions;
		}
	}

	static protected function getDBBlockRangeTransactions($startBlockNum, $endBlockNum, $dbCon) {
		// opening connection
		$dbCon->open();
		
		$tablename = $dbCon->getPrefixedTableName(TransactionPersistor::TABLE_NAME);
		
		$query = 'SELECT * FROM '.$tablename;
		$query .= ' WHERE block_id >= '.$startBlockNum;
		$query .= ' AND block_id <= '.$endBlockNum;
		$query .= ';';
		
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result = $dbCon->executeQuery($query);
		
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
		
		$transactions = null;
	
		if ($num_rows > 0) {
			$transactions = new Transactions();
			
			for ($i = 0; $i < $num_rows; $i++) {
				$transaction = TransactionPersistor::getTransactionFromDBResult($result, $i, $dbCon);
				
				$transactions->addTransaction($transaction);
			}
			
		}
	
		//closing connection
		$dbCon->close();

		return $transactions;
	}
	
	static public function getBlockRangeTransactions($startBlockNum, $endBlockNum, $perCon) {
		P2PMoneyAlphaGlobals::logString('TransactionPersistor::getBlockRangeTransactions called start '.$startBlockNum.' end '.$endBlockNum);
		
		if (isset($perCon) && ($perCon instanceof DBConnection)) {
			$transactions = TransactionPersistor::getDBBlockRangeTransactions($startBlockNum, $endBlockNum, $perCon);
			
			// should check array corresponds to request 
			if (isset($transactions)) {
				$transactionarray = $transactions->getTransactionArray();
					
				// (how?)
			}
				
			return $transactions;
		}
	}
}


?>