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

    // set basic CRM permissions to CRM user
    _drush_civicrmprofile_grant_crmuser_permissions(t('crm user'));

    // grant full superuser permissions
    _drush_civicrmprofile_superuser_permissions();

    // grant CRM admin permissions
    _drush_civicrmprofile_crmadmin_permissions();

    // grant CRM activist permissions
    _drush_civicrmprofile_activist_permissions();

    // revoke all CRM permissions from site admin
    _drush_civicrmprofile_revoke_all_permissions(t('site admin'));

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
        'access AJAX API',
        'register to volunteer',
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
        'add contacts',
        'view my contact',
        'edit my contact',
        'access CiviCRM',
        'access Contact Dashboard',
        'add contact notes',
        'access CiviMail',
        'edit inbound email basic information',
        'log own hours',
      )
    )
  );
}

// grant superuser permissions
function _drush_civicrmprofile_superuser_permissions() {
  $role_name=t('super user');
  // revoke all permissions and grant new CRM permissions as CRM user
  _drush_civicrmprofile_grant_crmuser_permissions($role_name);

  // grant extra permissions for 'super user'
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'edit all contacts',
        'delete contacts',
        'access deleted contacts',
        'import contacts',
        'profile listings and forms',
        'profile edit',
        'access all custom data',
        'view all notes',
        'view all activities',
        'delete activities',
        'edit groups',
        'manage tags',
        'administer Tagsets',
        'edit user-driven message templates',
        'create volunteer projects',
        'edit all volunteer projects',
        'delete all volunteer projects ',
      )
    )
  );
}

// grant CRM admin permissions
function _drush_civicrmprofile_crmadmin_permissions() {
  $role_name=t('crm admin');
  // revoke all permissions and grant new CRM permissions as CRM user
  _drush_civicrmprofile_grant_crmuser_permissions($role_name);

  // grant extra permissions for 'crm admin'
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'administer CiviCRM',
        'administer reserved tags',
        'administer reserved groups',
        'edit system workflow message templates',
        'edit volunteer project relationships ',
        'edit volunteer registration profiles ',
      )
    )
  );
}

// grant CRM activist permissions
function _drush_civicrmprofile_activist_permissions() {
  $role_name='activist';
  // revoke all permissions and grant new CRM permissions as CRM user
  _drush_civicrmprofile_grant_crmuser_permissions($role_name);

  // grant extra permissions for 'activist'
  drush_invoke('role-add-perm',
    array(
      $role_name,
      array(
        'create volunteer projects',
        'edit own volunteer projects',
        'delete own volunteer projects',
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
