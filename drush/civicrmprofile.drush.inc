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
 *   - enable CiviCRM components and extensions
 *   - set various CRM options: display, name format, ...
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
      'org.civicrm.flexmailer',
      'org.civicrm.angularprofiles',
      'org.civicrm.contactlayout',
    );
    _drush_civicrmprofile_enable_extensions($extensions);

    // grant basic CRM permissions to anonymous user
    _drush_civicrmprofile_grant_basic_permissions('anonymous user');

    // grant basic CRM permissions to authenticated user
    _drush_civicrmprofile_grant_basic_permissions('authenticated user');
    _drush_civicrmprofile_grant_crmuser_permissions('authenticated user');

    // set various CRM options: display, name format, ...
    _drush_civicrmprofile_set_crm_options();

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

/**
 * _drush_civicrmprofile_set_crm_options()
 *
 * set various CRM options: display, name format, ...
 * settings:
 *   - contact options
 *   - mobile phone providers for HU
 *   - phone types
 *   - individual prefixes
 */
function _drush_civicrmprofile_set_crm_options() {
  // 1) contact options
  \Civi\Api4\Setting::set()
    ->addValue('contact_view_options', [1, 3, 4, 5, 6, 10, 14, ])
    ->addValue('contact_edit_options', [12, 14, 15, 16, 7, 8, 5, 1, 2, 3, 4, 6, ])
    ->addValue('user_dashboard_options', [1, 5, 9, ])
    ->addValue('display_name_format', '{contact.individual_prefix}{ }{contact.last_name}{ }{contact.first_name}')
    ->setCheckPermissions(FALSE)
    ->execute();

  // 2) Mobile phone providers
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 5)
    ->addWhere('value', '=', 1)
    ->addValue('label', 'T-Mobile')
    ->addValue('name',  'T-Mobile')
    ->setCheckPermissions(FALSE)
    ->execute();
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 5)
    ->addWhere('value', '=', 2)
    ->addValue('label', 'Vodafone')
    ->addValue('name',  'Vodafone')
    ->setCheckPermissions(FALSE)
    ->execute();
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 5)
    ->addWhere('value', '=', 3)
    ->addValue('label', 'Telenor')
    ->addValue('name',  'Telenor')
    ->setCheckPermissions(FALSE)
    ->execute();

  // 3) phone types
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 35)
    ->addWhere('value', '=', 2)
    ->addValue('label', 'Mobil')
    ->addValue('name', 'Mobil')
    ->addValue('is_default', 1)
    ->setCheckPermissions(FALSE)
    ->execute();
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 35)
    ->addWhere('value', '=', 1)
    ->addValue('label', 'Vezetékes')
    ->addValue('name', 'Vezetékes')
    ->addValue('is_default', 0)
    ->setCheckPermissions(FALSE)
    ->execute();
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 35)
    ->addWhere('id', '>=', 333)
    ->addValue('is_active', 0)
    ->setCheckPermissions(FALSE)
    ->execute();

  // 4) contact prefixes
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 6)
    ->addWhere('value', '=', 1)
    ->addValue('label', 'özv.')
    ->addValue('name', 'özv.')
    ->setCheckPermissions(FALSE)
    ->execute();
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 6)
    ->addWhere('value', '=', 2)
    ->addValue('label', 'ifj.')
    ->addValue('name', 'ifj.')
    ->setCheckPermissions(FALSE)
    ->execute();
  \Civi\Api4\OptionValue::update()
    ->addWhere('option_group_id', '=', 6)
    ->addWhere('value', '=', 3)
    ->addValue('label', 'prof.')
    ->addValue('name', 'prof.')
    ->setCheckPermissions(FALSE)
    ->execute();

  // 5) advanced search options: no relationships (4)
  \Civi\Api4\Setting::set()
    ->addValue('advanced_search_options', [ 1, 2, 3, 5, 6, 10, 13, 16, 17, 18, 19, ])
    ->setCheckPermissions(FALSE)
    ->execute();
}
?>
