var expiryInput = '<div class="form-item form-type-textfield form-item-commerce-payment-payment-details-expiry"><input placeholder="MM/YY" type="text" id="edit-commerce-payment-payment-details-credit-card-expiry" name="commerce_payment[payment_details][expiry]" value="" size="60" maxlength="128" class="form-text required" /></div>';
jQuery(".form-item-commerce-payment-payment-details-credit-card-code").before(expiryInput);

var numberInput = '<div class="form-item form-type-textfield form-item-commerce-payment-payment-details-card-number-new"><input placeholder="Card Number" type="text" id="edit-commerce-payment-payment-details-credit-card-number-new" name="commerce_payment[payment_details][cardnumber]" value="" size="120" maxlength="120" class="form-text required" /></div>';
jQuery(".form-item-commerce-payment-payment-details-expiry").before(numberInput);

// Is Card on file active? If so then hide/remove the Card.js because it's not playing nice with C.O.F.
var $cof = jQuery(".form-item-commerce-payment-payment-details-cardonfile input").length;

     
jQuery( document ).ajaxComplete(function( event,request, settings ) {
  // Is the credit card payment method selected?
  var $first_checked = jQuery("#edit-commerce-payment-payment-method input:radio:first").is(':checked');  
  
  if (!$first_checked){    
   jQuery(".jp-card-container").hide();   
   jQuery(".form-item-commerce-payment-payment-details-expiry").before(numberInput);
  }else{   
    var expiryInput = '<div class="form-item form-type-textfield form-item-commerce-payment-payment-details-expiry"><input placeholder="MM/YY" type="text" id="edit-commerce-payment-payment-details-credit-card-expiry" name="commerce_payment[payment_details][expiry]" value="" size="60" maxlength="128" class="form-text required" /></div>';  
    jQuery(".form-item-commerce-payment-payment-details-credit-card-code").before(expiryInput);
    
    var numberInput = '<div class="form-item form-type-textfield form-item-commerce-payment-payment-details-card-number-new"><input placeholder="Card Number" type="text" id="edit-commerce-payment-payment-details-credit-card-number-new" name="commerce_payment[payment_details][cardnumber]" value="" size="120" maxlength="120" class="form-text required" /></div>';
    jQuery(".form-item-commerce-payment-payment-details-expiry").before(numberInput);
    
    jQuery(".jp-card-container").show();    
    commerce_card_bindings();
    newCard();
  }
  if ($cof > 1){
   var expiryInput = '<div class="form-item form-type-textfield form-item-commerce-payment-payment-details-expiry"><input placeholder="MM/YY" type="text" id="edit-commerce-payment-payment-details-credit-card-expiry" name="commerce_payment[payment_details][expiry]" value="" size="60" maxlength="128" class="form-text required" /></div>';  
   jQuery(".form-item-commerce-payment-payment-details-credit-card-code").before(expiryInput);
   commerce_card_bindings();
   newCard(); 
   // remove card graphic
   jQuery(".jp-card-container").hide();   
   jQuery(".form-item-commerce-payment-payment-details-expiry").before(numberInput);      
  } 
});



if ($cof < 2){
  jQuery(document).ready(function(){      
      commerce_card_bindings();
      newCard();
  })  
}
 

// Display custom message if configured
if (Drupal.settings.commerce_card.custom_message){
  var message = Drupal.settings.commerce_card.custom_message;
  jQuery(".commerce_payment.form-wrapper legend").after( "<div class='card-message'>" + message + "</div>" );
}


