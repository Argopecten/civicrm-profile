<?php

/**
 * 2 functions to implement drush_hook_post_COMMAND().
 *
 *   - drush_civicrmprofile_post_provision_install()
 *   - drush_civicrmprofile_post_provision_verify()
 *
 * CiviCRM modules are not yet enabled when .install file runs,
 * so CRM configuration will be done here, via drush hook allowing
 * Aegir to run extra code after install tasks
 *
 */

/**
 * drush_civicrmprofile_post_provision_install()
 *
 * called after Drupal modules enabled, but before CiviCRM setup
 * just a spaceholder for now
 *
 */
function drush_civicrmprofile_post_provision_install() {
  // drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  drush_log('context_type: ' . $context, 'debug');
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

    $role_name='anonymous user';
    // grant basic CRM permissions to anonymous user
    _drush_civicrmprofile_basic_permissions('anonymous user');

    // grant basic CRM permissions to authenticated user
    _drush_civicrmprofile_basic_permissions('authenticated user');

    // set basic CRM permissions to CRM user
    _drush_civicrmprofile_crmuser_permissions();

    // grant full superuser permissions
    _drush_civicrmprofile_superuser_permissions();

    // grant CRM admin permissions
    _drush_civicrmprofile_crmadmin_permissions();

    // revoke all CRM permissions from site admin
    _drush_civicrmprofile_revoke_all_permissions('site admin');

  } else {
    // something is wrong...
    drush_log('wrong context at drush_civicrmprofile_post_provision_verify: ' . $context, 'warning');
  }
}

// grant basic CRM permissions
function _drush_civicrmprofile_basic_permissions($role_name) {
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'access uploaded files',
        'profile create',
        'profile view',
      )
    )
  );
}

// grant CRM user permissions
function _drush_civicrmprofile_crmuser_permissions() {
  $role_name='crm user';
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'view my contact',
        'edit my contact',
        'access CiviCRM',
        'access Contact Dashboard',
        'add contacts',
      )
    )
  );
}

// grant superuser permissions
function _drush_civicrmprofile_superuser_permissions() {
  $role_name='super user';
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'access CiviCRM',
        'access Contact Dashboard',
        'edit inbound email basic information',
        'add contact notes',
        'edit all contacts',
        'delete contacts',
        'import contacts',
        'edit groups',
        'profile listings and forms',
        'profile edit',
        'access all custom data',
        'delete activities',
        'manage tags',
        'administer Tagsets',
        'view all notes',
      )
    )
  );
}

// grant CRM admin permissions
function _drush_civicrmprofile_crmadmin_permissions() {
  $role_name='crm admin';
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'access deleted contacts',
        'administer reserved tags',
        'force merge duplicate contacts',
        'edit message templates',
        'edit user-driven message templates',
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
