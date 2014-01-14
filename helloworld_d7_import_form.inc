<?php

namespace CVWO\HelloWorld {

	require_once drupal_get_path("module", MODULE_NAME)."/helloworld_d7_api.inc";

	/**
	 * Displays the form for adding/viewing/editing details of the user record
	 */
	function helloworld_import_form($form, &$form_state, $page_mode = 'add', $helloworld_id = null) {
		$form = array();

		if ($helloworld_id != null && $page_mode != 'add') {
			// we store the helloworld_id here for future reference (when updating/deleting later)
			$form['helloworld_id'] = array(
				'#type' => 'hidden',
				'#value' => $helloworld_id,
			);
			if (!load_user($form_state, $helloworld_id)) {
				drupal_set_message('Failed to load user details');
				drupal_goto(MODULE_NAME);
			}
		}
		
		if (isset($form_state['values'])) {
			$values = $form_state['values'];
		} else {
			$values = array(
				'information' => '',
				'comment' => '',
			);
		}
		
		if ($page_mode == 'view') {
			$disabled = TRUE;
		} else {
			$disabled = FALSE;
		}
		
		$form['information'] = array(
			'#type' => 'textarea',
			'#title' => t('User Records'),
			'#default_value' => $values['information'],
			'#disabled' => $disabled,
			'#rows' => 5,
			'#required' => TRUE,
		);
		
		$form['comment'] = array(
			'#type' => 'textarea',
			'#title' => t('Comments'),
			'#disabled' => $disabled,
			'#default_value' => $values['comment'],
			'#rows' => 3,
			'#required' => FALSE,
		);
		
		$form['import'] = array(
			'#type' => 'submit',
		    '#value' => t('Import'),
		);
				  
		return $form;
	}

	/**
	 * Implements form_validate
	 */
	function helloworld_import_form_validate($form, &$form_state){
		// valid_email_address is from drupal api
		// validation function checks whether the email is valid before inserting/updating/deleting
		//if (!valid_email_address($form_state['values']['email'])){
		//	form_set_error('email', t('Please enter a valid email address.'));
		//}
		$information = $form_state['values']['information'];
		

		while (strlen($information) > 0) {
            $name=substr( $information, 0, strpos($information,','));
            $information= substr($information , strpos($information, ',') + 1);
            if (strpos($information, "\n")){
            	$email=substr( $information , 0, strpos($information,"\n")-1);
                $information= substr($information , strpos($information, "\n") + 1);
            } else {
            	$email = $information;
            	$information = null;
            }

            if (!valid_email_address($email)){
		       form_set_error('email', t('Please enter a valid email address.'));}

		}

	}

	/**
	 * Implements form_submit
	 */
	function helloworld_import_form_submit($form, &$form_state){
		$error = FALSE;
		$button_name = $form_state['values']['op'];
 
		if ($button_name = 'import'){
		if (import_user($form_state['values'])) {
			drupal_set_message(t('Users has been successfully added'));
		} else {
			drupal_set_message(t('An error was encountered while adding users'));
			$error = TRUE;
		}}

		
		
		// if we encountered an error, we keep the form as it is
		// else we redirect back to home page
		if ($error) {
			$form_state['rebuild'] = TRUE;
		} else {
			$form_state['redirect'] = MODULE_NAME;
		}
	}

	/**
	 * Function implements the Helloworld admin settings form
	 */ 
	 function helloworld_admin() {
		$form[PAGELIMIT] = array(
			'#type' => 'textfield',
			'#title' => t('Maximum number of records in a page'),
			'#default_value' => variable_get(PAGELIMIT),
			'#size' => 2,
			'#maxlength' => 2,
			'#description' => t("The maximum number of records to display in the user records listing."),
			'#required' => TRUE,
		);
		
		$form[AUTOCOMPLETELIMIT] = array(
			'#type' => 'textfield',
			'#title' => t('Maximum number of records in autocomplete'),
			'#default_value' => variable_get(AUTOCOMPLETELIMIT),
			'#size' => 2,
			'#maxlength' => 2,
			'#description' => t("The maximum number of records to display under autocomplete function."),
			'#required' => TRUE,
		);
		
		return system_settings_form($form);
	 }
	 
	 /**
	 * Form validation handler for helloworld_admin().
	 *
	 * @see helloworld_admin()
	 */
	function helloworld_admin_validate($form, &$form_state) {
	  $helloworld_pagelimit = $form_state['values'][PAGELIMIT];
	  $helloworld_autocompletelimit = $form_state['values'][AUTOCOMPLETELIMIT];
	  
	  if (!is_numeric($helloworld_pagelimit)) {
	    form_set_error('helloworld_pagelimit', t('You must enter a number for the maximum number of records in a page.'));
	  } else if ($helloworld_pagelimit <= 0) {
	    form_set_error('helloworld_pagelimit', t('Maximum number of records in a page must be positive.'));
	  }
	 
	  if (!is_numeric($helloworld_autocompletelimit)) {
	    form_set_error('helloworld_autocompletelimit', t('You must enter a number for the maximum number of records displayed in autocomplete function.'));
	  } else if ($helloworld_autocompletelimit <= 0) {
	    form_set_error('helloworld_autocompletelimit', t('Maximum number of records in autocomplete function must be positive.'));
	  } 
	 
	}
}
