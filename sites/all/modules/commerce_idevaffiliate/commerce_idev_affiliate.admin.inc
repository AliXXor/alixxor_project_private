<?php

/**
 * @file
 * Functions to configure product mapping details & render configuration forms.
 */

/**
 * Callback from commerce_idev_affiliate_menu().
 */
function commerce_idev_affiliate_add_form($form, &$form_state) {
  $form['commerce_idev_affiliate_profile_id'] = array(
    '#type' => 'textfield',
    '#title' => t('iDevAffiliate profile id'),
    '#default_value' => variable_get('commerce_idev_affiliate_profile_id', ''),
    '#description' => t('Enter the iDevAffiliate profile id.'),
    '#required' => TRUE,
  );
  $form['import'] = array(
    '#type' => 'markup',
    '#markup' => '<ul class="action-links">
      <li><a href="/admin/idev-affiliate-mapping/import">' . t('Import') . '</a></li>
      <li><a href="/admin/idev-affiliate-mapping/export">' . t('Export') . '</a></li>
      </ul>',
    '#tree' => TRUE,
  );

  $form['#tree'] = TRUE;

  $form['idev_affiliate_mapping_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mapping'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="idev-affiliate-fieldset-wrapper" class="row">',
    '#suffix' => '</div>',
  );
  if (empty($form_state['num_fields'])) {
    $form_state['num_fields'] = 1;
  }
  for ($i = 0; $i < $form_state['num_fields']; $i++) {
    $form['idev_affiliate_mapping_fieldset'][$i]['idev_affiliate_commerce_sku'] = array(
      '#type' => 'textfield',
      '#title' => t('Commerce SKU'),
      '#prefix' => '<div class="col-commerce-sku">',
      '#suffix' => '</div>',
    );
    $form['idev_affiliate_mapping_fieldset'][$i]['idev_affiliate_product_id'] = array(
      '#type' => 'textfield',
      '#title' => t('iDevAffiliate Product ID'),
      '#prefix' => '<div class="col-product-id">',
      '#suffix' => '</div>',
    );
    $form['idev_affiliate_mapping_fieldset'][$i]['idev_affiliate_remove'] = array(
      '#title' => t('Remove'),
      '#type' => 'checkbox',
      '#default_value' => 0,
      '#prefix' => '<div class="col-remove">',
      '#suffix' => '</div>',
    );
  }
  $form['idev_affiliate_add_field'] = array(
    '#type' => 'submit',
    '#value' => t('Add new product'),
    '#submit' => array('commerce_idev_affiliate_add_more_fields'),
    '#ajax' => array(
      'callback' => 'commerce_idev_affiliate_add_more_callback',
      'wrapper' => 'idev-affiliate-fieldset-wrapper',
    ),
  );
  $form['idev_affiliate_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#validate' => array('commerce_idev_affiliate_add_form_validate'),
    '#submit' => array('commerce_idev_affiliate_add_form_submit'),
  );

  // Already mapped commerce products.
  $header = array('Commerce SKU', 'iDevAffiliate Product ID', 'Operations');
  $results = _commerce_idev_affiliate_get_all_field_mapping();
  $rows = array();
  foreach ($results as $result) {
    $rows[] = array(
      $result->commerce_sku,
      $result->product_id,
      l(t('Delete'), 'admin/idev-affiliate-mapping/delete/' . $result->id),
    );
  }
  if ($rows != NULL) {
    $form['idev_affiliate_table_title'] = array(
      '#type' => 'markup',
      '#markup' => t('Current Mapped Forms'),
      '#prefix' => '<h3>',
      '#suffix' => '</h3>',
      '#tree' => TRUE,
    );
    $form['mapped_products_table'] = array(
      '#type' => 'markup',
      '#title' => t('Current Mapped Forms'),
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
      '#tree' => TRUE,
    );
  }
  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the mapping in it.
 */
function commerce_idev_affiliate_add_more_callback($form, $form_state) {
  return $form['idev_affiliate_mapping_fieldset'];
}

/**
 * Function to validate commerce_idev_affiliate_add_form.
 */
function commerce_idev_affiliate_add_form_validate($form, &$form_state) {
  $mapping_details = $form_state['values']['idev_affiliate_mapping_fieldset'];
  if (!is_array($mapping_details)) {
    drupal_set_message(t("No product mapping details has been added! Profile ID has been saved."), 'warning');
    return TRUE;
  }
}

/**
 * Submit handler to save the mapping configuration.
 */
function commerce_idev_affiliate_add_form_submit($form, &$form_state) {
  variable_set('commerce_idev_affiliate_profile_id', $form_state['values']['commerce_idev_affiliate_profile_id']);
  $mapping_details = $form_state['values']['idev_affiliate_mapping_fieldset'];
  if (!is_array($mapping_details)) {
    drupal_set_message(t("No product mapping details has been added! Profile ID has been saved."), 'warning');
    return TRUE;
  }
  foreach ($mapping_details as $mapping_detail) {
    $idev_affiliate_commerce_sku = $mapping_detail['idev_affiliate_commerce_sku'];
    $idev_affiliate_product_id = $mapping_detail['idev_affiliate_product_id'];
    if ($idev_affiliate_commerce_sku && $idev_affiliate_product_id) {
      db_query("INSERT INTO {idev_affiliate} (commerce_sku, product_id)
        VALUES (:commerce_sku, :product_id)", array(
          ':commerce_sku' => $idev_affiliate_commerce_sku,
          ':product_id' => $idev_affiliate_product_id,
        )
      );
    }
  }
}

