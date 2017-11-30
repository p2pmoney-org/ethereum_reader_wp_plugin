<?php 
namespace ethereum_reader\rest_client\core\model;

use p2pmoney\common\core\model\DBConnection;
use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;


class AccountPersistor {
	protected $perCon;
	
	protected $account;
	
	public function __construct(&$account, $perCon) {
		
		// working with reference on $block for the read part
		$this->account =& $account;
		$this->perCon = $perCon;
	}
	
	public function saveAccount() {
		if (!isset($this->perCon))
			return;
		
		if ($this->perCon instanceof DBConnection) {
			$this->saveDB();
			return;
		}
	}
	
	public function readAccount() {
		if (!isset($this->perCon))
			return;
		
		if ($this->perCon instanceof DBConnection) {
			return $this->readDB();
		}
	}
	
	// static
	const TABLE_NAME = 'accounts';
	
	public function getPrefixedTableName() {
		return $this->perCon->getPrefixedTableName(AccountPersistor::TABLE_NAME);
	}

	private function saveDB() {
		$dbCon = $this->perCon;
		
		// DB formatting
		$address = '"'.strtolower($this->account->getAddress()).'"';
		
		$balance = ( $this->account->getBalance() !== null ? '"'.$this->account->getBalance().'"' : "NULL");
		$nonce = ($this->account->getNonce() !== null ? '"'.$this->account->getNonce().'"' : "NULL");
		$code = ($this->account->getCode() !== null ? '"'.$this->account->getCode().'"' : "NULL");
		$name = ($this->account->getName() !== null ? '"'.$this->account->getName().'"' : "NULL");
		$storage = ($this->account->getStorage() !== null ? '"'.$this->account->getStorage().'"' : "NULL");
		$firstseen = ($this->account->getFirstSeen() !== null ? "'".date('Y-m-d H:i:s', $this->account->getFirstSeen())."'" : "NULL");
		$transactioncount = ($this->account->getTransactionCount() !== null ? $this->account->getTransactionCount() : "NULL");
		
		$currentblocknumber = ($this->account->getCurrentBlockNumber() !== null ? $this->account->getCurrentBlockNumber() : "NULL");
		$new_blocks_seen = ($this->account->getNewBlocksSeen() !== null ? $this->account->getNewBlocksSeen() : "NULL");
		
		$firstscannedblock = ($this->account->getFirstScannedBlock() !== null ? $this->account->getFirstScannedBlock() : "NULL");
		$lastscannedblock = ($this->account->getLastScannedBlock() !== null ? $this->account->getLastScannedBlock() : "NULL");
		
		// DB fields
		$specials = $this->readDBSpecials();
		
		$id = $specials['id'];
		$InsertionDate = "'".date('Y-m-d H:i:s', $specials['InsertionDate'])."'"; 
		$LastModificationOn = "'".date('Y-m-d H:i:s', time())."'";
		
		// opening connection
		$dbCon->open();
		
		$tablename = $this->getPrefixedTableName();
		
		if ($id != -1) {
			// update
			$query = "UPDATE ".$tablename."
					SET
					
					LastModificationOn=".$LastModificationOn.",
			
					balance=".$balance.",
					nonce=".$nonce.",
					code=".$code.",
					name=".$name.",
					storage=".$storage.",
					firstseen=".$firstseen.",
					transactioncount=".$transactioncount.",
							
					currentblocknumber=".$currentblocknumber.",
					new_blocks_seen=".$new_blocks_seen.",
					firstscannedblock=".$firstscannedblock.",
					lastscannedblock=".$lastscannedblock."
							
			WHERE id=".$id.";";
		}
		else {
			// first insertion
			$query = "INSERT INTO ".$tablename."
					(
					InsertionDate,
					LastModificationOn,
		
					address,
			
					balance,
					nonce,
					code,
					name,
					storage,
					firstseen,
					transactioncount,
							
					currentblocknumber,
					new_blocks_seen,
					firstscannedblock,
					lastscannedblock
					
					)
					VALUES(
					".$InsertionDate.",
					".$LastModificationOn.",
							
					".$address.",
							
					".$balance.",
					".$nonce.",
					".$code.",
					".$name.",
					".$storage.",
					".$firstseen.",
					".$transactioncount.",
							
					".$currentblocknumber.",
					".$new_blocks_seen.",
					".$firstscannedblock.",
					".$lastscannedblock."
					);";
		}
		
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result =$dbCon->executeQuery($query);
		
		
		//closing connection
		$dbCon->close();
	}
	
