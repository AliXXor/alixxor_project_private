<?php

/**
 * @file
 * Theme functions and display logic
 *
 */

/**
 * Displays the table for all invoices including all invoice details
 *
 */
function commerce_invoices_manage_invoices() {
  global $base_url;

  $query = db_select('commerce_product', 'p')
    ->extend('PagerDefault');
  $query->limit(50)
    ->condition('type','invoice');
  $query->join('field_data_commerce_price', 'fdp', 'p.product_id = fdp.entity_id');
  $query->fields('p', array('product_id', 'title', 'status'))
    ->fields('fdp', array('commerce_price_amount'))
    ->orderBy('p.product_id', 'DESC');
  $invoices = $query->execute()->fetchAll();

  //Prepare table header and rows array.
  $header = array('Invoice #', 'Company Name', 'Amount', 'Status', 'Link');
  $rows = array();

  foreach($invoices as $invoice) {
    $invoice_no = substr($invoice->title, 9, strpos($invoice->title, '(') - 9);
    $company_name = substr($invoice->title, strpos($invoice->title, '(') + 1, strpos($invoice->title, ')') - strpos($invoice->title, '(') - 1);
    $price = $invoice->commerce_price_amount;
    $status = $invoice->status ? t('Pending') : t('Paid');
    $path = 'commerce-invoices/' . $invoice->product_id . '/' . drupal_hmac_base64($invoice->product_id, drupal_get_private_key() . drupal_get_hash_salt());
    $rows[] = array(
      check_plain($invoice_no),
      check_plain($company_name),
      commerce_currency_format($price, commerce_default_currency()),
      $status,
      url($path, array('absolute' => true)),
    );
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager');
  return $output;
}
