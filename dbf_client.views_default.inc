<?php

/**
 * Implements hook_views_default_views().
 */
function dbf_client_views_default_views() {
  $views = array();
  
  $view = new view();
  $view->name = 'clients';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'dbf_client';
  $view->human_name = 'Clients';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Clients administration';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit any dbf_client entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« primera';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ anterior';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'siguiente ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'última »';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'name_1' => 'name_1',
    'CCODCLI' => 'CCODCLI',
    'CNOMCOM' => 'CNOMCOM',
    'CNOMCLI' => 'CNOMCLI',
    'CDNICIF' => 'CDNICIF',
    'CCODGRUP' => 'CCODGRUP',
    'CTFO1CLI' => 'CTFO1CLI',
    'name' => 'name',
    'created' => 'created',
    'status' => 'status',
    'nothing' => 'nothing',
    'nothing_1' => 'nothing_1',
  );
  $handler->display->display_options['style_options']['default'] = 'id';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'CCODCLI' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'CNOMCOM' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'CNOMCLI' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'CDNICIF' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'CCODGRUP' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'CTFO1CLI' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: DBF Client: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'dbf_client';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Author';
  /* Field: DBF Client: Dbf client ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = 'Id';
  $handler->display->display_options['fields']['id']['separator'] = '';
  /* Field: DBF Client: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
  /* Field: DBF Client: CCODCLI */
  $handler->display->display_options['fields']['CCODCLI']['id'] = 'CCODCLI';
  $handler->display->display_options['fields']['CCODCLI']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['CCODCLI']['field'] = 'CCODCLI';
  $handler->display->display_options['fields']['CCODCLI']['element_label_colon'] = FALSE;
  /* Field: DBF Client: CNOMCOM */
  $handler->display->display_options['fields']['CNOMCOM']['id'] = 'CNOMCOM';
  $handler->display->display_options['fields']['CNOMCOM']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['CNOMCOM']['field'] = 'CNOMCOM';
  $handler->display->display_options['fields']['CNOMCOM']['element_label_colon'] = FALSE;
  /* Field: DBF Client: CNOMCLI */
  $handler->display->display_options['fields']['CNOMCLI']['id'] = 'CNOMCLI';
  $handler->display->display_options['fields']['CNOMCLI']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['CNOMCLI']['field'] = 'CNOMCLI';
  $handler->display->display_options['fields']['CNOMCLI']['element_label_colon'] = FALSE;
  /* Field: DBF Client: CDNICIF */
  $handler->display->display_options['fields']['CDNICIF']['id'] = 'CDNICIF';
  $handler->display->display_options['fields']['CDNICIF']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['CDNICIF']['field'] = 'CDNICIF';
  $handler->display->display_options['fields']['CDNICIF']['element_label_colon'] = FALSE;
  /* Field: DBF Client: CCODGRUP */
  $handler->display->display_options['fields']['CCODGRUP']['id'] = 'CCODGRUP';
  $handler->display->display_options['fields']['CCODGRUP']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['CCODGRUP']['field'] = 'CCODGRUP';
  $handler->display->display_options['fields']['CCODGRUP']['element_label_colon'] = FALSE;
  /* Field: DBF Client: CTFO1CLI */
  $handler->display->display_options['fields']['CTFO1CLI']['id'] = 'CTFO1CLI';
  $handler->display->display_options['fields']['CTFO1CLI']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['CTFO1CLI']['field'] = 'CTFO1CLI';
  $handler->display->display_options['fields']['CTFO1CLI']['element_label_colon'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'task/[id]';
  /* Field: DBF Client: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: DBF Client: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'dbf_client';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Published';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['status']['alter']['text'] = '<img src="/sites/default/files/admin_icons/plus-circle.png" />';
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['empty'] = '<img src="/sites/default/files/admin_icons/minus-circle.png" />';
  $handler->display->display_options['fields']['status']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['status']['separator'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'dbf_client/[id]/edit';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'dbf_client/[id]/delete';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Filter criterion: DBF Client: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'dbf_client';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'client' => 'client',
  );
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'clients';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Clients administration';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['clients'] = array(
    t('Master'),
    t('Clients administration'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« primera'),
    t('‹ anterior'),
    t('siguiente ›'),
    t('última »'),
    t('Author'),
    t('Id'),
    t('.'),
    t('Name'),
    t('CCODCLI'),
    t('CNOMCOM'),
    t('CNOMCLI'),
    t('CDNICIF'),
    t('CCODGRUP'),
    t('CTFO1CLI'),
    t('Created'),
    t('Published'),
    t('<img src="/sites/default/files/admin_icons/plus-circle.png" />'),
    t('<img src="/sites/default/files/admin_icons/minus-circle.png" />'),
    t('Edit'),
    t('Delete'),
    t('Page'),
  );
    
  $views['clients'] = $view;
  
  return $views;
}