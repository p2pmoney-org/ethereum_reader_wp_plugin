<?php 
namespace ethereum_reader\rest_client\core\model;

require_once('account.inc');

class Accounts extends AnswerObject {
	private $account_array = array();
	
	public function getAccountArray() {
		return $this->account_array;
	}
	
	public function addAccount($account) {
		if (($account instanceof Account) === false)
			return;
		
		$count = count($this->account_array);
		
		$i = $count;
		
		$this->account_array[$i] = $account;
	}
	
}
