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
  $message='It"s the drush_civicrmprofile_pre_provision_install hook';
  \Drupal::logger('civicrmprofile')->debug($message);
}

function drush_civicrmprofile_provision_install() {
  // just a spaceholder for now
  $message='It"s the drush_civicrmprofile_provision_install hook';
  \Drupal::logger('civicrmprofile')->debug($message);
}

function drush_civicrmprofile_post_provision_install() {
  // just a spaceholder for now
  // $message='It"s the drush_civicrmprofile_post_provision_install hook';
  $message='It"s the drush_civicrmprofile_post_provision_install hook. civi d(): <pre><code>' . print_r(d(), TRUE) . '</code></pre>';
  \Drupal::logger('civicrmprofile')->debug($message);

  $context = d()->context_type;
  $message='Context_type: ' . $context;
  \Drupal::logger('civicrmprofile')->debug($message);

  if ($context === "platform") {
    // do nothing
  } elseif ($context === "site") {
    // do nothing
  } else {
    // something is wrong...
    // drush_log('wrong context at drush_civicrmprofile_post_provision_install: ' . $context, 'warning');
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
   $message='It"s the drush_civicrmprofile_pre_provision_verify hook';
   \Drupal::logger('civicrmprofile')->debug($message);
 }

 function drush_civicrmprofile_provision_verify() {
   // just a spaceholder for now
   $message='It"s the drush_civicrmprofile_provision_verify hook';
   \Drupal::logger('civicrmprofile')->debug($message);
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
  // drush_log('civi d():' . print_r(d(), 1), 'debug');
  $message='It"s the drush_civicrmprofile_post_provision_verify hook. civi d(): <pre><code>' . print_r(d(), TRUE) . '</code></pre>';
  \Drupal::logger('civicrmprofile')->debug($message);

  $context = d()->context_type;
  $message='Context_type: ' . $context;
  \Drupal::logger('civicrmprofile')->debug($message);

  if ($context === "platform") {
    // do nothing
  } elseif ($context === "site") {

    // enable CiviCRM components

    // Enable CiviCRM extensions

    // grant basic CRM permissions to anonymous user

  } else {
    // something is wrong...
    // drush_log('wrong context at drush_civicrmprofile_post_provision_verify: ' . $context, 'warning');
  }
}

?>
