<?php

/**
 * @file
 */

 use Drupal\Core\Database\Query\Condition;

/**
 * Implements hook_install_tasks().
 */
function civicrmprofile_install_tasks($install_state){
  $tasks = array(
    'civicrmprofile_l10n_setup' => array(
      'display_name' => st('l10n setup'),
      'display' => FALSE,
      'type' => 'normal'
    ),
  );
  return $tasks;
}

/**
 * Install task to configure l10n settings in sites/all
 */
function civicrmprofile_l10n_setup() {
  $message='It"s the civicrmprofile_l10n_setup install task. civi d(): <pre><code>' . print_r(d(), TRUE) . '</code></pre>';
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

  if (in_array('civicrmprofile', $modules)) {
    // CiviCRM profile is installed
    $message="The CiviCRM profile module is installed";
    \Drupal::logger('civicrmprofile')->debug($message);

    // 1) enable CiviCRM standard components
    \Civi\Api4\Setting::set()
      ->addValue('enable_components', [
        'CiviContribute',
        'CiviMember',
        'CiviMail',
        'CiviCase',
        'CiviReport',
      ])
      ->setCheckPermissions(FALSE)
      ->execute();

    // 2) enable CiviCRM extensions
    $extensions = array(
      'org.civicrm.shoreditch',
      'oauth-client',
      'authx',
      'org.civicrm.afform_admin',
      'org.civicrm.contactlayout',
    );
    _civicrmprofile_crm_extensions('enable', $extensions);

    // 4) remove households
    _civicrmprofile_remove_households();

    // 5) set various CiviCRM options
    _civicrmprofile_civioptions();

  }

}

// Remove households
function _civicrmprofile_remove_households() {
  $message="Removing households from database";
  \Drupal::logger('civicrmprofile')->debug($message);

  // 1) disable contact type: UPDATE civicrm_contact_type SET is_active = 0 WHERE name = 'Household';
  \Drupal::database()->update('civicrm_contact_type')
    ->fields(array('is_active' => 0,))
    ->condition('name', 'Household')
    ->execute();

  // 2) disable the Household profile: UPDATE civicrm_uf_group set is_active = 0 where name = 'new_household';
  \Drupal::database()->update('civicrm_uf_group')
    ->fields(array('is_active' => 0,))
    ->condition('name', 'new_household')
    ->execute();

  // 3) relationship types
  //    delete relationships for households
  $db_or = new Condition('OR');
  $db_or->condition('contact_type_a', 'Household');
  $db_or->condition('contact_type_b', 'Household');
  \Drupal::database()->delete('civicrm_relationship_type')
    ->condition($db_or)
    ->execute();
  //     disable all the default relationship types
  \Drupal::database()->update('civicrm_relationship_type')
    ->fields(array('is_active' => 0,))
    ->where('1')
    ->execute();

  // 4) Remove the "New Household" menu item: UPDATE civicrm_navigation set is_active=0 where name='New Household';
  \Drupal::database()->update('civicrm_navigation')
    ->fields(array('is_active' => 0,))
    ->condition('name', 'New Household')
    ->execute();
}

function _civicrmprofile_crm_extensions($action, $extensions) {
  // download is done via composer
  // TBD: move CiviCRM Api4

  $message=$action . ' following CiviCRM extensions: <pre><code>' . print_r($extensions, TRUE) . '</code></pre>';
  \Drupal::logger('civicrmprofile')->debug($message);

  foreach ($extensions as $key => $ext) {
    try{
      civicrm_api3('Extension', $action, ['key' => $ext,]);
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
      $message=$action . ' extension has failed: <pre><code>' . print_r($result, TRUE) . '</code></pre>';
      \Drupal::logger('civicrmprofile')->warning($message);
    }
  }
}

/**
 * _civicrmprofile_civioptions()
 *
 * set various CiviCRM options: display, name format, ...
 * via API4 calls
 * settings:
 *   - contact options
 *   - mobile phone providers for HU
 *   - phone types
 *   - individual prefixes
 */
function _civicrmprofile_civioptions() {
  // 1) contact options
  \Civi\Api4\Setting::set()
    ->addValue('contact_view_options', [1, 2, 3, 4, 5, 6, 10, 14, ])
    ->addValue('contact_edit_options', [12, 14, 15, 16, 7, 8, 5, 1, 2, 3, 4, 6, ])
    ->addValue('user_dashboard_options', [1, 5, 9, ])
    ->addValue('display_name_format', '{contact.individual_prefix}{ }{contact.last_name}{ }{contact.first_name}')
    ->setCheckPermissions(FALSE)
    ->execute();

}
