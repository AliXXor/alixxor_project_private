jQuery(window).bind('load', function() {
									 //alert("test");

	jQuery('.foreground').toggle('slow');

});

jQuery(function(){
				//alert("test2")
     
	jQuery('.view-portfolio .views-row .views-field-field-portfolio-image a').hover(function(){

		jQuery(this).find('img').stop().animate({opacity:'.4'})

	},



	function(){

		jQuery(this).find('img').stop().animate({opacity:'1'})
		 alert("test1");

	})
	
	//jQuery( ".field-name-field-phone-no-b" ).insertAfter( $( ".street-block" ) );

})

