<?php

/**
 * @file
 * Enables modules and site configuration for a civicrm site installation.
 */

/**
 * Implements hook_install_tasks().
 */
function civicrmprofile_install_tasks($install_state){
  $tasks = array(
    'civicrmprofile_permissions' => array(
      'display_name' => st('Permissions'),
      'display' => FALSE,
      'type' => 'normal'
    ),
  );
  return $tasks;
}

/**
 * Install task to configure CRM permissions.
 * Only Drupal core permissions are set here
 */
function civicrmprofile_permissions() {
  // drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;

  if ($context === "platform") {
    // platform install, do nothing
    drush_log('civicrmprofile_permissions() platform install, do nothing' , 'notice');

  } elseif ($context === "site") {
    // CRM site install
    drush_log('civicrmprofile_permissions() site install' , 'notice');

    // 1) Anonymous users
    // used for users that don't have a user account or that are not authenticated
    user_role_grant_permissions(DRUPAL_ANONYMOUS_RID, array(
     'access content',
    ));

    // 2) Authenticated users
    // this role is automatically granted to all logged in users
    user_role_grant_permissions(DRUPAL_AUTHENTICATED_RID, array(
      'access content',
      'change own username',
      'cancel account',
      'Set up TFA for account',
    ));

    // 3) Administrators
    // the role for site administrators, with all available permissions assigned.
    user_role_grant_permissions(variable_get('user_admin_role'), array_keys(module_invoke_all('permission')));

  } else {
     // something is wrong...
     drush_log('wrong context in civicrmprofile_permissions(): ' . $context, 'warning');
  }
}
