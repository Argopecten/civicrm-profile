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
 *   - enable CiviCRM components and extensions
 *
 */
function drush_civicrmprofile_post_provision_verify() {
  // drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  drush_log('context_type: ' . $context, 'debug');
  if ($context === "platform") {
    // do nothing
  } elseif ($context === "site") {

    // enable CiviCRM components
    \Civi\Api4\Setting::set()
      ->addValue('enable_components', [
        'CiviMail',
        'CiviCase',
        'CiviReport',
      ])
      ->setCheckPermissions(FALSE)
      ->execute();

    // Enable CiviCRM extensions
    $extensions = array(
      'org.civicrm.shoreditch',
      'nz.co.fuzion.csvimport',
    );
    _drush_civicrmprofile_enable_extensions($extensions);

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

// Enable CiviCRM extensions
function _drush_civicrmprofile_enable_extensions($extensions) {
  // download is via composer
  foreach ($extensions as $key => $ext) {
    try{
      civicrm_api3('Extension', 'enable', ['key' => $ext,]);
    }
    catch (CiviCRM_API3_Exception $e) {
      $errorMessage = $e->getMessage();
      $errorCode = $e->getErrorCode();
      $errorData = $e->getExtraParams();
      $result = array(
        'is_error' => 1,
        'error_message' => $errorMessage,
        'error_code' => $errorCode,
        'error_data' => $errorData,
      );
      drush_log('civicrmprofile enable_extensions:' . print_r($result, 1), 'warning');
    }
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
        'add contacts',
        'view my contact',
        'edit my contact',
        'access CiviCRM',
        'access Contact Dashboard',
        'add contact notes',
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
        'add contacts',
        'edit all contacts',
        'delete contacts',
        'access deleted contacts',
        'import contacts',
        'access CiviCRM',
        'access Contact Dashboard',
        'edit groups',
        'profile listings and forms',
        'profile edit',
        'access all custom data',
        'view all activities',
        'delete activities',
        'manage tags',
        'administer Tagsets',
        'view all notes',
        'add contact notes',
        'edit inbound email basic information',
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
        'add contacts',
        'view my contact',
        'edit my contact',
        'administer CiviCRM',
        'access Contact Dashboard',
        'add contact notes',
        'administer reserved tags',
        'administer reserved groups',
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