/**
 * Submit handler for the "Add new product" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function commerce_idev_affiliate_add_more_fields($form, &$form_state) {
  $form_state['num_fields']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback from commerce_idev_affiliate_menu().
 *
 * Confirmation form before deleting mapping.
 */
function commerce_idev_affiliate_delete_form($form, &$form_state, $id = NULL) {
  $form_state['idev_affiliate_table_product_id'] = $id;
  $form['idev_affiliate_delete'] = array(
    '#type' => 'markup',
    '#markup' => t('Are you sure you want to delete this mapping?'),
    '#prefix' => '<h3>',
    '#suffix' => '</h3>',
  );
  $form['idev_affiliate_delete_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('commerce_idev_affiliate_delete_form_submit'),
    '#description' => t('This action cannot be undone.'),
  );
  $form['idev_affiliate_cancel'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Cancel'), 'admin/config/system/idev-affiliate-mapping/add'),
  );
  return $form;
}

/**
 * Submit handler to delete mapped forms.
 */
function commerce_idev_affiliate_delete_form_submit($form, &$form_state) {
  $id = $form_state['idev_affiliate_table_product_id'];
  db_query("DELETE FROM {idev_affiliate} WHERE id = :id", array(':id' => $id));
  drupal_goto('admin/config/system/idev-affiliate-mapping/add');
}

/**
 * Helper functions to get all mapping details.
 *
 * @return object
 *   All mapping details as object.
 */
function _commerce_idev_affiliate_get_all_field_mapping() {
  $results = db_query("SELECT id, commerce_sku, product_id FROM {idev_affiliate}");
  return $results;
}

/**
 * Callback from commerce_idev_affiliate_menu().
 */
function commerce_idev_affiliate_export_form($form, &$form_state) {
  $mapping_details = _commerce_idev_affiliate_get_all_field_mapping();

  $export = array();
  foreach ($mapping_details as $mapping_detail) {
    $export[] = $mapping_detail;
  }
  $export_json = json_encode($export);
  $form['idev_affiliate_export'] = array(
    '#title' => t('Export'),
    '#type' => 'textarea',
    '#value' => $export_json,
  );

  return $form;
}

/**
 * Callback from commerce_idev_affiliate_menu().
 *
 * Form for importing mapping.
 */
function commerce_idev_affiliate_import_form($form, &$form_state) {
  $form['idev_affiliate_import'] = array(
    '#title' => t('Import'),
    '#type' => 'textarea',
    '#default_value' => NULL,
  );
  $form['idev_affiliate_override'] = array(
    '#title' => t('Replace an existing Product mapping, if one exists with the same commerce SKU'),
    '#type' => 'checkbox',
    '#default_value' => FALSE,
  );
  $form['idev_affiliate_import_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('commerce_idev_affiliate_import_form_submit'),
  );
  return $form;
}

/**
 * Submit handler to save the mapping configuration.
 */
function commerce_idev_affiliate_import_form_submit($form, &$form_state) {
  $override_check = $form_state['values']['idev_affiliate_override'];
  // TODO: Error check.
  $mapping_details = json_decode($form_state['values']['idev_affiliate_import'], TRUE);
  foreach ($mapping_details as $mapping_detail) {
    $commerce_sku = $mapping_detail['commerce_sku'];
    $product_id = $mapping_detail['product_id'];
    // Get mapping dtails if the entered SKU is already mapped.
    $mapping = commerce_idev_affiliate_get_field_mapping($commerce_sku);
    if ($mapping) {
      // Allow override or not for import mapping fields.
      if (TRUE == $override_check) {
        $query = db_query("UPDATE {idev_affiliate}
          SET product_id = :product_id WHERE commerce_sku = :commerce_sku", array(
            ':commerce_sku' => $commerce_sku,
            ':product_id' => $product_id,
          )
        );
        drupal_set_message(t('The mapping of SKU, @sku is updated to @pid.', array(
          '@sku' => $commerce_sku,
          '@pid' => $product_id,
        )));
      }
      else {
        drupal_set_message(t('The SKU, @sku is already mapped! Please click on checkbox to override the existing mapping.', array(
          '@sku' => $commerce_sku,
        )));
      }
    }
    else {
      db_query("INSERT INTO {idev_affiliate} (commerce_sku, product_id)
        VALUES (:commerce_sku, :product_id)", array(
          ':commerce_sku' => $commerce_sku,
          ':product_id' => $product_id,
        )
      );
      drupal_set_message(t('The SKU, @sku is mapped with @pid.', array(
        '@sku' => $commerce_sku,
        '@pid' => $product_id,
      )));
    }
  }
  drupal_goto('admin/config/system/idev-affiliate-mapping/add');
}
