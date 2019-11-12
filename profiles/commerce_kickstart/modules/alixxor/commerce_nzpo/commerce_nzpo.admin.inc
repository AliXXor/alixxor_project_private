<?php

/**
 * @file
 * Handles rates page for Commerce NZ PO module.
 */
/*
 * Page with editable shipping rates
 */
function commerce_nzpo_rates_standart_page() {
    return commerce_nzpo_rates_page(FALSE);
}

/*
 * Page with editable shipping rates
 */

function commerce_nzpo_rates_priority_page() {
    return commerce_nzpo_rates_page(TRUE);
}

/*
 * Page with editable shipping rates
 */

function commerce_nzpo_rates_page($priority = FALSE) {
    $output = '<br/>';
    $output .= drupal_render(drupal_get_form('commerce_nzpo_rates_add_form', $priority));
    $output .= drupal_render(drupal_get_form('commerce_nzpo_rates_form', $priority));

    return $output;
}

/*
 * Form dor adding shipping rates for Product
 */

function commerce_nzpo_rates_add_form($form, &$form_state, $priority = FALSE) {
    $priority = $priority ? 1 : 0;

    $pids = db_select('commerce_nzpo_shipping_rates', 'sr')
            ->fields('sr', array('product_id'))
            ->condition('priority', $priority)
            ->execute()
            ->fetchCol();

    $available_products_result = db_select('commerce_product', 'p')
            ->fields('p')
            ->condition('type', 'product');

    if (count($pids)) {
        $available_products_result->condition('p.product_id', $pids, 'NOT IN');
    }

    $available_products_result = $available_products_result->execute();

    $available_products = array();
    foreach ($available_products_result as $available_product) {
        $available_products[$available_product->product_id] = $available_product->title;
    }

    if (!count($available_products)) {
        return array();
    };

    $form['add_rate'] = array(
        '#type' => 'fieldset',
        '#title' => t('Add rate'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['priority'] = array(
        '#type' => 'value',
        '#value' => $priority ? 1 : 0,
    );
    $form['add_rate']['product_id'] = array(
        '#type' => 'select',
        '#title' => t('Product'),
        '#options' => $available_products,
    );
    $form['add_rate']['shipping_NZ'] = array(
        '#type' => 'textfield',
        '#title' => t('Shipping to New Zealand'),
        '#size' => 4,
        '#default_value' => '',
    );
    $form['add_rate']['shipping_AUS'] = array(
        '#type' => 'textfield',
        '#title' => t('Shipping to Australia'),
        '#size' => 4,
        '#default_value' => '',
    );
    $form['add_rate']['shipping_EA_NA_UC_EUR_SP'] = array(
        '#type' => 'textfield',
        '#title' => t('Shipping to Asia, Europe, USA, Canada'),
        '#size' => 4,
        '#default_value' => '',
    );
    $form['add_rate']['shipping_RW'] = array(
        '#type' => 'textfield',
        '#title' => t('Shipping to Rest of World'),
        '#size' => 4,
        '#default_value' => '',
    );
    $form['add_rate']['shipping_addon_pct'] = array(
        '#type' => 'textfield',
        '#title' => t('Percentage for addon product'),
        '#size' => 4,
        '#default_value' => '',
    );

    $form['add_rate']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save new rate'),
    );

    return $form;
}

/**
 * Validate function for 'commerce_nzpo_rates_add_form' form
 * UNUSED
 */
//function commerce_nzpo_rates_add_form_validate($form, &$form_state) {
//    
//}

/**
 * Submit function for 'commerce_nzpo_rates_add_form' form
 */
function commerce_nzpo_rates_add_form_submit($form, &$form_state) {
    $new_rate = array();
    $post_fields = array(
        'product_id',
        'shipping_NZ',
        'shipping_AUS',
        'shipping_EA_NA_UC_EUR_SP',
        'shipping_RW',
        'shipping_addon_pct',
        'priority'
    );
    
    foreach ($post_fields as $post_field) {
        $new_rate[$post_field] = isset($form_state['values'][$post_field]) ? $form_state['values'][$post_field] : 0;
    }

    commerce_nzpo_rates_save($new_rate);

    // Redirect based on the button clicked.
    $redirect = 'admin/commerce/config/shipping/nzpo_rates';
    if ($new_rate['priority']) {
        $redirect .= '/priority';
    }

    $form_state['redirect'] = $redirect;
}

/*
 * Form dor editing shipping rates for each Product
 */

function commerce_nzpo_rates_form($form, &$form_state, $priority = FALSE) {
    $form['rates'] = array(
        '#prefix' => '<div id="shipping_rates">',
        '#suffix' => '</div>',
        '#tree' => TRUE,
        '#theme' => 'table',
        '#header' => array(
            t('Product'),
            array('data' => t('To New Zealand'), 'style' => 'text-align:center; width: 100px;'),
            array('data' => t('To Australia'), 'style' => 'text-align:center; width: 100px;'),
            array('data' => t('To Asia, Europe, USA, Canada'), 'style' => 'text-align:center; width: 100px;'),
            array('data' => t('To Rest of World'), 'style' => 'text-align:center; width: 100px;'),
            array('data' => t('Percentage for addon product'), 'style' => 'text-align:center; width: 100px;'),
            t(''),
        ),
        '#rows' => array(),
    );

    $priority = $priority ? 1 : 0;

    $query = db_select('commerce_nzpo_shipping_rates', 'sr');
    $query->innerJoin('commerce_product', 'p', 'sr.product_id = p.product_id');
    $rates = $query->fields('sr')
            ->fields('p', array('title'))
            ->condition('priority', $priority)
            ->execute();

    $show_submit = false;

    foreach ($rates as $rate) {
        $show_submit = true;

        $rid = $rate->id;

        // Build the fields for this row in the table. We'll be adding
        // these to the form several times, so it's easier if they are
        // individual variables rather than in an array.

        $product = array(
            '#markup' => l($rate->title, 'admin/commerce/products/' . $rate->product_id)
        );
        $product_id = array(
            '#type' => 'value',
            '#value' => $rate->product_id,
        );
        $shipping_NZ = array(
            '#id' => 'rate-' . $rid . '-shipping_NZ',
            '#type' => 'textfield',
            '#size' => 4,
            '#prefix' => '<div class="textfield_with_currency"><span class="currency_symbol">USD $</span>',
            '#suffix' => '</div>',
            '#default_value' => commerce_currency_amount_to_decimal($rate->shipping_NZ, 'USD'),
        );
        $shipping_AUS = array(
            '#id' => 'rate-' . $rid . '-shipping_AUS',
            '#type' => 'textfield',
            '#size' => 4,
            '#prefix' => '<div class="textfield_with_currency"><span class="currency_symbol">USD $</span>',
            '#suffix' => '</div>',
            '#default_value' => commerce_currency_amount_to_decimal($rate->shipping_AUS, 'USD'),
        );
        $shipping_EA_NA_UC_EUR_SP = array(
            '#id' => 'rate-' . $rid . '-shipping_EA_NA_UC_EUR_SP',
            '#type' => 'textfield',
            '#size' => 4,
            '#prefix' => '<div class="textfield_with_currency"><span class="currency_symbol">USD $</span>',
            '#suffix' => '</div>',
            '#default_value' => commerce_currency_amount_to_decimal($rate->shipping_EA_NA_UC_EUR_SP, 'USD'),
        );
        $shipping_RW = array(
            '#id' => 'rate-' . $rid . '-shipping_RW',
            '#type' => 'textfield',
            '#size' => 4,
            '#prefix' => '<div class="textfield_with_currency"><span class="currency_symbol">USD $</span>',
            '#suffix' => '</div>',
            '#default_value' => commerce_currency_amount_to_decimal($rate->shipping_RW, 'USD'),
        );
        $shipping_addon_pct = array(
            '#id' => 'rate-' . $rid . '-shipping_addon_pct',
            '#type' => 'textfield',
            '#size' => 4,
            '#prefix' => '<div class="textfield_with_percent"><span class="percent_symbol">%</span>',
            '#suffix' => '</div>',
            '#default_value' => $rate->shipping_addon_pct,
        );
        $delete_link = array(
            '#markup' => l('delete', 'admin/commerce/config/shipping/nzpo_rates/delete/' . $rid)
        );

        // Include the fields so they'll be rendered and named
        // correctly, but they'll be ignored here when rendering as
        // we're using #theme => table.
        //
        // Note that we're using references to the variables, not just
        // copying the values into the array.

        $form['rates'][] = array(
            'product_id' => &$product_id,
            'product' => &$product,
            'shipping_NZ' => &$shipping_NZ,
            'shipping_AUS' => &$shipping_AUS,
            'shipping_EA_NA_UC_EUR_SP' => &$shipping_EA_NA_UC_EUR_SP,
            'shipping_RW' => &$shipping_RW,
            'shipping_addon_pct' => &$shipping_addon_pct,
            'delete_link' => $delete_link,
        );

        // Now add references to the fields to the rows that
        // `theme_table()` will use.
        //
        // As we've used references, the table will use the very same
        // field arrays as the FAPI used above.

        $form['rates']['#rows'][] = array(
            array('data' => &$product),
            array('data' => &$shipping_NZ),
            array('data' => &$shipping_AUS),
            array('data' => &$shipping_EA_NA_UC_EUR_SP),
            array('data' => &$shipping_RW),
            array('data' => &$shipping_addon_pct),
            array('data' => &$delete_link),
        );

        // Because we've used references we need to `unset()` our
        // variables. If we don't then every iteration of the loop will
        // just overwrite the variables we created the first time
        // through leaving us with a form with 3 copies of the same fields.

        unset($product_id);
        unset($product);
        unset($shipping_NZ);
        unset($shipping_AUS);
        unset($shipping_EA_NA_UC_EUR_SP);
        unset($shipping_RW);
        unset($shipping_addon_pct);
        unset($delete_link);
    }

    $form['priority'] = array(
        '#type' => 'value',
        '#value' => $priority ? 1 : 0,
    );


    if ($show_submit) {
        $form['save'] = array(
            '#type' => 'submit',
            '#value' => t('Save changes'),
        );
    } else {
        $form['rates']['#rows'][] = array(
            array(
                'data' => t('There are no rates defined for this method'),
                'colspan' => 7,
            )
        );
    }

    return $form;
}

/**
 * Submit function for 'commerce_nzpo_rates_form' form
 */
function commerce_nzpo_rates_form_submit($form, &$form_state) {
    $rates = $form_state['values']['rates'];
    $priority = $form_state['values']['priority'];

    foreach ($rates as $rate) {
        $rate['priority'] = $priority;
        commerce_nzpo_rates_save($rate, false);
    }

    drupal_set_message(t('Shipping rates updated.'));
}

/**
 * Saves new rate for product
 * 
 * @param array $rate
 */
function commerce_nzpo_rates_save($rate, $show_message = true) {
    $priority = $rate['priority'];
    $product_id = $rate['product_id'];
    
    $amount_fields = array(
        'shipping_NZ',
        'shipping_AUS',
        'shipping_EA_NA_UC_EUR_SP',
        'shipping_RW',
    );

    foreach ($amount_fields as $amount_field) {
        $rate[$amount_field] = commerce_currency_decimal_to_amount($rate[$amount_field], 'USD');
    }

    $exist = db_select('commerce_nzpo_shipping_rates', 'sr')
            ->fields('sr')
            ->condition('product_id', $product_id)
            ->condition('priority', $priority)
            ->execute()
            ->fetchObject();

    if ($exist) {
        db_update('commerce_nzpo_shipping_rates')
                ->fields($rate)
                ->condition('product_id', $product_id)
                ->condition('priority', $priority)
                ->execute();
        if ($show_message) {
            drupal_set_message(t('Shipping rate updated.'));
        }
    } else {
        db_insert('commerce_nzpo_shipping_rates')
                ->fields($rate)
                ->execute();
        if ($show_message) {
            drupal_set_message(t('Shipping rate saved.'));
        }
    }
}

/**
 * Removes rate by id
 * 
 * @param integer $rate_id
 */
function commerce_nzpo_delete_rate($rate_id = 0) {
    $return_url = 'admin/commerce/config/shipping/nzpo_rates';

    if (is_numeric($rate_id) && $rate_id > 0) {
        $exist = db_select('commerce_nzpo_shipping_rates', 'sr')
                ->fields('sr')
                ->condition('id', $rate_id)
                ->execute()
                ->fetchObject();
        if ($exist) {
            if ($exist->priority) {
                dpm($exist);
                $return_url .= '/priority';
            }
            db_delete('commerce_nzpo_shipping_rates')
                    ->condition('id', $rate_id)
                    ->execute();
            drupal_set_message(t('Shipping rate deleted.'));
        }
    }

    drupal_goto($return_url);
}
