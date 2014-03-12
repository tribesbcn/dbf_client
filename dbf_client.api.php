<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on dbf_client being loaded from the database.
 *
 * This hook is invoked during $dbf_client loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $dbf_client entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_dbf_client_load(array $entities) {
  $result = db_query('SELECT CCODCLI, name FROM {dbf_client} WHERE CCODCLI IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->CCODCLI = $record->CCODCLI;
  }
}

/**
 * Responds when a $dbf_client is inserted.
 *
 * This hook is invoked after the $dbf_client is inserted into the database.
 *
 * @param DBFClient $dbf_client
 *   The $dbf_client that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_dbf_client_insert(DBFClient $dbf_client) {
  db_insert('mytable')
    ->fields(array(
      'CCODCLI' => entity_id('dbf_client', $dbf_client),
      'extra' => print_r($dbf_client, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $dbf_client being inserted or updated.
 *
 * This hook is invoked before the $dbf_client is saved to the database.
 *
 * @param DBFClient $dbf_client
 *   The $dbf_client that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_dbf_client_presave(DBFClient $dbf_client) {
  $dbf_client->name = 'foo';
}

/**
 * Responds to a $dbf_client being updated.
 *
 * This hook is invoked after the $dbf_client has been updated in the database.
 *
 * @param DBFClient $dbf_client
 *   The $dbf_client that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_dbf_client_update(DBFClient $dbf_client) {
  db_update('mytable')
    ->fields(array('extra' => print_r($dbf_client, TRUE)))
    ->condition('CCODCLI', entity_id('dbf_client', $dbf_client))
    ->execute();
}

/**
 * Responds to $dbf_client deletion.
 *
 * This hook is invoked after the $dbf_client has been removed from the database.
 *
 * @param DBFClient $dbf_client
 *   The $dbf_client that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_dbf_client_delete(DBFClient $dbf_client) {
  db_delete('dbf_client')
    ->condition('CCODCLI', entity_id('dbf_client', $dbf_client))
    ->execute();
}

/**
 * Act on a dbf_client that is being assembled before rendering.
 *
 * @param $dbf_client
 *   The dbf_client entity.
 * @param $view_mode
 *   The view mode the dbf_client is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $dbf_client->content prior to rendering. The
 * structure of $dbf_client->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_dbf_client_view($dbf_client, $view_mode, $langcode) {
  $dbf_client->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for dbf_clients.
 *
 * @param $build
 *   A renderable array representing the dbf_client content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * dbf_client content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the dbf_client rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_dbf_client().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_dbf_client_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on dbf_client_type being loaded from the database.
 *
 * This hook is invoked during dbf_client_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of dbf_client_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_dbf_client_type_load(array $entities) {
  $result = db_query('SELECT id, name FROM {dbf_client} WHERE id IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->name = $record->name;
  }
}

/**
 * Responds when a dbf_client_type is inserted.
 *
 * This hook is invoked after the dbf_client_type is inserted into the database.
 *
 * @param DBFClientType $dbf_client_type
 *   The dbf_client_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_dbf_client_type_insert(DBFClientType $dbf_client_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('dbf_client_type', $dbf_client_type),
      'extra' => print_r($dbf_client_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a dbf_client_type being inserted or updated.
 *
 * This hook is invoked before the dbf_client_type is saved to the database.
 *
 * @param DBFClientType $dbf_client_type
 *   The dbf_client_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_dbf_client_type_presave(DBFClientType $dbf_client_type) {
  $dbf_client_type->name = 'foo';
}

/**
 * Responds to a dbf_client_type being updated.
 *
 * This hook is invoked after the dbf_client_type has been updated in the database.
 *
 * @param DBFClientType $dbf_client_type
 *   The dbf_client_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_dbf_client_type_update(DBFClientType $dbf_client_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($dbf_client_type, TRUE)))
    ->condition('id', entity_id('dbf_client_type', $dbf_client_type))
    ->execute();
}

/**
 * Responds to dbf_client_type deletion.
 *
 * This hook is invoked after the dbf_client_type has been removed from the database.
 *
 * @param DBFClientType $dbf_client_type
 *   The dbf_client_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_dbf_client_type_delete(DBFClientType $dbf_client_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('dbf_client_type', $dbf_client_type))
    ->execute();
}

/**
 * Define default dbf_client_type configurations.
 *
 * @return
 *   An array of default dbf_client_type, keyed by machine names.
 *
 * @see hook_default_dbf_client_type_alter()
 */
function hook_default_dbf_client_type() {
  $defaults['main'] = entity_create('dbf_client_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default dbf_client_type configurations.
 *
 * @param array $defaults
 *   An array of default dbf_client_type, keyed by machine names.
 *
 * @see hook_default_dbf_client_type()
 */
function hook_default_dbf_client_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
