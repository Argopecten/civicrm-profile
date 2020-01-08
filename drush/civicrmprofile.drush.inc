<?php
  /**
   * 2 functions to implement drush_hook_post_COMMAND().
   *
   *   - drush_civicrmprofile_post_provision_install()
   *   - drush_civicrmprofile_post_provision_verify()
   *
   * CRM module is not yet enabled  when .install file runs,
   * so CRM permissions will be granted here, via drush hook of
   * drush_hook_post_COMMAND(), allowing Aegir to run extra code after install tasks
   */
function drush_civicrmprofile_post_provision_install() {
  drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  if ($context === "platform") {
    drush_log('context_type: ' . $context, 'debug');
    // do nothing
  } elseif ($context === "site") {
    // revoke CRM permissions from the site admin
    _drush_civicrmprofile_revoke_siteadmin_permissions();
    // grant CRM admin permissions to CRM admin
    _drush_civicrmprofile_grant_crmadmin_permissions();
  } else {
    // something is wrong...
    drush_log('wrong context at drush_civicrmprofile_post_provision_verify: ' . $context, 'warning');
  }
}

function drush_civicrmprofile_post_provision_verify() {
  drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  if ($context === "platform") {
    drush_log('context_type: ' . $context, 'debug');
    // do nothing
  } elseif ($context === "site") {
    // revoke CRM permissions from the site admin
    _drush_civicrmprofile_revoke_siteadmin_permissions();
    // grant CRM admin permissions to CRM admin
    _drush_civicrmprofile_grant_crmadmin_permissions();
  } else {
    // something is wrong...
    drush_log('wrong context at drush_civicrmprofile_post_provision_verify: ' . $context, 'warning');
  }
}

// grant CRM admin permissions for CRM admin
function _drush_civicrmprofile_grant_crmadmin_permissions() {
}

// revoke all CRM permissions from site admin
function _drush_civicrmprofile_revoke_siteadmin_permissions() {
    // $crm_permissions = array_keys(module_invoke("civicrm", 'permission'));
    // drush_log('crm_permissions:' . print_r($crm_permissions, 1), 'debug');
}

?>
