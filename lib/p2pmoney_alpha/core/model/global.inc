<?php
namespace p2pmoney\common\core\model;

require_once('i18n.inc');
require_once('p2pmoneydeployment.inc');

use Exception;


class P2PMoneyAlphaGlobals {
	
	const UUID_LENGTH = 16;
	const UUID_EXTRA_LENGTH = 32;
	
	const CORE_UUID_LENDTH = 16;
	
	static public function generateUUID($prefix = 'W', $maxlength=0 ) {
		global $increment;
	
		if (!isset($increment))
			$increment = 0;
	
		// Perfect for: UNIQUE ID GENERATION
		// Create a UUID made of: PREFIX:RANDOM CHAR:INCREMENT:TIMESTAMP:UUID(PART - LENGTH - or FULL)
		$my_random_id = $prefix;
		$my_random_id .= chr ( rand ( 65, 90 ) );
		$my_random_id .= strval($increment++);
		$my_random_id .= time ();
	
		$randomprefix = chr ( rand ( 65, 90 ) );
		$my_uniqid = uniqid ( $randomprefix );
	
		if($maxlength > 0) {
			$idlength = strlen($my_random_id);
	
			if ($idlength > $maxlength ) {
				$my_random_id = substr($my_random_id, 0, $maxlength);
			}
			else {
				$sparelength = $maxlength - $idlength;
	
				if ($sparelength > 0) {
					$uidlength = strlen($my_uniqid);
					$my_random_id .= substr($my_uniqid, $uidlength - $sparelength, $uidlength);
				}
			}
		}
		else {
			// no limit set
			$my_random_id .= $my_uniqid;
		}
	
		$my_random_id = strtolower($my_random_id);
	
		return $my_random_id;
	}

	
	static public function getCountryIsoCodeFromCountryCode($countrycode) {

		if (!isset($countrycode))
			return 'FRA';

		switch($countrycode) {
			case 32:
				return 'BEL';
					
			case 33:
				return 'FRA';
					
			default:
				return 'FRA';
					
		}
	}

	const ISO3_CURRENCY_CODE_EURO = 'EUR';
	const ISO3_CURRENCY_CODE_USD = 'USD';

	static public function getCurrencyFromCountryCode($countrycode) {

		if (!isset($countrycode))
			return 'EUR';

		switch($countrycode) {
			case 1:
				return 'USD';
					
			case 32:
				return 'EUR';
					
			case 33:
				return 'EUR';
					
			case 44:
				return 'GBP';
					
			default:
				return 'EUR';
					
		}
	}


	static public function getCountryNameFromCountryCode($countrycode, $locale) {

		if (!isset($countrycode))
			return 'EUROPE';

		switch($countrycode) {
			case 1:
				return 'USA';
					
			case 32:
				return 'BELGIQUE';
					
			case 33:
				return 'FRANCE';
					
			case 44:
				return 'UNITED KINGDOM';
					
			default:
				return 'EUROPE';
					
		}
	}

	//
	// dates
	//
	public static function getFormattedDate($date, $locale = null) {
		return date('Y-m-d', $date);
	}

	public static function getFormattedDateTime($date, $locale = null) {
		return date('Y-m-d H:i:s', $date);
	}

	public static function parseFormattedDateString($datestring, $locale = null) {
		return strtotime($datestring);
	}

	public static function parseFormattedDateTimeString($datestring, $locale = null) {
		return strtotime($datestring);
	}

	//
	// amounts
	//
	const AMOUNT_PRECISION = 1000; // we work in 1/10 of cents
	const CURRENCY_PRECISION = 100; // currencies (e.g. euro and dollars) work in cents

	static public function getPrecisionAmount($amount) {
		// intval is rounding down
		return intval($amount*P2PMoneyAlphaGlobals::AMOUNT_PRECISION);
	}

	static public function getAmountFloat($precisionamount) {
		$amountfloat = (float)$precisionamount;

		$amountfloat = $amountfloat/P2PMoneyAlphaGlobals::AMOUNT_PRECISION;

		return $amountfloat;
	}

	static public function roundPrecisionAmountToCurrencyPrecision($precisionamount) {
		$floatamount = P2PMoneyAlphaGlobals::getAmountFloat($precisionamount);
		$floatamount = floor($floatamount * P2PMoneyAlphaGlobals::CURRENCY_PRECISION) / P2PMoneyAlphaGlobals::CURRENCY_PRECISION;

		return P2PMoneyAlphaGlobals::getPrecisionAmount($floatamount);
	}

	public static function getFormattedCurrencyAmountString($amount, $currency, $decimals = 2, $locale = null) {
		return strval(I18nServer::getFormattedDecimalAmount($amount, $decimals, $locale))." ".$currency;
	}

	static public function roundAmountFloat($amount) {
		return round($amount);
	}

	static public function floorAmountFloat($amount) {
		return floor($amount);
	}

	static public function ceilAmountFloat($amount) {
		return ceil($amount);
	}

	//
	// percentages
	//
	const PERCENTAGE_PRECISION = 10000; // we work in 10,000 e.g. 9.01% is 901

	const EXTRA_PERCENTAGE_PRECISION = 1000000; // we work in 1,000,000 e.g. 9.0111% is 90111


