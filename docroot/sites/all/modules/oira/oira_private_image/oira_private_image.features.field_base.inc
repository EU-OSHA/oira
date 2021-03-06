<?php
/**
 * @file
 * oira_private_image.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function oira_private_image_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_private_images'.
  $field_bases['field_private_images'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_private_images',
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => 0,
      'entity_translation_sync' => array(
        0 => 'fid',
      ),
      'uri_scheme' => 'private',
    ),
    'translatable' => 0,
    'type' => 'image',
  );

  return $field_bases;
}
