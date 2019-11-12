<?php

/**
 * @file
 * Default implementation of the commerce order email template.
 *
 * Available variables:
 * - $site: site_name & site_url
 * - $order: order object.
 * - $order->order_items: array of order items
 * - $customer_profile: customer order name and address.
 * - $language: language code of the mail.
 */
?>

 <?php
 global $base_url;
 //watchdog('order', '<pre>'. print_r($order,true).'</pre>');
 
 
 /*$customer_profile = commerce_customer_profile_load($order->data['profiles']['customer_profile_shipping']);*/
 $profile_id =  $order->commerce_customer_billing['und'][0]['profile_id'];
 $customer_profile = commerce_customer_profile_load($profile_id); 

  if(!empty($customer_profile->commerce_customer_address['und'][0]['premise'])){
   $address2 = ' '.$customer_profile->commerce_customer_address['und'][0]['premise']; 
 }
  if(!empty($customer_profile->commerce_customer_address['und'][0]['dependent_locality'])){
   $subcity = ' '.$customer_profile->commerce_customer_address['und'][0]['dependent_locality']; 
 }
 
 //watchdog('order', '<pre>'. print_r($order,true).'</pre>');
 //watchdog('customer_profile', '<pre>'. print_r($customer_profile,true).'</pre>');
 //watchdog('commerce_line_items', '<pre>'. print_r($commerce_line_items,true).'</pre>');
 ?>
 <strong>Thank you for your order!</strong>
 <p></p>
 <p></p>

 <table width="800px" border="0" cellspacing="0" cellpadding="1" align="center" style="border:1px solid #ccc;">
    <tr>
    
    <td>
    <table width="800px" border="0" cellspacing="0" cellpadding="5" align="center" bgcolor="#FFF" style="font-family: verdana, arial, helvetica; font-size: 12px;">       
<tr>
<td>
<table width="800px" border="0" cellspacing="0" cellpadding="0" style="font-family: verdana, arial, helvetica; font-size: 12px;">
<tr>

<td style="line-height: 1.6em;" valign=" middle"><!-- Invoice Header -->
<p></p>
<p></p>
          <img src="<?php print $base_url; ?>/sites/default/files/logo_0_0.png" /> <br/>
</td>
</br>

