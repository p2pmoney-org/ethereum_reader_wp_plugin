<?php 


function ethereum_reader_load_widgets() {
	register_widget( 'ethereum_reader_account_widget' );
}

class ethereum_reader_account_widget extends WP_Widget {

	function __construct() {
		parent::__construct(

				// Base ID of your widget
				'ethereum_reader_account_widget',

				// Widget name will appear in UI
				__('Ethereum Account Widget', 'ethereum_reader_widget_domain'),

				// Widget description
				array( 'description' => __( 'Widget to have a live view on a ethereum account', 'ethereum_reader_widget_domain' ), )
				);
	}

	// Creating widget front-end
	
	public function getAccountInfoContent($address) {
		ethereum_reader_Globals::logString('widget - ethereum_reader_account_widget::getAccountInfoContent called');
		
		// get info for this account

		// get ethereum reader server
		try {
			$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection
		
		}
		catch(Exception $e) {
			$content = '<div>Exception has occured: '.$e->getMessage().'</div>';
		
			return $content;
		}
		
		
		$account = $ethereumserver->getAccount($address);
		
    	$balance = (isset($account) ? $account->getBalance()  : null);
    	
		$content = __( 'Balance:', 'ethereum_reader_widget_domain' ).'<br>'.$balance;
		
		return $content;
	}

	public function widget( $args, $instance ) {
		$title = apply_filters( 'widget_title', $instance['title'] );
		$address = apply_filters( 'widget_title', $instance['address'] );
		
		// before and after widget arguments are defined by themes
		echo $args['before_widget'];
		if ( ! empty( $title ) )
			echo $args['before_title'] . $title . $args['after_title'];

			$content = $this->getAccountInfoContent($address);
			
			// This is where you run the code and display the output
			echo $content;
			echo $args['after_widget'];
	}
	 
	// Widget Backend
	public function form( $instance ) {
		if ( isset( $instance[ 'title' ] ) ) {
			$title = $instance[ 'title' ];
		}
		else {
			$title = __( 'New title', 'ethereum_reader_widget_domain' );
		}
		
		if ( isset( $instance[ 'address' ] ) ) {
			$address = $instance[ 'address' ];
		}
		else {
			$address = __( 'account address', 'ethereum_reader_widget_domain' );
		}
		
		
		
		// Widget admin form
		
		?>
		<p>
		<label for="<?php echo $this->get_field_id( 'title' ); ?>"><?php _e( 'Title:' ); ?></label> 
		<input class="widefat" id="<?php echo $this->get_field_id( 'title' ); ?>" name="<?php echo $this->get_field_name( 'title' ); ?>" type="text" value="<?php echo esc_attr( $title ); ?>" />
		</p>
		<?php 

		?>
		<p>
		<label for="<?php echo $this->get_field_id( 'address' ); ?>"><?php _e( 'Address:' ); ?></label> 
		<input class="widefat" id="<?php echo $this->get_field_id( 'address' ); ?>" name="<?php echo $this->get_field_name( 'address' ); ?>" type="text" value="<?php echo esc_attr( $address ); ?>" />
		</p>
		<?php 

	}
     
	// Updating widget replacing old instances with new
	public function update( $new_instance, $old_instance ) {
		$instance = array();
		$instance['title'] = ( ! empty( $new_instance['title'] ) ) ? strip_tags( $new_instance['title'] ) : '';
		$instance['address'] = ( ! empty( $new_instance['address'] ) ) ? strip_tags( $new_instance['address'] ) : '';
		return $instance;
	}
}

?>