<?php
/**
 * @file
 * hwc_practical_tool.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function hwc_practical_tool_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function hwc_practical_tool_node_info() {
  $items = array(
    'practical_tool' => array(
      'name' => 'OiRA Tool',
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => 'Title',
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}