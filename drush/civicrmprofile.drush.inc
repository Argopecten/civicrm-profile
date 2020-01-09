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
    _drush_civicrmprofile_grant_basic_permissions($role_name);

    $role_name='authenticated user';
    // grant basic CRM permissions to authenticated user
    _drush_civicrmprofile_grant_basic_permissions($role_name);
    // grant full CRM user permissions to authenticated user
    _drush_civicrmprofile_grant_crmuser_permissions($role_name);

  } else {
    // something is wrong...
    drush_log('wrong context at drush_civicrmprofile_post_provision_verify: ' . $context, 'warning');
  }
}

// grant basic CRM permissions
function _drush_civicrmprofile_grant_basic_permissions($role_name) {
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

// grant full CRM user permissions
function _drush_civicrmprofile_grant_crmuser_permissions($role_name) {
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'view my contact',
        'edit my contact',
        'access uploaded files',
        'profile create',
        'profile view',
        'add contacts',
        'edit inbound email basic information',
        'access CiviCRM',
        'access Contact Dashboard',
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

?>
