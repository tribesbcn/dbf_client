<?php

/**
 * @file
 * Pathauto integration for MOH Premises
 */

/**
 * Implements hook_path_alias_types().
 *
 * Used primarily by the bulk delete form.
 */
function dbf_client_path_alias_types() {
  return array('dbf_client/' => t('Client'));
}

/**
 * Implements hook_pathauto().
 */
function dbf_client_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'dbf_client';
      $settings['token_type'] = 'dbf_client';
      $settings['groupheader'] = t('Call Center paths');
      $settings['patterndescr'] = t('Default path pattern');
      $settings['patterndefault'] = 'dbf_client/[dbf_client:name]';
      $settings['batch_update_callback'] = 'dbf_client_pathauto_bulk_update_batch_process';
      $settings['batch_file'] = drupal_get_path('module', 'dbf_client') . '/dbf_client.pathauto.inc';

      $languages = array();
      if (module_exists('locale')) {
        $languages = array(LANGUAGE_NONE => t('language neutral')) + locale_language_list('name');
      }
      return (object) $settings;
    default:
      break;
  }
}

/**
 * Batch processing callback; Generate aliases for nodes.
 */
function dbf_client_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('dbf_client', 'e');
  $query->leftJoin('url_alias', 'ua', "CONCAT('dbf_client/', e.CCODCLI) = ua.source");
  $query->addField('e', 'CCODCLI');
  $query->isNull('ua.source');
  $query->condition('e.CCODCLI', $context['sandbox']['current'], '>');
  $query->orderBy('e.CCODCLI');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'dbf_client');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $pids = $query->execute()->fetchCol();

  dbf_client_update_alias_multiple($pids, 'bulkupdate');
  $context['sandbox']['count'] += count($pids);
  $context['sandbox']['current'] = max($pids);
  $context['message'] = t('Updated alias for premise @pid.', array('@pid' => end($pids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}

/**
 * Update the URL aliases for an individual my_entity.
 *
 * @param $my_entity
 *   A dbf_client object.
 * @param $op
 *   Operation being performed on the dbf_client ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function dbf_client_update_alias($dbf_client, $op, array $options = array()) {
  // Skip processing if the user has disabled pathauto for the dbf_client.
  if (isset($dbf_client->path['pathauto']) && empty($dbf_client->path['pathauto'])) {
    return;
  }

  $options += array(
    'language' => isset($dbf_client->language) ? $dbf_client->language : LANGUAGE_NONE,
  );

  // Skip processing if the dbf_client has no pattern.
  if (!pathauto_pattern_load_by_entity('dbf_client', $dbf_client->type, $options['language'])) {
    return;
  }

  module_load_include('inc', 'pathauto');
  $uri = entity_uri('dbf_client', $dbf_client);
  pathauto_create_alias('dbf_client', $op, $uri['path'], array('dbf_client' => $dbf_client), $dbf_client->type, $options['language']);
}

/**
 * Update the URL aliases for multiple premises.
 *
 * @param $pids
 *   An array of premise IDs.
 * @param $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function dbf_client_update_alias_multiple(array $pids, $op, array $options = array()) {
  $options += array('message' => FALSE);

  $entities = dbf_client_load_multiple($pids);
  foreach ($entities as $dbf_client) {
    dbf_client_update_alias($dbf_client, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($pids), 'Updated URL alias for 1 DBF Client.', 'Updated URL aliases for @count DBF Clients.'));
  }
}
