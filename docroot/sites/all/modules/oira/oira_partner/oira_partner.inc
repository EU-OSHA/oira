<?php

/**
 * Retrieve partner node by its corresponding section ID.
 *
 * @param integer $id
 *   Section id for the corresponding partner.
 *
 * @return mixed
 *   Returns partner node object or FALSE if not found
 */
function oira_partner_by_section_id_load($id) {
  $data = &drupal_static(__FUNCTION__);
  if (isset($data[$id])) {
    return $data[$id];
  }
  $data[$id] = FALSE;
  // @todo: We could improve with direct query on node & workbench_access_node.
  $q = new EntityFieldQuery();
  $q->entityCondition('entity_type', 'node');
  $q->entityCondition('bundle', 'partner');
  $q->entityCondition('status', 1);
  $result = $q->execute();
  if (isset($result['node'])) {
    $partners = entity_load('node', array_keys($result['node']));
    foreach ($partners as $partner) {
      if (!empty($partner->workbench_access) && in_array($id, $partner->workbench_access)) {
        $data[$id] = $partner;
      }
    }
  }
  return $data[$id];
}

/**
 * Check if the given user matches a partner node (they are in same section).
 *
 * @param stdClass $node
 * @param stdClass $account
 *
 * @return bool
 *   TRUE if the account is partner and matches the current 'partner' $node.
 */
function oira_partner_match_user_partner($node, $account = NULL) {
  if (empty($account)) {
    global $user;
    $account = $user;
  }
  if (empty($account->workbench_access)) {
    workbench_access_user_load_data($account);
  }
  if ($node && $node->type == 'partner' && in_array(ROLE_OIRA_PARTNER, $account->roles) && !empty($account->workbench_access) && !empty($node->workbench_access)) {
    foreach(array_keys($account->workbench_access) as $id_section) {
      if (in_array($id_section, $node->workbench_access)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Retrieve the corresponding partner for a given account.
 *
 * It checks the sections and matches current user sections (editor) with
 * agains partner sections.
 *
 * @param stdClass $account
 *   Account to retrieve partner for - if NULL, global $user is used
 *
 * @return null
 */
function oira_partner_get_account_partner($account = NULL) {
  $ret = NULL;
  if (empty($account)) {
    global $user;
    $account = $user;
  }
  if (empty($account->workbench_access)) {
    workbench_access_user_load_data($account);
  }
  if (!empty($account->workbench_access)) {
    reset($account->workbench_access);
    if ($id_section = key($account->workbench_access)) {
      $ret = oira_partner_by_section_id_load($id_section);
    }
  }
  return $ret;
}

function oira_partner_is_ocp($partner) {
  $ret = FALSE;
  if ($partner && $partner->type == 'partner') {
    /** @var stdClass $wrapper */
    $wrapper = entity_metadata_wrapper('node', $partner);
    if ($type = $wrapper->field_partner_type->value()) {
      return $type->uuid == PARTNER_TYPE_OCP;
    }
  }
  return $ret;
}

function oira_partner_is_mp($partner) {
  $ret = FALSE;
  if ($partner && $partner->type == 'partner') {
    /** @var stdClass $wrapper */
    $wrapper = entity_metadata_wrapper('node', $partner);
    if ($type = $wrapper->field_partner_type->value()) {
      return $type->uuid == PARTNER_TYPE_MP;
    }
  }
  return $ret;
}

function oira_partner_is_fop($partner) {
  $ret = FALSE;
  if ($partner && $partner->type == 'partner') {
    /** @var stdClass $wrapper */
    $wrapper = entity_metadata_wrapper('node', $partner);
    if ($type = $wrapper->field_partner_type->value()) {
      return $type->uuid == PARTNER_TYPE_FOP;
    }
  }
  return $ret;
}

/**
 * Retrieve partner type.
 *
 * @param stdClass $account
 *   User account to check the partner type.
 *
 * @return mixed
 *   Returns partner type (PARTNER_TYPE_FOP, PARTNER_TYPE_MP, PARTNER_TYPE_OCP) or null
 */
function oira_partner_get_partner_type($account = NULL) {
  $partner = oira_partner_get_account_partner($account);

  if (oira_partner_is_ocp($partner)) {
    return PARTNER_TYPE_OCP;
  } elseif (oira_partner_is_mp($partner)) {
    return PARTNER_TYPE_MP;
  } elseif (oira_partner_is_fop($partner)) {
    return PARTNER_TYPE_FOP;
  } else {
    return NULL;
  }

}
