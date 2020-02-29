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
        'CiviEvent',
        'CiviCampaign',
      ])
      ->setCheckPermissions(FALSE)
      ->execute();

    // Enable CiviCRM extensions
    $extensions = array(
      'org.civicrm.shoreditch',
      'nz.co.fuzion.csvimport',
      'org.civicrm.sms.clickatell',
    );
    _drush_civicrmprofile_enable_extensions($extensions);

    // grant basic CRM permissions to anonymous user
    _drush_civicrmprofile_basic_permissions('anonymous user');

    // grant basic CRM permissions to authenticated user
    _drush_civicrmprofile_basic_permissions('authenticated user');

    // set basic CRM permissions to CRM user
    _drush_civicrmprofile_crmuser_permissions();

    // set basic CRM permissions to CRM coordinators
    _drush_civicrmprofile_crmcoordinator_permissions();

    // set basic CRM permissions to CRM communicators
    _drush_civicrmprofile_crmcommunicator_permissions();

    // set basic CRM permissions to CRM analysts
    _drush_civicrmprofile_crmanalyst_permissions();

    // grant full superuser permissions
    _drush_civicrmprofile_superuser_permissions();

    // grant CRM admin permissions
    _drush_civicrmprofile_crmadmin_permissions();

    // revoke all CRM permissions from site admin
    _drush_civicrmprofile_revoke_all_permissions('site admin');

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
function _drush_civicrmprofile_basic_permissions($role_name) {
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
        'edit inbound email basic information',
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

// set basic CRM permissions to CRM coordinators
function _drush_civicrmprofile_crmcoordinator_permissions() {
  $role_name='crm coord';
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'access CiviEvent',
        'access CiviMail',
        'edit event participants',
        'view event info',
        'edit inbound email basic information',
        'interview campaign contacts',
        'manage campaign',
        'register for events',
        'release campaign contacts',
        'reserve campaign contacts',
        'access CiviMail',
        'access my cases and activities',
        'access CiviReport',
      )
    )
  );
}

// set basic CRM permissions to CRM communicators
function _drush_civicrmprofile_crmcommunicator_permissions() {
  $role_name='crm comm';
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'edit message templates',
        'edit user-driven message templates',
        'send SMS',
        'access CiviMail',
        'access CiviMail subscribe/unsubscribe pages',
        'delete in CiviMail',
      )
    )
  );
}

// set basic CRM permissions to CRM analysts
function _drush_civicrmprofile_crmanalyst_permissions() {
  $role_name='crm analyst';
  _drush_civicrmprofile_revoke_all_permissions($role_name);
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'access CiviReport',
        'access Report Criteria',
        'save Report Criteria',
        'administer Reports',
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
