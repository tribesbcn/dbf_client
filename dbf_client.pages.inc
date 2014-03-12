<?php

/**
 * DBF Client Entity view callback.
 */
function dbf_client_view($dbf_info, $view_mode = 'full') {
  drupal_set_title(entity_label('dbf_client', $dbf_info));
  $ev = entity_view('dbf_client', array(entity_id('dbf_client', $dbf_info) => $dbf_info), $view_mode);
  //dpm($ev);
  return $ev;
}
