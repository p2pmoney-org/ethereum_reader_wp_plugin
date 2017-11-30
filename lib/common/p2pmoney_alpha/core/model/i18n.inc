<?php
namespace p2pmoney\common\core\model;

class I18nServer {
	public static function getNumericFormatInfo($locale, &$decimalpoint, &$thousandssep) {
		if ($locale == null) {
			$locale_info = localeconv();

			$decimalpoint = $locale_info['decimal_point'];
			$thousandssep = $locale_info['thousands_sep'];
		}
		else {
			$oldlocale = setlocale(LC_NUMERIC, $locale);

			$locale_info = localeconv();

			$decimalpoint = $locale_info['decimal_point'];
			$thousandssep = $locale_info['thousands_sep'];

			// bug on Windows
			if (($locale == 'fr_FR') && ($decimalpoint == '.')) {
				$decimalpoint = ',';
				$thousandssep = '';
			}

			if ($oldlocale != false)
				setlocale(LC_NUMERIC, $oldlocale);
		}
	}

	// currency amounts
	public static function getFormattedDecimalAmount($amount, $decimals, $locale = null) {
		$amt = (float)$amount;
		
		// we round down to avoid number_format rounding
		$precision = pow(10, $decimals);
		$amt = floor($amt * $precision) / $precision;

		I18nServer::getNumericFormatInfo($locale, $decimalpoint, $thousandssep);

		return 	number_format($amt, $decimals, $decimalpoint, $thousandssep);
	}

	public static function parseAmountString($amountstring, $locale = null) {
		$floatstring = $amountstring;

		// floatval is not locale aware, we have to modify the string to en
		if ($locale != null) {

			I18nServer::getNumericFormatInfo($locale, $decimalpoint, $thousandssep);

			$floatstring = str_replace($thousandssep, "", $floatstring);
			$floatstring = str_replace($decimalpoint , ".", $floatstring);
		}

		return floatval($floatstring);
	}

	// numerics
	public static function getFormattedInteger($numeric, $locale = null) {
		$int = intval($numeric);

		I18nServer::getNumericFormatInfo($locale, $decimalpoint, $thousandssep);

		return number_format($int, 0, $decimalpoint, $thousandssep);
	}

	public static function parseIntegerString($numericstring, $locale = null) {
		//return (int)preg_replace("/[^\d-]+/","",$numericstring);
		return (int)preg_replace("/([^\d-]+|(?<=.)-)/","",$numericstring);
	}

	// percentages

	public static function getFormattedPercentage($percentagefloat, $decimals, $locale = null) {
		$pct = (float)$percentagefloat;

		I18nServer::getNumericFormatInfo($locale, $decimalpoint, $thousandssep);
		
		// we round down to avoid number_format rounding
		$precision = pow(10, $decimals);
		
		//$pct = floor($pct * 100 * $precision) / $precision;
		// problem for 0.0725 becoming 724
		
		// first get an int and add 0.01 to avoid the 7.24999999999 for 7.25
		$intvalue = floor($pct * 100 * $precision + 0.01);
		
		// then get float
		$pct = $intvalue / $precision;
		
		
		return 	number_format($pct, $decimals, $decimalpoint, $thousandssep);
	}

	public static function parsePercentageString($percentagestring, $locale = null) {
		$floatstring = $percentagestring;

		// floatval is not locale aware, we have to modify the string to en
		if ($locale != null) {

			I18nServer::getNumericFormatInfo($locale, $decimalpoint, $thousandssep);

			$floatstring = str_replace($thousandssep, "", $floatstring);
			$floatstring = str_replace($decimalpoint , ".", $floatstring);
		}

		return floatval($floatstring)/100;
	}
}
