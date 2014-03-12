<?php

/**
 * Generates the DBF Client type editing form.
 */
function dbf_client_type_form($form, &$form_state, $dbfclient_type, $op = 'edit') {

  if ($op == 'clone') {
    $dbfclient_type->label .= ' (cloned)';
    $dbfclient_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $dbfclient_type->label,
    '#description' => t('The human-readable name of this DBF Client type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($dbfclient_type->type) ? $dbfclient_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $dbfclient_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'dbf_client_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this DBF Client type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($dbfclient_type->description) ? $dbfclient_type->description : '',
    '#description' => t('Description about the DBF Client type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save DBF Client type'),
    '#weight' => 40,
  );

  if (!$dbfclient_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete DBF Client type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('dbf_client_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing dbfclient_type.
 */
function dbf_client_type_form_submit(&$form, &$form_state) {
  $dbfclient_type = entity_ui_form_submit_build_entity($form, $form_state);
  //dpm($dbfclient_type);
  // Save and go back.
  dbf_client_type_save($dbfclient_type);

  // Redirect user back to list of DBF Client types.
  $form_state['redirect'] = 'admin/structure/try_entity-types';
}

function dbf_client_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/try_entity-types/' . $form_state['dbf_client_type']->type . '/delete';
}

/**
 * DBF Client type delete form.
 */
function dbf_client_type_form_delete_confirm($form, &$form_state, $dbfclient_type) {
  $form_state['dbfclient_type'] = $dbfclient_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['dbfclient_type_id'] = array('#type' => 'value', '#value' => entity_id('dbf_client_type' ,$dbfclient_type));
  return confirm_form($form,
    t('Are you sure you want to delete DBF Client type %title?', array('%title' => entity_label('dbfclient_type', $dbfclient_type))),
    'dbf_client/' . entity_id('dbf_client_type' ,$dbfclient_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * DBF Client type delete form submit handler.
 */
function dbf_client_type_form_delete_confirm_submit($form, &$form_state) {
  $dbfclient_type = $form_state['dbfclient_type'];
  dbf_client_type_delete($dbfclient_type);

  watchdog('dbf_client_type', '@type: deleted %title.', array('@type' => $dbfclient_type->type, '%title' => $dbfclient_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $dbfclient_type->type, '%title' => $dbfclient_type->label)));

  $form_state['redirect'] = 'admin/structure/dbf_client-types';
}

/**
 * Page to select DBF Client Type to add new entity.
 */
function dbf_client_admin_add_page() {
  $items = array();
  foreach (dbf_client_types() as $dbfclient_type_key => $dbfclient_type) {
    $items[] = l(entity_label('dbf_client_type', $dbfclient_type), 'dbf_client/add/' . $dbfclient_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of DBF Client to create.')));
}

/**
 * Page to show import dbf initial form.
 */
function dbf_client_admin_import_dbf_form($form, &$form_state) {
  drupal_set_title(t('Import Clients DBF'));
  if(isset($form_state['values']['final_content']) && !empty($form_state['values']['final_content'])) {
    //$form['submission'] = array(
      //'#markup' => $form_state['values']['final_content'],
    //);
    return $form_state['values']['final_content'];
  }else{
    $form['dbf_file'] = array(
      '#type' => 'file',
      '#title' => t('Upload DBF File'),
      '#description' => t('The DBF file exported from ContaPlus. Only DBF extension allowed.'),
      '#title_display' => 'before',
      '#weight' => 1,
      '#upload_validators' => array(
        'file_validate_extensions' => array('dbf'),
      ),
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Import DBF File'),
      '#weight' => 40,
    );
  }
  return $form;
}

/**
 * Page to show import dbf initial form submit.
 */
function dbf_client_admin_import_dbf_form_validate($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('dbf'));
  $file = file_save_upload('dbf_file', $validators, FALSE);
  if(!$file){
    form_set_error('dbf_file', t('DBF file is required.'));
  }
}

function _dbf_client_read_dbf($file_path){
  global $user;
  $dbase_processed_records = array();
  $dbase_processed_ids = array();
  $dbf = @dbase_open($file_path, 0);
  if($dbf){
    $fields = dbase_get_header_info($dbf);
    $used_fields = array();
    foreach($fields as $index => $field){
      switch($field['name']){
        case 'CCODCLI':
          $used_fields['CCODCLI'] = $index;
          break;
        case 'CNOMCLI':
          $used_fields['CNOMCLI'] = $index;
          break;
        case 'CNOMCOM':
          $used_fields['CNOMCOM'] = $index;
          break;
        case 'CDIRCLI':
          $used_fields['CDIRCLI'] = $index;
          break;
        case 'CPOBCLI':
          $used_fields['CPOBCLI'] = $index;
          break;
        case 'CPTLCLI':
          $used_fields['CPTLCLI'] = $index;
          break;
        case 'CDNICIF':
          $used_fields['CDNICIF'] = $index;
          break;
        case 'CTFO1CLI':
          $used_fields['CTFO1CLI'] = $index;
          break;
        case 'CFAXCLI':
          $used_fields['CFAXCLI'] = $index;
          break;
        case 'CTFO2CLI':
          $used_fields['CTFO2CLI'] = $index;
          break;
        case 'EMAIL':
          $used_fields['EMAIL'] = $index;
          break;
        case 'CCODGRUP':
          $used_fields['CCODGRUP'] = $index;
          break;
      }
    }
    $records = @dbase_numrecords($dbf);
    //Retrieve all records from DBF file
    for($x = 1; $x <= $records; $x++){
      $record = dbase_get_record($dbf, $x);
      $entity_name = utf8_encode(trim($record[$used_fields['CNOMCLI']]));
      $dbf_info = array();
      $dbf_info['type'] = 'client';
      $dbf_info['name'] = $entity_name;
      $dbf_info['CCODCLI'] = utf8_encode(trim($record[$used_fields['CCODCLI']]));
      $dbf_info['CNOMCLI'] = utf8_encode(trim($record[$used_fields['CNOMCLI']]));
      $dbf_info['CNOMCOM'] = utf8_encode(trim($record[$used_fields['CNOMCOM']]));
      $dbf_info['CDIRCLI'] = utf8_encode(trim($record[$used_fields['CDIRCLI']]));
      $dbf_info['CPOBCLI'] = utf8_encode(trim($record[$used_fields['CPOBCLI']]));
      $dbf_info['CPTLCLI'] = utf8_encode(trim($record[$used_fields['CPTLCLI']]));
      $dbf_info['CDNICIF'] = utf8_encode(trim($record[$used_fields['CDNICIF']]));
      $dbf_info['CTFO1CLI'] = utf8_encode(trim($record[$used_fields['CTFO1CLI']]));
      $dbf_info['CFAXCLI'] = utf8_encode(trim($record[$used_fields['CFAXCLI']]));
      $dbf_info['CTFO2CLI'] = utf8_encode(trim($record[$used_fields['CTFO2CLI']]));
      $dbf_info['EMAIL'] = utf8_encode(trim($record[$used_fields['EMAIL']]));
      $dbf_info['CCODGRUP'] = utf8_encode(trim($record[$used_fields['CCODGRUP']]));
      $dbf_info['uid'] = $user->uid;
      $dbf_info['created'] = REQUEST_TIME;
      $dbf_info['changed'] = REQUEST_TIME;
      $dbf_info['status'] = 1;
      $record_pk = $dbf_info['CCODCLI'];
      $dbase_processed_ids[] = $record_pk;
      $dbase_processed_records[$record_pk] = $dbf_info;
    }
    dbase_close($dbf);
  }
  $resp = array(
    'ids' => $dbase_processed_ids,
    'records' => $dbase_processed_records,
  );
  return $resp;
}

function _dbf_client_read_db_compare_dbf($dbf_ids = array(), $dbase_processed_records = array()){
  // Get all records from dbf_client system database table.
  $result = db_select('dbf_client', 'dc')
    ->fields('dc')
    ->condition('CCODCLI', $dbf_ids,'IN')
    ->execute();
    // Compare the DBF records and the DB records and separate between existing and new.
  $existing_records = array();
  $new_records = $dbase_processed_records;
  foreach($result as $row){
    if(isset($dbase_processed_records[$row->CCODCLI])){
      $existing_records[$row->CCODCLI] = array(
        'db' => $row,
        'dbf' => $dbase_processed_records[$row->CCODCLI],
      );
      unset($new_records[$row->CCODCLI]);
    }
  }
  $resp = array(
    'existing' => $existing_records,
    'new' => $new_records,
  );
  return $resp;
}

function _dbf_client_update_existing_clients($existing_records) {
  $content = array();
  $total_counter = 0;
  $equal_counter = 0;
  $changed_counter = 0;
  $html_change_msg = '<h3>' . t('Change details') . '</h3><ul>';
  foreach($existing_records as $key => $row){
    //Compare DBF record with database record.
    $differences = _dbf_client_difference_dbf_db_records($row['dbf'], $row['db']);
    if(count($differences) > 0){
      //Update the record.
      $html_change_msg .= '<li>' . t('Changing %client_name (%client_code) fields:', array('%client_name' => $row['dbf']['CNOMCLI'], '%client_code' => $row['dbf']['CCODCLI'])) . '<ul>';
      $update_fields = array();
      foreach ($differences as $diff) {
        $update_fields[$diff] = $row['dbf'][$diff];
        $html_change_msg .= '<li>' . t('Field %field_name: From "%db_val" to "%dbf_val"', array('%field_name' => $diff, '%db_val' => $row['db']->$diff, '%dbf_val' => $row['dbf'][$diff])) . '</li>';
        if($diff == 'CCODGRUP'){
          _dbf_client_add_client_service($row['dbf']['CCODCLI'], $row['dbf'][$diff], $row['dbf']['CNOMCLI'], $row['dbf']['CDNICIF'], $row['dbf']['EMAIL']);
          $html_change_msg .= '<li>' . t('Added new service (%service_code) to %client_code. Old client service %service_code_old', array(
            '%service_code' => $row['dbf'][$diff],
            '%client_code' => $row['dbf']['CCODCLI'],
            '%service_code_old' => $row['db']->CCODGRUP,
          )) . '</li>';
        }
      }
      $num_updated = db_update('dbf_client')
        ->fields($update_fields)
        ->condition('CCODCLI', $key, '=')
        ->execute();
        
      $html_change_msg .= '</ul></li>';
      $changed_counter++;
    }else{
      $equal_counter++;
    }
    $total_counter++;
  }
  $html_change_msg .= '</ul>';
  // Content response.
  $content['equal_msg'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('%equal of %total clients not changed', array('%equal' => $equal_counter, '%total' => $total_counter)) . '</h2>',
  );
  $content['changed_msg'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('%changed of %total clients are changed', array('%changed' => $changed_counter, '%total' => $total_counter)) . '</h2>',
  );
  if($changed_counter) {
    $content['change_details'] = array(
      '#type' => 'markup',
      '#markup' => $html_change_msg,
    );
  }
  return $content;
}

function _dbf_client_update_new_clients($new_records) {
  $content = array();
  $content['new_msg'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('%new new clients to add', array('%new' => count($new_records))) . '</h2>',
  );
  if(count($new_records) > 0){
    // Add all clients in the same query (multiple insert).
    $query = db_insert('dbf_client')
      ->fields(array('type', 'name', 'CCODCLI', 'CNOMCLI', 'CNOMCOM', 'CDIRCLI', 'CPOBCLI', 'CPTLCLI', 'CDNICIF', 'CTFO1CLI', 'CFAXCLI', 'CTFO2CLI', 'EMAIL', 'CCODGRUP', 'uid', 'created', 'changed', 'status'));
    $html_new_msg = '<h3>' . t('New clients details') . '</h3><ul>';
    foreach ($new_records as $record) {
      $query->values($record);
      _dbf_client_add_client_service($record['CCODCLI'], $record['CCODGRUP'], $record['CNOMCLI'], $record['CDNICIF'], $record['EMAIL']);
      $html_new_msg .= '<li>' . t('New client: %client_name (%client_code), imported!', array('%client_name' => $record['CNOMCLI'], '%client_code' => $record['CCODCLI'])) . '</li>';
    }
    $html_new_msg .= '</ul>';
    $query->execute();
    // Content response.
    $content['add_details'] = array(
      '#type' => 'markup',
      '#markup' => $html_new_msg,
    );
  }
  return $content;
}

/**
 * Add new service to the client (service will insert in cron task with client_service_update module).
 */
function _dbf_client_add_client_service($client_id, $service_code, $client_name, $dni_cif, $client_email) {
  try {
    db_insert('client_service_update')
      ->fields(array(
        'client_id' => $client_id,
        'service_code' => $service_code,
        'client_name' => $client_name,
        'dni_cif' => $dni_cif,
        'email' => $client_email,
        'created' => REQUEST_TIME,
        'processed' => 0))
      ->execute();
  }catch(Exception $e){
    watchdog('dbf_client', 'Duplicate PK in client_service_update (%client_id - %service_code): %error_message', array('%client_id' => $client_id, '%service_code' => $service_code, '%error_message' => $e->getMessage()), WATCHDOG_ERROR);
  }
}

/**
 * Page to show import csv initial form submit.
 */
function dbf_client_admin_import_dbf_form_submit($form, &$form_state) {
  $content = array();
  $content['generate_pdf'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Generate PDF'), 'dbf_client/print_dbf_results'),
  );
  $validators = array('file_validate_extensions' => array('dbf'));
  $file = file_save_upload('dbf_file', $validators, FALSE);
  $file_path = drupal_realpath($file->uri);
  $dbase_info = _dbf_client_read_dbf($file_path);
  $import_info = _dbf_client_read_db_compare_dbf($dbase_info['ids'], $dbase_info['records']);
  $content_compare = _dbf_client_update_existing_clients($import_info['existing']);
  $content_update = _dbf_client_update_new_clients($import_info['new']);
  $result_content = array();
  $result_content['timestamp'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . format_date(REQUEST_TIME) . '</p>',
  );
  $result_content = array_merge($result_content, $content_compare);
  $result_content = array_merge($result_content, $content_update);
  $result_content['final_msg'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Client import finished!') . '</h2>',
  );
  variable_set('import_dbf_results', $result_content);
  $final_content = array_merge($content, $result_content);
  $form_state['values']['final_content'] = $final_content;
  $form_state['rebuild'] = TRUE;
}

function dbf_client_admin_import_dbf_print() {
  $final_content = variable_get('import_dbf_results', array());
  return pdf_using_mpdf_api(drupal_render($final_content));
}

function _dbf_client_difference_dbf_db_records($dbf_record, $db_record){
  $resp = array();
  if($dbf_record['CNOMCLI'] != $db_record->CNOMCLI) $resp[] = 'CNOMCLI';
  if($dbf_record['CNOMCOM'] != $db_record->CNOMCOM) $resp[] = 'CNOMCOM';
  if($dbf_record['CDIRCLI'] != $db_record->CDIRCLI) $resp[] = 'CDIRCLI';
  if($dbf_record['CPOBCLI'] != $db_record->CPOBCLI) $resp[] = 'CPOBCLI';
  if($dbf_record['CPTLCLI'] != $db_record->CPTLCLI) $resp[] = 'CPTLCLI';
  if($dbf_record['CDNICIF'] != $db_record->CDNICIF) $resp[] = 'CDNICIF';
  if($dbf_record['CTFO1CLI'] != $db_record->CTFO1CLI) $resp[] = 'CTFO1CLI';
  if($dbf_record['CFAXCLI'] != $db_record->CFAXCLI) $resp[] = 'CFAXCLI';
  if($dbf_record['CTFO2CLI'] != $db_record->CTFO2CLI) $resp[] = 'CTFO2CLI';
  if($dbf_record['EMAIL'] != $db_record->EMAIL) $resp[] = 'EMAIL';
  if($dbf_record['CCODGRUP'] != $db_record->CCODGRUP) $resp[] = 'CCODGRUP';
  return $resp;
}

/**
 * Add new DBF Client page callback.
 */
function dbf_client_add($type) {
  $dbfclient_type = dbf_client_types($type);

  $dbfclient = entity_create('dbf_client', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('dbfclient_type', $dbfclient_type))));

  $output = drupal_get_form('dbf_client_form', $dbfclient);

  return $output;
}

/**
 * DBF Client Form.
 */
function dbf_client_form($form, &$form_state, $dbfclient) {
  $form_state['dbf_client'] = $dbfclient;

  $form['name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Entity name or title'),
  	'#weight' => 0,
    '#default_value' => $dbfclient->name,
  );
  $form['CCODCLI'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client code'),
    '#weight' => 1,
    '#default_value' => $dbfclient->CCODCLI,
  );
  $form['CNOMCLI'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client name'),
    '#weight' => 2,
    '#default_value' => $dbfclient->CNOMCLI,
  );
  $form['CNOMCOM'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client comercial name'),
    '#weight' => 3,
    '#default_value' => $dbfclient->CNOMCOM,
  );
  $form['CDIRCLI'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client address'),
    '#weight' => 4,
    '#default_value' => $dbfclient->CDIRCLI,
  );
  $form['CPOBCLI'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client city'),
    '#weight' => 5,
    '#default_value' => $dbfclient->CPOBCLI,
  );
  $form['CPTLCLI'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Client postal code'),
    '#weight' => 6,
    '#default_value' => $dbfclient->CPTLCLI,
  );
  $form['CDNICIF'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client DNI, NIF or CIF'),
    '#weight' => 7,
    '#default_value' => $dbfclient->CDNICIF,
  );
  $form['CTFO1CLI'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client first phone number'),
    '#weight' => 8,
    '#default_value' => $dbfclient->CTFO1CLI,
  );
  $form['CFAXCLI'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Client fax number'),
    '#weight' => 9,
    '#default_value' => $dbfclient->CFAXCLI,
  );
  $form['CTFO2CLI'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Client second phone number'),
    '#weight' => 10,
    '#default_value' => $dbfclient->CTFO2CLI,
  );
  $form['EMAIL'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client email'),
    '#weight' => 11,
    '#default_value' => $dbfclient->EMAIL,
  );
  $form['CCODGRUP'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Client service code'),
    '#weight' => 12,
    '#default_value' => $dbfclient->CCODGRUP,
  );
  /*
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => $dbfclient->description,
    );
  */

  $form['uid'] = array(
    '#type' => 'value',
  	'#weight' => 15,
    '#value' => $dbfclient->uid,
  );
  
  // Node author information for administrators
  $form['author'] = array(
  		'#type' => 'fieldset',
  		'#title' => t('Authoring information'),
  		'#collapsible' => TRUE,
  		'#collapsed' => TRUE,
  		'#attributes' => array(
  			'class' => array('dbfclient-form-author'),
  		),
  		'#attached' => array(
  			'js' => array(
  				drupal_get_path('module', 'dbf_client') . '/js/dbfclient.admin.js',
  				array(
  					'type' => 'setting',
  					'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
  			),
  		),
  	),
  	'#weight' => 90,
  );
  $dbfclient_creator_user = ($dbfclient->uid)? user_load($dbfclient->uid) : FALSE;
  $username = ($dbfclient_creator_user)? $dbfclient_creator_user->name : '';
  $form['author']['uid'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Authored by'),
  	'#maxlength' => 60,
  	'#autocomplete_path' => 'user/autocomplete',
  	'#default_value' => $username,
  	'#weight' => -1,
  	'#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );
  
  $form['options'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('DBF Client options'),
  	'#collapsible' => TRUE,
  	'#collapsed' => FALSE,
  	'#attributes' => array(
  		'class' => array('dbfclient-form-options'),
  	),
  		'#attached' => array(
  		'js' => array(drupal_get_path('module', 'dbf_client') . '/js/dbfclient.admin.js'),
  	),
  	'#weight' => 95,
  );
  $form['options']['status'] = array(
  		'#type' => 'checkbox',
  		'#title' => t('Published'),
  		'#default_value' => $dbfclient->status,
  );
  field_attach_form('dbf_client', $dbfclient, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 99,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save DBF Client'),
    '#submit' => $submit + array('dbf_client_form_submit'),
  	'#weight' => 100,
  );

  // Show Delete button if we edit DBF Client.
  $dbfclient_id = entity_id('dbf_client' ,$dbfclient);
  if (!empty($dbfclient_id) && dbf_client_access('edit', $dbfclient)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('dbf_client_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'dbf_client_form_validate';
  return $form;
}

function dbf_client_form_validate($form, &$form_state) {

}

/**
 * DBF Client submit handler.
 */
function dbf_client_form_submit($form, &$form_state) {
  $dbfclient = $form_state['dbf_client'];
  //dpm($dbfclient);
  //dpm($form);
  //dpm($form_state);
  entity_form_submit_build_entity('dbf_client', $dbfclient, $form, $form_state);
  dbf_client_save($dbfclient);
  $dbfclient_uri = entity_uri('dbf_client', $dbfclient);
  $form_state['redirect'] = $dbfclient_uri['path'];
  drupal_set_message(t('DBF Client %title saved.', array('%title' => entity_label('dbf_client', $dbfclient))));
}

function dbf_client_form_submit_delete($form, &$form_state) {
  $dbfclient = $form_state['dbf_client'];
  $dbfclient_uri = entity_uri('dbf_client', $dbfclient);
  $form_state['redirect'] = $dbfclient_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function dbf_client_delete_form($form, &$form_state, $dbfclient) {
  $form_state['dbf_client'] = $dbfclient;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['dbfclient_type_id'] = array('#type' => 'value', '#value' => entity_id('dbf_client' ,$dbfclient));
  $dbfclient_uri = entity_uri('dbf_client', $dbfclient);
  return confirm_form($form,
    t('Are you sure you want to delete DBF Client %title?', array('%title' => entity_label('dbf_client', $dbfclient))),
    $dbfclient_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function dbf_client_delete_form_submit($form, &$form_state) {
  $dbfclient = $form_state['dbf_client'];
  //DBF Client entity deletion
  dbf_client_delete($dbfclient);
  drupal_set_message(t('DBF Client %name deleted.', array('%name' => entity_label('dbf_client', $dbfclient))));

  $form_state['redirect'] = 'clients';
}
