<?php
/*
Plugin Name: Ethereum Reader
Plugin URI: https://p2pmoney.org/
Description: Plug-in providing access in your wordpress site to the Ethereum blockchain thanks to a <strong>Ethereum Reader Rest Server</strong> installed on a node. To get started use our site to set up your API key or enjoy anonymous access (limited requests per second).
Version: 0.0.1
Author: P2PMoney.org
Author URI: https://p2pmoney.org/ethereum-reader/
License: GPLv2 or later
Text Domain: Ethereum
*/
defined( 'ABSPATH' ) or die( 'Nope, not accessing this' );

require_once ( dirname(__FILE__) . '/./includes/global.inc');
require_once ( plugin_dir_path(__FILE__) . '/./includes/wp_shortcodes.inc');
require_once ( plugin_dir_path(__FILE__) . '/./includes/wp_activation.inc');

require_once ( ethereum_reader_Globals::getEthereumReaderClientCodeDir().'/core/model/ethereumreaderserver.inc');
require_once ( ethereum_reader_Globals::getEthereumReaderClientCodeDir().'/core/model/block.inc');

ethereum_reader_Globals::logString('loading ethereum_reader plug-in');


class wp_ethereum_reader {

}

// activation, deactivation
register_activation_hook( __FILE__, 'ethereum_reader_plugin_activation' );
register_deactivation_hook( __FILE__, 'ethereum_reader_plugin_deactivation' );
register_uninstall_hook( __FILE__, 'ethereum_reader_plugin_uninstall');

// shortcodes
add_shortcode('ethereum_account_info', 'ethereum_reader_getAccountInfo');
add_shortcode('ethereum_last_blocks_info', 'ethereum_reader_getLastBlocksInfo');
add_shortcode('ethereum_last_transactions_info', 'ethereum_reader_getLastTransactionsInfo');