	private function readDBSpecials() {
		$dbCon = $this->perCon;
		$account = $this->account;
		$address = strtolower($this->account->getAddress());
		
		// opening connection
		$dbCon->open();
		
		$tablename = $this->getPrefixedTableName();
		
		$query = 'SELECT * FROM '.$tablename.' WHERE '.$tablename.'.address = "'.$address.'";';
		
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result =$dbCon->executeQuery($query);
		
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
	
		if ($num_rows > 0) {
			$i = 0;
			
			$specials = $this->getDBSpecials($result, $i);
		}
		else {
			$specials['id'] = -1;
			
			$specials['InsertionDate'] = time();
			$specials['LastModificationOn ']= time();
		}
		
		//closing connection
		$dbCon->close();
		
		return $specials;
	}
	
	private function getDBSpecials($result, $i) {
		$dbCon = $this->perCon;
		
		$specials['id'] = $dbCon->getField($result, $i,'id');
		
		$specials['InsertionDate'] = strtotime($dbCon->getField($result, $i,'InsertionDate'));
		$specials['LastModificationOn ']= strtotime($dbCon->getField($result, $i,'LastModificationOn'));
		
		return $specials;
	}
	
	private function readDB() {
		$dbCon = $this->perCon;
		$account = $this->account;
		$address = strtolower($this->account->getAddress());
		
		// opening connection
		$dbCon->open();
	
		$tablename = $this->getPrefixedTableName();
	
		$query = 'SELECT * FROM '.$tablename.' WHERE '.$tablename.'.address = "'.$address.'";';
	
		//P2PMoneyAlphaGlobals::logString('query is '.$query);
		
		$result = $dbCon->executeQuery($query);
	
		if ($result != false)
			$num_rows = $dbCon->getNumberOfRows($result);
		else
			$num_rows = 0;
	
		if ($num_rows > 0) {
			$i = 0;
			
			$specials = $this->getDBSpecials($result, $i);
			
			$data['address'] = $dbCon->getField($result, $i,'address');
			$data['balance'] = $dbCon->getField($result, $i,'balance');
			$data['nonce'] = $dbCon->getField($result, $i,'nonce');
			$data['code'] = $dbCon->getField($result, $i,'code');
			$data['name'] = $dbCon->getField($result, $i,'name');
			$data['storage'] = $dbCon->getField($result, $i,'storage');
			$data['firstseen'] = strtotime($dbCon->getField($result, $i,'firstseen'));
			$data['transactioncount'] = $dbCon->getField($result, $i,'transactioncount');
			
			$data['currentblocknumber'] = $dbCon->getField($result, $i,'currentblocknumber');
			$data['new_blocks_seen'] = $dbCon->getField($result, $i,'new_blocks_seen');
				
			$data['firstscannedblock'] = $dbCon->getField($result, $i,'firstscannedblock');
			$data['lastscannedblock'] = $dbCon->getField($result, $i,'lastscannedblock');

			$account->read($data);
		}
		else {
			$account = null;
		}

		//closing connection
		$dbCon->close();
		
		return $account;
	}
	
	
	static public function getAccountId($address, $percon) {
		$account = Account::createAccountForAddress($address);
		
		$persistor = new AccountPersistor($account, $percon);
		
		if ($persistor->perCon instanceof DBConnection) {
			$specials = $persistor->readDBSpecials();
			
			return $specials['id'];
		}
		
	}
	
	static public function getSafeAccountId($address, $percon) {
		// safe means we create an account if we can't get an id
		$id = AccountPersistor::getAccountId($address, $percon);
		
		if ($id == -1) {
			$account = Account::createAccountForAddress($address);
			
			$account->save($percon);
			
			$id = AccountPersistor::getAccountId($address, $percon);
		}
		
		return $id;
	}
}


	
	?>