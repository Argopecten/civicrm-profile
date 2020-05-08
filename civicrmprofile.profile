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
    'civicrmprofile_sitesettings' => array(
      'display_name' => st('Site settings'),
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
      'setup own tfa',
    ));

    // 3) Administrators
    // the role for site administrators, with all available permissions assigned.
    user_role_grant_permissions(variable_get('user_admin_role'), array_keys(module_invoke_all('permission')));

    // 4) Permissions for CRM users
    $role = user_role_load_by_name(t('crm user'));
    user_role_grant_permissions($role->rid, array(
      'access content',
    ));

    // 5) Permissions for super users
    $role = user_role_load_by_name(t('super user'));
    user_role_grant_permissions($role->rid, array(
      'administer users',
      'assign crm user role',
      'assign super user role',
    ));

    // 6) Permissions for CRM admins
    $role = user_role_load_by_name(t('crm admin'));
    user_role_grant_permissions($role->rid, array(
      'administer users',
      'assign crm user role',
      'assign super user role',
      'assign crm admin role',
      'access toolbar',
    ));

    // 7) Permissions for CRM activists
    $role = user_role_load_by_name(t('activist'));
    user_role_grant_permissions($role->rid, array(
      'access content',
    ));

  } else {
     // something is wrong...
     drush_log('wrong context in civicrmprofile_permissions(): ' . $context, 'warning');
  }
}

/**
 * Install task to configure site settings.
 * - site_name
 * - site_slogan
 * - site_mail
 * - site_frontpage
 * - site_403
 * - site_404
 */
function civicrmprofile_sitesettings() {
  variable_set('site_name',   'CiviHelp');
  variable_set('site_slogan', 'A segítségnyújtás eszköze');

  global $base_url;
  $base_url_parts = parse_url($base_url);
  $adminMail = 'admin@'.$base_url_parts['host'];
  variable_set('site_mail', $adminMail);

  variable_set('site_frontpage', 'civicrm');
  // variable_set('site_403', '');
  // variable_set('site_404', '');
}

/**
 * block_info hook: information schema for block
 */
function civicrmprofile_block_info() {
}

/**
 * block_view hook to set content for block and to make it visible
 */
function civicrmprofile_block_view($delta='') {
}
