<?php
namespace p2pmoney\common\core\model;

use PDO;

Class DbConnection {
	protected $database;
	protected $username;
	protected $password;
	protected $host;
	protected $port;
	
	protected $tableprefix;
	
	private $PDO_Connection;
	
	private $isClosed;
	private $opencount;
	
	private $inCommitTransaction;
	
	private $ExecutedQueries;
	
	public function getName() {
		return $this->database;
	}
	
	public function setName($dbname) {
		$this->database = $dbname;
	}
	
	public function getUserName() {
		return $this->username;
	}
	
	public function setUserName($username) {
		$this->username = $username;
	}
	
	public function getPassword() {
		return $this->password;
	}
	
	public function setPassword($password) {
		$this->password = $password;
	}
	
	public function getHost() {
		return $this->host;
	}
	
	public function setHost($host) {
		$this->host = $host;
	}
	
	public function getPort() {
		return $this->port;
	}
	
	public function setPort($port) {
		$this->port = $port;
	}
	
	public function getTablePrefix() {
		return $this->tableprefix;
	}
	
	public function setTablePrefix($tableprefix) {
		$this->tableprefix = $tableprefix;
	}
	
	// operations
	public function isClosed() {
		return $this->isClosed;
	}
	
	private function connect() {
		$this->isClosed = false;
	}
	
	public function open() {
		if ( (isset($this->inCommitTransaction)) && 
		($this->inCommitTransaction == "START TRANSACTION"))
		return;
		
		$this->connect();
		
		if (!isset($this->opencount) || ($this->opencount == 0)) {
			$this->PDO_Connection = new PDO('mysql:host='.$this->host.';dbname='.$this->database,$this->username,$this->password);
		
			$this->opencount = 1;
		}
		else {
			$this->opencount++;
		}
	}
	
	public function close () {
		if ( (isset($this->inCommitTransaction)) && 
		($this->inCommitTransaction == "START TRANSACTION"))
		return;
		
		if (isset($this->opencount)) {
			$this->opencount --;
			
			if ($this->opencount > 0)
			return;
		}
		else {
			throw new Exception("count not set");
		}
		
		// DEPRECATED	
		//mysql_close();

		unset($this->PDO_Connection);
		
		$this->isClosed = true;
	}
	
	public function getPrefixedTableName($tablename) {
		return $this->tableprefix.$tablename;
	}
	
	public function executeQuery($query) {
		
		// prepare statement do the escaping
		$stmt = $this->PDO_Connection->prepare($query); 
		$stmt->execute();
		
		$result = $stmt->fetchAll();
		
		return $result;
	}
	
	public function getField($result, $rownum, $fieldname) {
		if (isset($result) && isset($rownum)) {
			$row = $result[$rownum];
		}
		else 
			$row = null;
		
		

		if (isset($row) && isset($fieldname) && (isset($row[$fieldname]))) {
			return $row[$fieldname] ;
		}
	}
	
	public function getNumberOfRows($result) {
		if ($result != false) {
			return count($result);
		}
		else 
			return 0;
	}
	
	public function getLastError() {
		$error = "A PDO error has occurred. Error: (" . $this->PDO_Connection->errorCode() . ") " . $this->PDO_Connection->errorInfo();
		
		return $error;
	}
	
	public function startTransaction() {
		$inCommitTransaction = "START TRANSACTION";
		
		return $this->executeQuery('START TRANSACTION;');
	}
	
	public function commitTransaction() {
		$inCommitTransaction = "";
		
		return $this->executeQuery('COMMIT;');
	}
	
	public function escapeString($str) {
		// PDO prepare does the escaping
		return $str;
	}
	
}


?>