<?php

function commerce_card_admin_form() {  
  $form['text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text Options'),
  );
  $form['text']['commerce_card_name_placeholder_text'] = array(
    '#type'  => 'textfield',
    '#title' => t('Credit Card Name Placeholder Text'),
    '#default_value' => variable_get('commerce_card_name_placeholder_text', 'Your Name'),
    '#description' => 'Enter the placeholder text to be displayed on the credit card image.',
  );
  
  // Turn on payment message
  $form['text']['commerce_card_display_message_status'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Custom Message Display'),    
    '#default_value' => variable_get('commerce_card_display_message_status', 0),    
    '#description' => t('Enable this option to insert a custom message within the payment pane (using jQuery).'),    
  );
  
  $form['text']['commerce_card_display_message_text'] = array(
    '#type'  => 'textarea',
    '#title' => t('Custom Message Text'),    
    '#default_value' => variable_get('commerce_card_display_message_text', '<p> &#128274; This is a secure 2048-bit SSL Encrypted Payment. You\'re safe.</p>'),    
    '#states' => array(
    'invisible' => array(
        ':input[name="commerce_card_display_message_status"]' => array('checked' => FALSE),
      ),
    ),    
    '#description' => t('Enter the custom text to be displayed in the payment pane (may contain HTML).'),    
  );
  
  $form['options'] = array(
     '#type' => 'fieldset',
     '#title' => t('Configuration Options'),
  );
  
  $form['options']['commerce_card_width'] = array(
     '#type'  => 'textfield',
     '#title' => t('Image Width'),    
     '#default_value' => variable_get('commerce_card_width', 350),    
     '#description' => t('Change this value to the width (in pixels) for the credit card image.'),    
  );
  
  $form['options']['commerce_card_year_type'] = array(
     '#type'  => 'select',
     '#title' => t('Year Type'),    
     '#options' => array(0 => 'Two Digits (XX)', 1 => 'Four Digits (YYYY)'),
     '#default_value' => variable_get('commerce_card_year_type', 2),    
     '#description' => t('Should the expiration date be entered as a two digit or four digit number?'),    
  );
  
  $form['options']['commerce_card_debug'] = array(
     '#type'  => 'checkbox',
     '#title' => t('Debug Mode.'),    
     '#default_value' => variable_get('commerce_card_debug', 0),    
     '#description' => t('If enabled, will log troubleshooting messages to the browser console.'),    
  );
  
  $options = array(0 => 'First Position', 1 => 'Second Position');
  $form['options']['commerce_card_position'] = array(
    '#type' => 'select',
    '#title' => t('Billing Summary Position'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_card_position', 0),    
    '#description' => t('Select the position on the screen of the billing summary output. <u>This is only applicable if the billing information pane and payment pane are not on the same screen.</u>'),  
  );
  
  return system_settings_form($form);
}


function commerce_card_admin_form_validate($form, $form_state){
  $width = $form_state['values']['commerce_card_width'];
  if (!is_numeric($width)) {
      form_set_error('commerce_card_width', t('You must enter an numerical value for the credit card image width.'));
  }
  elseif ($width <= 0) {
      form_set_error('commerce_card_width', t('The width must be a positive number.'));
  }
}