</tr>
</table>
</td>
</tr>

        <tr>
        <td>
        <table width="800px" border="0" cellspacing="0" cellpadding="0" style="font-family: verdana, arial, helvetica; font-size: 11px;">
            <tr>
                <th style="background: gray; color: #FFF; text-align: left; padding: 3px 5px; text-transform: uppercase; font-weight: bold; border: 0; vertical-align: middle;"><?php print t('Order Summary'); ?></th>
            </tr>
            <tr>
            
            <td style="font-family: verdana, arial, helvetica; font-size:12px; padding-left:5px;">
                <br/>
				<b><?php print t('Order Date:'); ?></b> <?php print date('F j, Y', $order->created); ?><br/>
                <b><?php print t('Order No:'); ?></b> <?php print $order->order_number; ?><br/>
                <br/>
				<b><?php print t('Email Address:'); ?></b> <?php print $order->mail; ?><br/>
                <br/>
                <b><?php print t('Delivery Address:'); ?></b><br />
				
                <?php print $customer_profile->commerce_customer_address['und'][0]['name_line']; ?><br />
                <?php print $customer_profile->commerce_customer_address['und'][0]['thoroughfare'].$address2.$subcity; ?><br />
                <?php print $customer_profile->commerce_customer_address['und'][0]['locality'] . ',' . $customer_profile->commerce_customer_address['und'][0]['administrative_area'] . ' ' . $customer_profile->commerce_customer_address['und'][0]['postal_code']; ?><br/>
                <?php print ('Country:'); ?> <?php  print $customer_profile->commerce_customer_address['und'][0]['country'];?><br/>
                <?php print ('Phone Number:');?><br /> <?php print $customer_profile->field_phone['und'][0]['value']; ?><br />
				<p></p>
				<p></p>
             
                <tr>
            <td><table class="products" width="800px" border="0" cellspacing="0" cellpadding="0" align="center" style="font-family: verdana, arial, helvetica; font-size: 12px;">
                    <thead>
                        <tr>
                            <th class="views-field views-field-quantity" style="background: gray; color: #FFF; text-align: left; padding: 0px 5px 0px 10px; text-transform: uppercase; font-weight: bold; border: 0; vertical-align:middle; width:200px;">QTY</th>
							
                            <th class="views-field views-field-field-product-image" style="background: gray; color: #FFF; text-align: left; padding: 0px 5px; text-transform: uppercase; font-weight: bold; border: 0; vertical-align: middle; width: 400px;">PRODUCT</th>
							
                            <!--<th class="views-field views-field-line-item-title" style="background: gray; color: #FFF; text-align: left; padding: 0px 5px; text-transform: uppercase; font-weight: bold; border: 0; vertical-align: middle;">&nbsp;</th>-->
                         
                            <th class="views-field views-field-commerce-total views-align-right" style="background: gray; color: #FFF; text-align: right; padding: 0px 5px; text-transform: uppercase; font-weight: bold; border: 0; vertical-align: middle; padding-right: 12px;">Total</th>
                        </tr>
                    </thead>
					<p></p>
                    <tbody>
                        <?php 
			    foreach($order->commerce_line_items['und'] as $key => $value){
			      $line_item = commerce_line_item_load($value['line_item_id']);

				  if ($line_item->type == 'product') {
				    $product_data = commerce_product_load($line_item->commerce_product['und'][0]['product_id']);
			  ?>
                        <tr>
                            <td class="views-field views-field-quantity" style="width:200px; text-align:left; padding: 0px 5px 0px 10px; border-bottom:1px solid #46BF56;">
							<?php 

				              $qty = $line_item->quantity;
			                   print $qty;
			                ?>
                           </td>
                       <!--<td class="views-field views-field-field-product-image" style="width: 60px;  padding:3px 5px 1px; border-bottom:1px solid #46BF56;">
					   <?php
			       // $image_url = @$product_data->field_images['und'][0]['uri'];
				   /*if(!empty($image_url)){
					  $image_settings = array(
					  'style_name' => 'thumbnail',
					  'path' => $product_data->field_images['und'][0]['uri'],
					  'attributes' => array('class' => 'image'),
					  'getsize' => TRUE,
					  );
					  $image_path = theme('image_style', $image_settings);
					  $full_path = file_create_url($product_data->field_images['und'][0]['uri']);
					  print l($image_path, $full_path, array('html' => TRUE));
				   }else{
					  print '<img src="'.$base_url.'/sites/default/files/small-default.png"/>';
				   }*/ 
				           
                  ?>
                            </td>-->
                            <td class="views-field views-field-line-item-title" style="border-bottom:1px solid #46BF56;width: 400px;"><?php 
			
			    print $product_data->title;

			 ?>
                            </td>
           
                            <td class="views-field views-field-commerce-total views-align-right" style="text-align:right; padding-right: 18px; border-bottom:1px solid #46BF56;"><?php print commerce_currency_format($line_item->commerce_total['und'][0]['amount'],'USD'); ?> </td>
                        </tr>
                        <?php 
			}
			   }
             ?>
                    </tbody>
                </table>
                <table class="commerce-price-formatted-components" style="margin-left:auto; font-size:12px; text-align:right;">
                  
                    <tr class="component-type-base-price">
                        <td class="component-title" style="padding-right:20px;"><b>Subtotal</b></td>
                        <td class="component-total" style="text-align:right; padding-right:10px;"><?php print commerce_currency_format($order->commerce_order_total['und'][0]['data']['components'][0]['price']['amount'],'USD'); ?></td>
                    </tr>
                 
                    <?php
	foreach($order->commerce_line_items['und'] as $key => $value){
			      $line_item = commerce_line_item_load($value['line_item_id']);
				  if ($line_item->type == 'shipping') {
					
				   $delivery_label = 'Delivery Fee: ';
          print '<tr class="component-type-delivery">' .
                 '<td class="component-title" style="padding-right:20px;"><b>' . $delivery_label . '</b></td>' .
                 '<td class="component-total" style="text-align:right; padding-right:10px;">' . commerce_currency_format($line_item->commerce_total['und']['0']['amount'], 'USD') . '</td></tr>';
				  }
		}
	?>
                    <?php
    print  '<tr  class="component-type-commerce-price-formatted-amount">'.
                '<td class="component-title" style="padding-right:20px;"><b>Order Total</b> </td>'.
                '<td class="component-total" style="text-align:right; padding-right:10px;">'. commerce_currency_format($order->commerce_order_total['und'][0]['amount'],'USD') . '</td></tr>';		   
	?>
                </table></td>
        </tr>
    </table>
    </td>
    
    </tr>
    
</table>