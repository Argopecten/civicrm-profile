<?php

/**
 * @file
 */

 use Drupal\Core\Database\Query\Condition;

/**
 * Implements hook_install_tasks().
 * https://api.drupal.org/api/drupal/core%21lib%21Drupal%21Core%21Extension%21module.api.php/function/hook_install_tasks/9.4.x
 * Any tasks you define here will be run, in order, after the installer has
 * finished the site configuration step but before it has moved on to the final
* import of languages and the end of the installation.
 */
function civicrmprofile_install_tasks($install_state){
  $tasks = [
    'civicrmprofile_l10nsetup' => [
      'display_name' => t('CiviCRM internationalization'),
      'display' => FALSE,
      'type' => 'normal',
    ],
  ];
  return $tasks;
}

/**
 * Install task to configure l10n settings in sites/all
 */
function civicrmprofile_l10nsetup() {
  $message='It"s the civicrmprofile_l10nsetup install task. civi d(): <pre><code>' . print_r(d(), TRUE) . '</code></pre>';
  \Drupal::logger('civicrmprofile')->debug($message);

  $context = d()->context_type;
  if ($context === "platform") {
    // platform install: define CIVICRM_L10N_BASEDIR environmental variable as platform settings in Aegir
    // cp -rl scripts/install-l10n/platform.settings.php web/sites/all/
    // sed -i "s/PLATFORM_DIR/$(basename $PWD)/g" web/sites/all/platform.settings.php

    $message='platform install: config XXXX';
    \Drupal::logger('civicrmprofile')->debug($message);


  } elseif ($context === "site") {
    // CRM site install
    $message='site install: do nothing here';
    \Drupal::logger('civicrmprofile')->debug($message);

  } else {
     // something is wrong...
     $message='wrong context: ' . $context;
     \Drupal::logger('civicrmprofile')->debug($warning);
  }
}

/**
 * Implements hook_modules_installed().
 * It performs actions when a module is installed
 *
 */
function civicrmprofile_modules_installed($modules) {
  // debug message
  $message='Entering civicrmprofile_modules_installed hook. Modules installed: <pre><code>' . print_r($modules, TRUE) . '</code></pre>';
  \Drupal::logger('civicrmprofile')->debug($message);
  // if (in_array('civicrmprofile', $modules)) {
    // CiviCRM profile is installed
  //  $message="The CiviCRM profile is installed";
  //  \Drupal::logger('civicrmprofile')->debug($message);
  // }
}
