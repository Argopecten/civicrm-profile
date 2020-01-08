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
  // drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  if ($context === "platform") {
    drush_log('context_type: ' . $context, 'debug');
    // do nothing
  } elseif ($context === "site") {
    // grant basic CRM permissions to anonymous user
    $role_name='anonymous user';
    _drush_civicrmprofile_grant_basic_permissions($role_name);

    // grant basic CRM permissions to authenticated user
    $role_name='authenticated user';
    _drush_civicrmprofile_grant_basic_permissions($role_name);

  } else {
    // something is wrong...
    drush_log('wrong context at drush_civicrmprofile_post_provision_verify: ' . $context, 'warning');
  }
}

function drush_civicrmprofile_post_provision_verify() {
  // drush_log('civicrmprofile d():' . print_r(d(), 1), 'debug');
  $context = d()->context_type;
  if ($context === "platform") {
    drush_log('context_type: ' . $context, 'debug');
    // do nothing
  } elseif ($context === "site") {
    // TODO
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
        'view my contacts',
        'edit my contacts',
        'access uploaded files',
        'profile create',
        'profile view',
      )
    )
  );
}

?>
