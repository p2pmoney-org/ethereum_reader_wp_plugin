<?php 
namespace ethereum_reader\rest_client\core\model;

require_once('transaction.inc');

use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;


class Transactions extends AnswerObject {
	private $transaction_array = array();
	
	public function getTransactionArray() {
		return $this->transaction_array;
	}
	
	public function getTransactionCount() {
		return count($this->transaction_array);
	}
	
	public function getTransactionRange($from, $to) {
		P2PMoneyAlphaGlobals::logString('Transactions::getTransactionRange called from '.$from.' to '.$to);
		
		$transactions = new Transactions();
		
		$cnt = count($this->transaction_array);
		
		$start = (($from > 0) && ($from < $cnt) ? $from : 0);
		$end = (($to > 0) && ($to < $cnt) ? $to : $cnt - 1);
		
		$j = 0;
		for($i = $start; $i <= $end; $i++) {
			$transactions->transaction_array[$j] = $this->transaction_array[$i];
			$j++;
		}
		
		return $transactions;
	}
	
	public function addTransaction($transaction) {
		if (($transaction instanceof Transaction) === false)
			return;
		
		$count = count($this->transaction_array);
		
		$i = $count;
		
		$this->transaction_array[$i] = $transaction;
	}
	
	public function addTransactions($transactions) {
		if (($transactions instanceof Transactions) === false)
			return;
		
		$txarray = $transactions->transaction_array;
		
		$this->transaction_array = array_merge($this->transaction_array, $txarray);
	}
	
	
	static protected function _compare_descending_transactions($a, $b) {
		// descending time, then descending index
		$aTime = $a->getTime();
		$bTime = $b->getTime();
	
		if ($aTime == $bTime) {
			$aTxIndex = $a->getTransactionIndex();
			$bTxIndex = $b->getTransactionIndex();
			
			if ($aTxIndex == $bTxIndex)
				return 0;
	
			return ($aTxIndex > $bTxIndex) ? -1 : +1;
		}
		return ($aTime > $bTime) ? -1 : +1;
	}
	
	static protected function _compare_ascending_transactions($a, $b) {
		// descending time, then descending index
		$aTime = $a->getTime();
		$bTime = $b->getTime();
	
		if ($aTime == $bTime) {
			$aTxIndex = $a->getTransactionIndex();
			$bTxIndex = $b->getTransactionIndex();
	
			if ($aTxIndex == $bTxIndex)
				return 0;
	
			return ($aTxIndex > $bTxIndex) ? +1 : -1;
		}
		return ($aTime > $bTime) ? +1 : -1;
	}
	
	public function sort($ascending) {
		if ($ascending == 1) {
			usort($this->transaction_array, 'ethereum_reader\rest_client\core\model\Transactions::_compare_ascending_transactions');
		}
		else if ($ascending == -1) {
			usort($this->transaction_array, 'ethereum_reader\rest_client\core\model\Transactions::_compare_descending_transactions');
		}
				
	}
	
}
