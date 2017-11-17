<?php 

function ethereum_reader_plugin_activation() {
	ethereum_reader_Globals::logString("ethereum_reader_plugin_activation called");
	
	// create tables
	ethereum_reader_create_db_tables();
}

function ethereum_reader_create_db_tables() {
	global $wpdb;
	
	require_once( ABSPATH . 'wp-admin/includes/upgrade.php' );
	
	$tableprefix = ethereum_reader_Globals::getPluginTablePrefix();
	
	// blocks
	$table_name = $tableprefix.'blocks';
	$charset_collate = $wpdb->get_charset_collate();
	
	$sql = "CREATE TABLE  IF NOT EXISTS $table_name (
	  `number` int(11) NOT NULL DEFAULT '0',
	  `hash` varchar(128) NOT NULL,
	  `parentHash` varchar(128) NOT NULL,
	  `uncleHash` varchar(128) NOT NULL,
	  `coinbase` varchar(64) NOT NULL,
	  `root` varchar(128) NOT NULL,
	  `txHash` varchar(128) NOT NULL,
	  `difficulty` bigint(20) NOT NULL,
	  `gasLimit` int(11) NOT NULL,
	  `gasUsed` int(11) NOT NULL,
	  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	  `extra` varchar(256) NOT NULL,
	  `mixDigest` varchar(256) NOT NULL,
	  `nonce` varchar(64) NOT NULL,
	  `tx_count` int(11) NOT NULL,
	  `uncle_count` int(11) NOT NULL,
	  `size` int(11) NOT NULL,
	  `blockTime` int(11) NOT NULL,
	  `totalDifficulty` varchar(32) NOT NULL,
	  `reward` int(11) NOT NULL,
	  `totalFee` int(11) NOT NULL,
	  PRIMARY KEY (`number`)
	) $charset_collate;";
	
	dbDelta( $sql );
	
	// transactions
	$table_name = $tableprefix.'transactions';
	$charset_collate = $wpdb->get_charset_collate();
	
	$sql = "CREATE TABLE  IF NOT EXISTS $table_name (
	  `hash` varchar(128) NOT NULL,
	  `sender` varchar(64) NOT NULL,
	  `senderId` int(11) NOT NULL,
	  `recipient` varchar(64) NOT NULL,
	  `recipientId` int(11) NOT NULL,
	  `accountNonce` int(11) NOT NULL,
	  `price` int(11) NOT NULL,
	  `gasLimit` int(11) NOT NULL,
	  `amount` int(11) NOT NULL,
	  `block_id` int(11) NOT NULL,
	  `time` timestamp NOT NULL,
	  `newContract` tinyint(1) NOT NULL,
	  `isContractTx` varchar(32) DEFAULT NULL,
	  `blockHash` varchar(128) NOT NULL,
	  `parentHash` varchar(128) NOT NULL,
	  `txIndex` int(11) NOT NULL,
	  `gasUsed` int(11) NOT NULL,
	  `type` varchar(8) NOT NULL,
	  PRIMARY KEY (`hash`)
	) $charset_collate;";
	
	dbDelta( $sql );
	
	// accounts
	$table_name = $tableprefix.'accounts';
	$charset_collate = $wpdb->get_charset_collate();
	
	$sql = "CREATE TABLE  IF NOT EXISTS $table_name (
	  `id` int(11) NOT NULL AUTO_INCREMENT,
	  `LastModificationOn` timestamp NOT NULL,
	  `InsertionDate` timestamp NOT NULL,
	  `address` varchar(64) COLLATE utf8mb4_unicode_ci NOT NULL,
	  `balance` int(11) DEFAULT NULL,
	  `nonce` varchar(64) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
	  `code` text COLLATE utf8mb4_unicode_ci,
	  `name` varchar(32) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
	  `storage` varchar(128) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
	  `firstseen` timestamp NULL DEFAULT NULL,
	  `transactioncount` int(11) DEFAULT NULL,
	  `currentblocknumber` int(11) DEFAULT NULL,
	  `new_blocks_seen` int(11) DEFAULT NULL,
	  PRIMARY KEY (`id`),
	  UNIQUE KEY `address` (`address`)
	) $charset_collate;";
	
	dbDelta( $sql );
	
}

function ethereum_reader_plugin_deactivation() {
	ethereum_reader_Globals::logString("ethereum_reader_plugin_deactivation called");

}

function ethereum_reader_plugin_uninstall() {
	ethereum_reader_Globals::logString("ethereum_reader_plugin_uninstall called");

}



?>