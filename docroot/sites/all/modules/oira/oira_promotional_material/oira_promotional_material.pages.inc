<?php

function oira_promotional_material_search_form($form, $form_state) {
  global $language;
  $per_page = 10;
  $facets_def = _oira_oira_promotional_material_search_form_get_facets();

  $text = _hwc_practical_tool_get_req_param($form_state, 'text');
  $sort = _hwc_practical_tool_get_req_param($form_state, 'sort', 'date');

  $form = array(
    '#action' => "search",
    '#method' => 'GET',
    '#token' => FALSE,
    'left-column' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('col-md-3')),
    ),
    'content' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('col-md-9')),
    ),
  );
  $form['left-column']['search']['text'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#title' => t('Search'),
    '#maxlength' => 200,
    '#default_value' => $text,
    '#title_display' => 'invisible',
    '#attributes' => array('placeholder' => t('Search promotional materials')),
  );
  $form['left-column']['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  $form['content']['sort_fieldset'] = array(
    /* Needed so that the page passes Web Accessibility test */
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('col-md-12')),
  );
  $form['content']['sort_fieldset']['sort'] = array(
    '#type' => 'select',
    '#title' => t('Order'),
    '#default_value' => $sort,
    '#prefix' => '<div class="sort-container col-xs-3">',
    '#suffix' => '</div>',
    '#options' => array('date' => t('Latest first'), 'alphabetically' => t('Alphabetically')),
  );


  if (!empty($text)) {
    $form['content']['search_phrase'] = array(
      '#type' => 'markup',
      '#markup' => t('Results for !text', array('!text' => '<span class="searched-text"> ' . check_plain($text) .'</span>')),
    );
  }

  // Find if any of this form params are set on GET.
  $is_searched = !empty(array_intersect(array_keys($_GET), array_merge(array_keys($facets_def), array('text', 'sort'))));
  $try_native = FALSE;
  if (!$is_searched) {
    // When no search performed by user, try to filter by current language.
    $try_native = TRUE;
    $_GET['field_language:value'][] = $language->language;
  }

  $response = oira_promotional_material_search($form_state, $per_page);

  if ($try_native && empty($response['result count'])) {
    // If no results are available for current language
    // perform the search with no lang.
    $_GET['field_language:value'] = array('en');
    $response = oira_promotional_material_search($form_state, $per_page);
  }
  
  $facets = array();
  if (!empty($response['search_api_facets'])) {
    $facets = $response['search_api_facets'];
    _hwc_practical_tool_facet_value_clear($facets);
  }
  $query_params = drupal_get_query_parameters();

  foreach ($facets_def as $field_name => $facet_def) {
    if (!empty($facets[$field_name])) {
      $values_list = call_user_func($facet_def['options_callback'], $facet_def['options_callback_args']);
      $options = array();
      foreach ($facets[$field_name] as $facet_value) {
        if (!empty($values_list[$facet_value['filter']])) {
          $options[$facet_value['filter']] = $values_list[$facet_value['filter']] . ' ' . strtr('(@no)', array('@no' => $facet_value['count']));
          $form['left-column'][$field_name] = array(
            '#type' => 'checkboxes',
            '#options' => $options,
            '#default_value' => _hwc_practical_tool_get_req_param($form_state, $field_name, array()),
            '#title' => $facet_def['label'],
          );
        }
      }
      if (!empty($form['left-column'][$field_name]['#default_value'])) {
        foreach ($form['left-column'][$field_name]['#default_value'] as $val) {
          if (!empty($options[$val])) {
            $filter_label = '<span class="glyphicon glyphicon-remove"></span>' . $options[$val];
            $form['left-column']['search']['filters'][] = array(
              '#type' => 'markup',
              '#markup' => l($filter_label, 'oira-tools', array('query' => _hwc_practical_tool_strip_param_value($query_params, $field_name, $val), 'html' => TRUE)),
            );
          }
        }
      }
    }
  }

  $nids = array();
  if (!empty($response['results'])) {
    foreach ($response['results'] as $row) {
      list($lng, $nid) = explode('_', $row['id'], 2);
      $nids[] = $nid;
    }

    $nodes = node_load_multiple($nids);

    pager_default_initialize($response['result count'], $per_page);
    $pager_params = $_GET;
    $page_no = isset($pager_params['page']) ? (int) $pager_params['page'] : 0;
    unset($pager_params['page']);
    $form['content']['results'] = array(
      '#nids' => $nids,
      'results-native' => node_view_multiple($nodes, 'teaser', 0, $language->language),
      '#suffix' => theme('pager', array(
        'tags' => array('<', '<', '', '>', '>'),
        'quantity' => 9,
        'parameters' => $pager_params,
      )),
    );

    $start = $page_no * $per_page;
    $form['content']['sort_fieldset']['search_summary'] = array(
      '#prefix' => '<span class="count-container pull-right">',
      '#suffix' => '</span>',
      '#type' => 'markup',
      '#markup' => t('Showing @start - @limit from @total tools', array('@start' => $start + 1, '@limit' => $start + count($nids), '@total' => $response['result count'])),
    );
  }
  else {
    $form['content']['results']['#markup'] = '<p class="no-results">' . t('No results found to match your search.') . '</p>';
  }

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'hwc_practical_tool') . '/js/checkbox-submit.js',
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function oira_promotional_material_form_oira_promotional_material_search_form_alter(&$form, $form_state) {
  // Unset the form_id and build_id in order not to have nasty urls.
  unset($form['form_id']);
  unset($form['form_build_id']);
}

function _oira_oira_promotional_material_search_form_get_facets() {
  return [
    'field_country' => array(
      'label' => t('Country'),
      'field' => 'field_country',
      'limit' => 50,
      'operator' => 'or',
      'min_count' => 1,
      'missing' => 0,
      'options_callback' => '_hwc_taxonomy_filter_options_list',
      'options_callback_args' => array('country'),
    ),
  ];
}

function oira_promotional_material_search($form_state, $per_page = 10) {
  $facets_def = _oira_oira_promotional_material_search_form_get_facets();

  $page = _hwc_practical_tool_get_req_param($form_state, 'page', 0);
  $text = _hwc_practical_tool_get_req_param($form_state, 'text');
  $sort = _hwc_practical_tool_get_req_param($form_state, 'sort', 'date');
  /** @var SearchApiQuery $query */
  /** @var SearchApiSolrService $server */
  list($server, $query) = hwc_practical_tool_menu_tools_get_query();
  $query->condition('status', 1);
  $query->condition('type', 'promotional_material');
  $query->keys($text);
  $query->fields(array('title_field', 'body:value',
    'field_alternative_title', 'field_alternative_body:value'));
  $query->setOption('search_api_facets', $facets_def);
  foreach ($facets_def as $field_name => $facet_def) {
    $selected_values = _hwc_practical_tool_get_req_param($form_state, $field_name, array());
    if (!empty($selected_values) && is_array($selected_values)) {
      $filter = $query->createFilter($facet_def['operator'], ['facet:' . $field_name]);
      foreach ($selected_values as $value) {
        $filter->condition($field_name, $value);
        $query->filter($filter);
      }
    }
  }
  switch ($sort) {
    case 'alphabetically':
      $query->sort('title2', 'ASC');
      break;

    default:
      $query->sort('field_revised_date', 'DESC');
      break;
  }
  $query->range($page * $per_page, $per_page);

  $response = $server->search($query);
  return $response;
}