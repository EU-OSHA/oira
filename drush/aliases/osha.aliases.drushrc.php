<?php

$aliases['staging'] = array(
  'uri' => 'osha.edw.ro',
  'db-allows-remote' => TRUE,
  'remote-host' => 'osha.edw.ro',
  'remote-user' => 'osha',
  'root' => '/var/www/html/docroot',
  'path-aliases' => array(
    '%files' => 'sites/default/files',
  ),
  'command-specific' => array(
    'sql-sync' => array(
      'simulate' => '1',
      'source-dump' => '/tmp/source-dump.sql',
    ),
  ),
);

// This alias is used in install and update scripts.
// Rewrite it in your aliases.local.php as you need.

// Example of local setting.
// $aliases['osha.local'] = array(
//   'uri' => 'osha.localhost',
//   'root' => '/home/my_user/osha-website/docroot',
// );

// Add your local aliases.
if (file_exists(dirname(__FILE__) . '/aliases.local.php')) {
  include dirname(__FILE__) . '/aliases.local.php';
}
