<?php

/**
 * functions to implement drush_hook_post_COMMAND() for:
 *   - provision-install
 *   - provision-verify
 *
 * CiviCRM modules are not yet enabled when .install file runs,
 * so CRM configuration will be done here, via drush hook allowing
 * Aegir to run extra code after install tasks
 *
 */

 /**
  * impplementation of provision-install hooks
  *   3 hooks for the drush command 'provision-install
  *   drush_civicrmprofile_pre_provision_install():
  *      called before the install task, no site/ directory yet
  *   drush_civicrmprofile_provision_install():
  *      called after site/ directory deployed, but before Drupal site install
  *   drush_civicrmprofile_post_provision_install():
  *      called after Drupal modules enabled, but before CiviCRM setup
  *
  */
function drush_civicrmprofile_pre_provision_install() {
  // just a spaceholder for now
  // drush_log('It"s the drush_civicrmprofile_pre_provision_install hook', 'debug');
}

function drush_civicrmprofile_provision_install() {
  // just a spaceholder for now
  // drush_log('It"s the drush_civicrmprofile_provision_install hook', 'debug');
}

function drush_civicrmprofile_post_provision_install() {
  // just a spaceholder for now
  // drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  // drush_log('context_type: ' . $context, 'debug');
  if ($context === "platform") {
    // do nothing
  } elseif ($context === "site") {
    // do nothing
  } else {
    // something is wrong...
    drush_log('wrong context at drush_civicrmprofile_post_provision_install: ' . $context, 'warning');
  }
}

/**
 * impplementation of provision-verify hooks
 *   3 hooks for the drush command 'provision-verify
 *   drush_civicrmprofile_pre_provision_verify():
 *      called before the install task, no site/ directory yet
 *   drush_civicrmprofile_provision_verify():
 *      called after site/ directory deployed, but before Drupal site install
 *   drush_civicrmprofile_post_provision_verify():
 *      called after Drupal modules enabled, but before CiviCRM setup
 *
 */

 function drush_civicrmprofile_pre_provision_verify() {
   // just a spaceholder for now
   // drush_log('It"s the drush_civicrmprofile_pre_provision_verify hook', 'debug');
 }

 function drush_civicrmprofile_provision_verify() {
   // just a spaceholder for now
   // drush_log('It"s the drush_civicrmprofile_provision_verify hook', 'debug');
 }

/**
 * drush_civicrmprofile_post_provision_verify()
 *
 * functions:
 *   - grants CRM permissions during site verification
 *
 */
function drush_civicrmprofile_post_provision_verify() {
  // drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  drush_log('context_type: ' . $context, 'debug');
  if ($context === "platform") {
    // do nothing
  } elseif ($context === "site") {

    // grant basic CRM permissions to anonymous user
    _drush_civicrmprofile_grant_basic_permissions(t('anonymous user'));

    // grant basic CRM permissions to authenticated user
    _drush_civicrmprofile_grant_basic_permissions(t('authenticated user'));
    _drush_civicrmprofile_grant_crmuser_permissions(t('authenticated user'));

  } else {
    // something is wrong...
    drush_log('wrong context at drush_civicrmprofile_post_provision_verify: ' . $context, 'warning');
  }
}

// grant basic CRM permissions
function _drush_civicrmprofile_grant_basic_permissions($role_name) {
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'access CiviMail subscribe/unsubscribe pages',
        'access uploaded files',
        'profile create',
        'profile view',
      )
    )
  );
}

// grant full CRM user permissions
function _drush_civicrmprofile_grant_crmuser_permissions($role_name) {
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'access CiviCRM',
        'access Contact Dashboard',
        'add contacts',
        'edit all contacts',
        'delete contacts',
        'import contacts',
        'add contact notes',
        'profile listings and forms',
        'profile edit',
        'access all custom data',
        'view all notes',
        'delete activities',
        'edit groups',
        'manage tags',
        'administer Tagsets',
        'access CiviMail',
        'edit inbound email basic information',
        'access my cases and activities',
        'access CiviReport',
      )
    )
  );
}

// revoke all CRM permissions
function _drush_civicrmprofile_revoke_all_permissions($role_name) {
  // all permissions from CiviCRM
  $all_crm_perms = module_invoke("CiviCRM", 'permission');
  // drush_log('revoke permissions from ' . $role_name . ' CRM perms:' . print_r($all_crm_perms, TRUE), 'debug');
  // revoke permissions
  drush_invoke('role-remove-perm',array($role_name, array_keys($all_crm_perms)));
}

?>
