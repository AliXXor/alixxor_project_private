/*jQuery(document).ready(function(){
								
								//alert ("test")
    
	jQuery("#block-menu-menu-left-menu h2.block-title").click(function(){
        jQuery(this).parents('#block-menu-menu-left-menu').find('.content ul.menu').slideToggle();
    });
	
	/**********************
    function windowSize() {
      var width = jQuery( window ).width();

      if( width <= 767 ){
		 jQuery('#block-menu-menu-left-menu h2.block-title').removeClass('mobile_click');
         jQuery('#block-menu-menu-left-menu .content ul.menu').hide();
      } else {
		 jQuery('#block-menu-menu-left-menu h2.block-title').addClass('mobile_click');
         jQuery('#block-menu-menu-left-menu .content ul.menu').show();
      }
    }
    windowSize();
    jQuery(window).resize(function(){
       windowSize();
    });
 
});*/
///////////////////////////

var updateQueryStringParam = function (key, value) {
    var baseUrl = [location.protocol, '//', location.host, location.pathname].join(''),
        urlQueryString = document.location.search,
        newParam = key + '=' + value,
        params = '?' + newParam;

    // If the "search" string exists, then build params from it
    if (urlQueryString) {
        keyRegex = new RegExp('([\?&])' + key + '[^&]*');

        // If param exists already, update it
        if (urlQueryString.match(keyRegex) !== null) {
            params = urlQueryString.replace(keyRegex, "$1" + newParam);
        } else { // Otherwise, add it to end of query string
            params = urlQueryString + '&' + newParam;
        }
    }
    window.history.replaceState({}, "", baseUrl + params);
};

//////////////////////
Drupal.behaviors.simple = {
  attach: function (context, settings) {
	 
	 
	jQuery('#block-block-8').insertBefore('.view-commerce-cart-form #edit-submit');
	jQuery('#block-block-9').insertAfter('body.node-type-product-display .commerce-product-field-commerce-price');
	//jQuery('#commerce-multicurrency-selector-form').insertBefore('.views-field-buyitnowbutton');
	
	
	//////////
	
     jQuery(".selected_currency").on('change', function() {
	    /*var baseurl = document.URL;
		var current_url = window.location.href;
		
		var new_current_url = 
        var currcode  = this.value;  
	    var new_url =  current_url+ '?currency=' + currcode;
		
		var pathArray = window.location.pathname.split( '/' );
		
		 // alert(baseurl);
          //alert(pathArray);		
        //window.location = new_url;*/
		 var currcode  = this.value; 
     	 //jQuery('[id*="commerce-multicurrency-selector-form"] select').val(currcode);
		 //jQuery("#commerce-multicurrency-selector-form select option[selected]").removeAttr("selected");
         //jQuery('[id*="commerce-multicurrency-selector-form"] select option[value=currcode]').attr('selected','selected');		 
		 updateQueryStringParam('currency', currcode);
		 window.location.reload();
		 //$("#commerce-multicurrency-selector-form").submit();
		 //jQuery('[id*="commerce-multicurrency-selector-form"] select option[value=currcode]').attr('selected','selected');	
		
     });
	
	//////////////
	jQuery('[id*="commerce-multicurrency-selector-form"] select').change(function() {
	//jQuery('#block-block-8 form select').change(function() {
        this.form.submit();
    });
	
	 
	jQuery("#block-menu-menu-left-menu h2.block-title").click(function(){
        jQuery(this).parents('#block-menu-menu-left-menu').find('.content ul.menu').slideToggle();
    });
	
	jQuery(".customer_profile_shipping .fieldset-legend").click(function(){
    jQuery(".customer_profile_shipping .panel-body").slideToggle();
	jQuery('.customer_profile_shipping .fieldset-legend').css( 'cursor', 'pointer' );
        });
	
	/**********************/
    function windowSize() {
      var width = jQuery( window ).width();

      if( width <= 767 ){
		 jQuery('#block-menu-menu-left-menu h2.block-title').removeClass('mobile_click');
         jQuery('#block-menu-menu-left-menu .content ul.menu').hide();
      } else {
		 jQuery('#block-menu-menu-left-menu h2.block-title').addClass('mobile_click');
         jQuery('#block-menu-menu-left-menu .content ul.menu').show();
      }
    }
    windowSize();
    jQuery(window).resize(function(){
       windowSize();
    });
  }
};