if (Drupal.settings.commerce_card.same_page){     
  if (Drupal.settings.commerce_card.name_field){
    var name = jQuery('.form-item-customer-profile-billing-commerce-customer-address-und-0-name-line .name-block').val();      
  }else{    
    var first_name = jQuery('.form-item-customer-profile-billing-commerce-customer-address-und-0-first-name .first-name').val();    
    var last_name  = jQuery('.form-item-customer-profile-billing-commerce-customer-address-und-0-last-name .last-name').val();
      
    if (first_name != null && last_name != null){      
      var name = first_name + ' ' + last_name;    
    }    
  }  
  if (isEmpty(name)){
    name = Drupal.settings.commerce_card.name_placeholder;  
  }
  var form = Drupal.settings.commerce_card.form_name;    
 }else{   
  var position = Drupal.settings.commerce_card.position;
  var name = '';
  
  var name = jQuery('.field-name-commerce-customer-address:eq(' + position + ') .name-block .name-block').text();
  
  if (isEmpty(name)){
    name = Drupal.settings.commerce_card.name_placeholder;  
  }
  
  var form = Drupal.settings.commerce_card.form_name + ' .commerce_payment'; 
 }       
 
 if (Drupal.settings.commerce_card.owner){                            
   var owner_field = '*[id^="edit-commerce-payment-payment-details-credit-card-owner"]';       
 }else{
   if (Drupal.settings.commerce_card.name_field){
     var owner_field = 'input.name-block';
   }else{
     var owner_field = '.first-name, .last-name';
   }
}

function commerce_card_bindings(){
    var evt = document.createEvent('HTMLEvents');
    evt.initEvent('keyup', false, true);
    if (jQuery("#edit-commerce-payment-payment-details-credit-card-owner").length ) {
      document.getElementById('edit-commerce-payment-payment-details-credit-card-owner').dispatchEvent(evt);
    }
    
    if (jQuery("#edit-commerce-payment-payment-details-credit-card-number").length ) {
      document.getElementById('edit-commerce-payment-payment-details-credit-card-number').dispatchEvent(evt);
    }
    
    if (jQuery("#edit-commerce-payment-payment-details-credit-card-expiry").length ) {
      document.getElementById('edit-commerce-payment-payment-details-credit-card-expiry').dispatchEvent(evt);
    }
    
    if (jQuery("#edit-commerce-payment-payment-details-credit-card-code").length ) {
      document.getElementById('edit-commerce-payment-payment-details-credit-card-code').dispatchEvent(evt);
    }    
    jQuery("#edit-commerce-payment-payment-details-credit-card-expiry").bind("focusout", fillExpiry);
    jQuery("#edit-commerce-payment-payment-details-credit-card-number-new").bind("focusout", fillCardNumber);
    
    jQuery( ".checkout-continue" ).click(function() {
      fillExpiry();
      fillCardNumber();
    });    
}

function fillCardNumber(){
  var cardNumber = jQuery('#edit-commerce-payment-payment-details-credit-card-number-new').val();
  // Remove all white space from card number
  var cardNumber = cardNumber.replace(/\s+/g, '');  
  jQuery("#edit-commerce-payment-payment-details-credit-card-number").val(cardNumber);
}


function fillExpiry(){
  var exp = jQuery('#edit-commerce-payment-payment-details-credit-card-expiry').val();
  if (exp.length > 5){
    var dates = exp.split('/');
    var month = jQuery.trim(dates[0]);
    
    if (month.length < 2){
      month = '0' + month;
    }    
    jQuery("#edit-commerce-payment-payment-details-credit-card-exp-month").val(month);
    
    var year = jQuery.trim(dates[1]);
    if (year.length < 3){
      year = '20' + year;  
    }    
    jQuery("#edit-commerce-payment-payment-details-credit-card-exp-year").val(year); 
  }    
}
  
function isEmpty(str) {
   return (!str || 0 === str.length);
}
 
function newCard(){
  var card = new Card({   
      form: form,     
      container: '.commerce_payment', 
      formSelectors: {
          numberInput:  '*[id^="edit-commerce-payment-payment-details-credit-card-number-new"]',         
          expiryInput:  '*[id^="edit-commerce-payment-payment-details-credit-card-expiry"]',
          cvcInput:     '*[id^="edit-commerce-payment-payment-details-credit-card-code"]', 
          nameInput:    owner_field,
      },
      width: Drupal.settings.commerce_card.width,      
      formatting: true,    
      messages: {
          validDate: '', 
          monthYear: Drupal.settings.commerce_card.month_year, 
      },
          placeholders: {
          number: '•••• •••• •••• ••••',
          name: name,
          expiry: Drupal.settings.commerce_card.expiry,
          cvc: '•••'
      },    
      debug: Drupal.settings.commerce_card.debug,
  });
} 
 
