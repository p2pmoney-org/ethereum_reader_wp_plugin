<?php 
require_once(dirname(__FILE__) . '/../view/views_styles.inc');


use ethereum_reader\rest_client\core\view\AccountView;

class ethereum_reader_Controller {
	
	static public function getAccountView($viewcontext, $ethereumserver, $account) {
		$content = '';
		
		$view = $viewcontext->getRequestedView();
		
		$accountview = AccountView::createAccountView($ethereumserver, $account);
		
		// overloading styles
		$accountviewstyles = ethereum_reader_views_styles::getAccountViewStyles();
		if (isset($accountviewstyles))
			$accountview->setStyles($accountviewstyles);
		
		
			// display the widget corresponding to the view requested
			switch ($view) {
				case 'account':
					$content .= $accountview->getAccountWidget($viewcontext, $account);
					break;
						
				case 'accounttxs':
					$content .= $accountview->getAccountTransactionsWidget($viewcontext, $ethereumserver, $account);
					break;
						
				default:
					$content .= $accountview->getAccountWidget($viewcontext, $account);
					break;
			}
		
		
		
			return $content;
		
	}
}

?>