<?php 

/*
 *  You can change the styles in this file to customize
 *  the rendering of ethereum_reader shortcodes according
 *  to the look and feel of your wordpress site
 */


// overloading account view styles
class ethereum_reader_views_styles {
	static public function getAccountViewStyles() {
		$accountviewstyles['widget'] = 'font-family: Arial Black;';
		
		$accountviewstyles['top_banner'] = 'background-color: #82b3e657;margin: 10px;padding: 30px;color: #3e5fb1;font-size: 90%;font-weight:800;';
		$accountviewstyles['top_banner_table'] = 'color: #3e5fb1;margin-top: 30px';
		
		$accountviewstyles['nav_band'] = 'margin-top: 35px;margin-bottom: 15px;float:right;font-size: 80%';
		
		
		$accountviewstyles['account_details'] = 'clear:both;color: #285b82;background-color: #e8e6e6;';
		
		$accountviewstyles['txs_paging'] = 'clear:both;font-size: 80%;color:#382020';
		$accountviewstyles['txs_nav_button'] = 'float:right;width: 60px;text-align:center;border: 1px solid #b3bfb3;padding: 2px;margin-top: 10px;margin-left:5px;margin-bottom: 20px;;background-color: #e9ebf1;';
		$accountviewstyles['txs_nav_page_button'] = 'float:right;width: 60px;text-align:center;border: 1px solid #b3bfb3;padding: 2px;;margin-top: 10px;margin-left:5px;margin-bottom: 20px;background-color: #c9d3e6;';
		
		$accountviewstyles['txs_list'] = 'clear:both;color:#423a3a';
		$accountviewstyles['txs_header_row'] = 'font-weight: bold;background-color: #eae3dd;';
		$accountviewstyles['txs_even_row'] = 'font-size: 75%;';
		$accountviewstyles['txs_odd_row'] = 'background-color: #d7edfb;font-size: 75%;';
		
		return $accountviewstyles;
	}
}


?>