<?php 
namespace ethereum_reader\rest_client\core\model;

require_once('transaction.inc');

class Transactions extends AnswerObject {
	private $transaction_array = array();
	
	public function getTransactionArray() {
		return $this->transaction_array;
	}
	
	public function addTransaction($transaction) {
		if (($transaction instanceof Transaction) === false)
			return;
		
		$count = count($this->transaction_array);
		
		$i = $count;
		
		$this->transaction_array[$i] = $transaction;
	}
	
}