	static public function getAmountShare($amount, $precisionpercentage, $precision = -1) {
		if ($precision == -1)
			return ($amount*$precisionpercentage) / P2PMoneyAlphaGlobals::PERCENTAGE_PRECISION;
		else
			return ($amount*$precisionpercentage) / $precision;
	}

	static public function getPercentageFloat($precisionpercentage) {
		$pctfloat = (float)$precisionpercentage;

		$pctfloat = $pctfloat/P2PMoneyAlphaGlobals::PERCENTAGE_PRECISION;

		return $pctfloat;
	}

	static public function getPrecisionPercentage($percentagefloat) {
		$precisionpercentage = intval($percentagefloat*P2PMoneyAlphaGlobals::PERCENTAGE_PRECISION);
		// intval is rounding down

		return $precisionpercentage;
	}

	static public function getExtraPercentageFloat($precisionpercentage) {
		$pctfloat = (float)$precisionpercentage;

		$pctfloat = $pctfloat/P2PMoneyAlphaGlobals::EXTRA_PERCENTAGE_PRECISION;

		return $pctfloat;
	}

	static public function getExtraPrecisionPercentage($percentagefloat) {
		$precisionpercentage = intval($percentagefloat*P2PMoneyAlphaGlobals::EXTRA_PERCENTAGE_PRECISION);
		// intval is rounding down

		return $precisionpercentage;
	}

	public static function getFormattedPercentageString($percentagefloat, $decimals, $locale = null) {
		return strval(I18nServer::getFormattedPercentage($percentagefloat, $decimals, $locale))."%";
	}


	//
	// control
	//
	static public function checkLooksLikeBotEmail($email, $blacklist) {

		// check 1: containing a +
		if (strpos($email, '+',  0) !== false) {
				
			// check 1.1: black list
			if (isset($blacklist)) {
				foreach($blacklist as $bad) {
					if (strpos($email, $bad,0) !== false)
						return true;
				}
			}
				
				
			//  structure is "un-hunman"
				
			// check 1.2: several numbers
			if (preg_match('/[0-9]+/', $email)) {
					
				$parts = explode('@', $email);
					
				$username = $parts[0];
					
				//$segments = preg_split('#(?<=\d)(?=[a-z])#i', $username);
				$segments = preg_split('~\D~', $username);
				$segments = array_filter($segments);
					
				//echo ' print is '.print_r($segments, true);
					
				if (count($segments) > 1) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
				
			// check 1.3: capital letters after the +
			$segments = explode('+', $username);
			$segments = array_filter($segments);
				
			if (isset($segments[1]) && (preg_match('/[A-Z]/', $segments[1]))){
				true;
			}
			else {
				return false;
			}

		}

		// check 2: containing too many full points
		$count =  mb_substr_count($email, '.');

		if ($count > 5)
			return true; // strange to have 5 . in a human email
			
		return false;
	}
	
	
	static private $error_log_file;
	
	static public function setErrorLogFile($path) {
		if (file_exists($path))
		P2PMoneyAlphaGlobals::$error_log_file = $path;
	}
	
	static private $bCanLog;
	
	static public function logString($string) {
		$bLogString = false;
		
		if ((isset(P2PMoneyAlphaGlobals::$bCanLog)) && (P2PMoneyAlphaGlobals::$bCanLog == false)) {
			return;
		}
		
		// define log path
		if (isset(P2PMoneyAlphaGlobals::$error_log_file)) {
			$file_log = P2PMoneyAlphaGlobals::$error_log_file;
		}
		else {
			$file_log = P2PMoneyAlphaDeployment::getModuleDataDir('p2pmoney_alpha').'/files/logs/p2pmoney_alpha_error.log';
		}
		
		if ((isset(P2PMoneyAlphaGlobals::$bCanLog))) {
			$bLogString = true;
		}
		else {
			// check now if we can actually log
			if (file_exists($file_log)) {
				$bLogString = true;
				P2PMoneyAlphaGlobals::$bCanLog = true;
			}
			else {
				$bLogString = false;
				P2PMoneyAlphaGlobals::$bCanLog = false;
			}
		}
			
		
		if ($bLogString) {
			$logstring = date("c", time()).": ".$string.PHP_EOL;
				
			error_log($logstring, 3, $file_log);
		}
	}
	
	// security
	static public function isEmailValid($email) {
		if (validateEmail($email) != 0)
			return false;
		else
			return true;
	}
	
	static public function isWellFormedUUID($uuid, $maxlength) {
		if (!isset($uuid))
			return false;
	
		if (strlen($uuid) > $maxlength)
			return false;
	
		if ( preg_match('/\s/',$uuid) )
			return false;
	
		return true;
	}
	
	
}

class SecurityException extends Exception {

}

class EncryptionUtils {
	private static $algo = '$2a';
	private static $cost = '$10';

	static public function getHashedPhrase($phrase, $salt, $pepper) {
		$algo = self::$algo;
		$cost = self::$cost;

		$password = hash_hmac('sha256', $phrase, $pepper, false);
		$hashedphrase = crypt($password, $algo . $cost . '$' . $salt);

		return $hashedphrase;
	}
}

